#ifdef USE_OPENMP

#include "fractal/mandelbrot.hpp"

#include <omp.h>
#include <complex>

// Use an alias to simplify the use of complex type
using Complex = std::complex<double>;

template <typename F, typename T = Complex>
concept FractalFunction = std::invocable<F, T, T> && requires(F f, T a, T b) {
  { f(a, b) } -> std::same_as<T>;
};

// Convert a pixel coordinate to the complex domain
Complex scale(const WindowDim<uint32_t> &screen, const WindowDim<double> &fr,
              Complex c) noexcept {
  // Avoid repeated calculations by storing width and height ratios
  const double x_ratio = fr.width() / static_cast<double>(screen.width());
  const double y_ratio = fr.height() / static_cast<double>(screen.height());

  return Complex(c.real() * x_ratio + fr.x_min(), c.imag() * y_ratio + fr.y_min());
}

// Check if a point is in the set or escapes to infinity, return the number if iterations
template <FractalFunction Fractal_t>
uint32_t escape(Complex c, uint32_t iter_max, Fractal_t func) {
  Complex z(0);
  uint32_t iter = 0;

  while (abs(z) < 2.0 && iter < iter_max) {
    z = func(z, c);
    iter++;
  }

  return iter;
}

// Loop over each pixel from our image and check if the points associated with this pixel
// escape to infinity
template <FractalFunction Fractal_t>
void get_number_iterations(const WindowDim<uint32_t> &screen,
                           const WindowDim<double> &fract, uint32_t iter_max,
                           uint32_t *escape_step, Fractal_t func) {
  const uint32_t width    = screen.width();
  const uint32_t height   = screen.height();
  const uint32_t total_px = width * height;

// iterate through pixel
#pragma omp parallel for schedule(dynamic)
  for (uint32_t k = 0; k < total_px; ++k) {
    uint32_t i = k / width + screen.y_min();
    uint32_t j = k % width + screen.x_min();
    Complex c((double)j, (double)i);
    c              = scale(screen, fract, c);
    escape_step[k] = escape(c, iter_max, func);
  }
}

void mandelbrot(const WindowDim<uint32_t> screen, const WindowDim<double> fract,
                uint32_t *escape_step, uint32_t iter_max) {
  // The function used to calculate the fractal
  constexpr auto func = [](const Complex z, const Complex c) noexcept -> Complex {
    return z * z + c;
  };
  get_number_iterations(screen, fract, iter_max, escape_step, func);
  return;
}

#endif  // USE_SEQUENTIAL
