#ifdef USE_HIP

#include "fractal/mandelbrot.hpp"

#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_complex.h>

#include <iostream>

#define DEVICE_INLINE_FUNCTION __forceinline__ __device__

using Complex = hipDoubleComplex;

template <typename F, typename T = Complex>
concept FractalFunction = std::invocable<F, T, T> && requires(F f, T a, T b) {
  { f(a, b) } -> std::same_as<T>;
};

#define HIP_CHECK(condition)                                             \
  {                                                                      \
    hipError_t err = condition;                                          \
    if (err != hipSuccess) {                                             \
      std::cerr << __FILE__ << ":" << __LINE__ << "\t"                   \
                << "HIP error: " << hipGetErrorString(err) << std::endl; \
    }                                                                    \
  }

DEVICE_INLINE_FUNCTION Complex scale(const WindowDim<uint32_t> &screen,
                                     const WindowDim<double> &fr, Complex c) noexcept {
  // Avoid repeated calculations by storing width and height ratios
  const double x_ratio = fr.width() / static_cast<double>(screen.width());
  const double y_ratio = fr.height() / static_cast<double>(screen.height());

  return make_hipDoubleComplex(hipCreal(c) * x_ratio + fr.x_min(),
                               hipCimag(c) * y_ratio + fr.y_min());
}

template <FractalFunction Fractal_t>
DEVICE_INLINE_FUNCTION uint32_t escape(Complex c, uint32_t iter_max, Fractal_t func) {
  Complex z     = make_hipDoubleComplex(0, 0);
  uint32_t iter = 0;

  while (hipCabs(z) < 2.0 && iter < iter_max) {
    z = func(z, c);
    iter++;
  }

  return iter;
}

template <FractalFunction Fractal_t>
__global__ void get_number_iterations(const WindowDim<uint32_t> &screen,
                                      const WindowDim<double> &fract, uint32_t iter_max,
                                      uint32_t *escape_step, Fractal_t func) {
  const uint32_t col    = threadIdx.x;
  const uint32_t row    = blockIdx.x;
  const uint32_t stride = blockDim.x;
  const uint32_t width  = screen.width();

  extern __shared__ uint32_t row_data[];

  for (uint32_t j = col; j < width; j += stride) {
    Complex c   = make_hipDoubleComplex((double)j, (double)row);
    c           = scale(screen, fract, c);
    row_data[j] = escape(c, iter_max, func);
  }

  __syncthreads();

  for (uint32_t j = col; j < width; j += stride) {
    escape_step[row * width + j] = row_data[j];
  }
}

void mandelbrot(const WindowDim<uint32_t> screen, const WindowDim<double> fract,
                uint32_t *escape_step, uint32_t iter_max) {
  constexpr auto func = [](const Complex z, const Complex c) noexcept -> Complex {
    return hipCfma(z, z, c);
  };
  dim3 block_size(256, 1, 1);
  dim3 grid_size(screen.height(), 1, 1);
  uint32_t *d_escape_step;
  const size_t alloca_size = screen.size() * sizeof(uint32_t);

  HIP_CHECK(hipSetDevice(0));
  HIP_CHECK(hipMalloc((void**)&d_escape_step, alloca_size));
  HIP_CHECK(hipMemset(d_escape_step, 0, alloca_size));
  
  hipLaunchKernelGGL(HIP_KERNEL_NAME(get_number_iterations<decltype(func)>), grid_size,
                     block_size, screen.width() * sizeof(uint32_t), 0, screen, fract,
                     iter_max, d_escape_step, func);
  HIP_CHECK(hipDeviceSynchronize());
  HIP_CHECK(hipGetLastError());
  HIP_CHECK(hipMemcpy(escape_step, d_escape_step, alloca_size, hipMemcpyDeviceToHost));
  HIP_CHECK(hipFree(d_escape_step));
  return;
}

#endif