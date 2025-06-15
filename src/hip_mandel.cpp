#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>

#define HIP_CHECK(condition)                                           \
  hipError_t err = condition;                                          \
  if (err != hipSuccess) {                                             \
    std::cerr << __FILE__ << ":" << __LINE__ << "\t"                   \
              << "HIP error: " << hipGetErrorString(err) << std::endl; \
  }

void get_number_iterations(window<int> &scr, window<double> &fract, int iter_max,
                           std::vector<int> &colors,
                           const std::function<Complex(Complex, Complex)> &func) {
  const size_t alloca_size = fract.size() * sizeof(double);
  double                   = d_scr;
  hip_malloc(&d_scr, alloca_size);
}