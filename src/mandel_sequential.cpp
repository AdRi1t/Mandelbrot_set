#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <functional>

#include "mandelbrot.hpp"
#include "global_config.hpp"
#include "save_image.h"
#include "window_utils.h"

// Use an alias to simplify the use of complex type
using Complex = std::complex<double>;

template <typename F, typename T = Complex>
concept FractalFunction = std::invocable<F, T, T> && requires(F f, T a, T b) {
  { f(a, b) } -> std::same_as<T>;
};

// Convert a pixel coordinate to the complex domain
Complex scale(WindowDim<uint32_t> &screen, WindowDim<double> &fr, Complex c) {
  return Complex(c.real() / (double)screen.width() * fr.width() + fr.x_min(),
                 c.imag() / (double)screen.height() * fr.height() + fr.y_min());
}

// Check if a point is in the set or escapes to infinity, return the number if iterations
template <FractalFunction Fractal_t>
int escape(Complex c, uint32_t iter_max, Fractal_t func) {
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
void get_number_iterations(WindowDim<uint32_t> &screen, WindowDim<double> &fract,
                           uint32_t iter_max, int *colors, Fractal_t func) {
  int k = 0;
  // iterate through pixel
  for (uint32_t i = screen.y_min(); i < screen.y_max(); ++i) {
    for (uint32_t j = screen.x_min(); j < screen.x_max(); ++j) {
      Complex c((double)j, (double)i);
      c         = scale(screen, fract, c);
      colors[k] = escape(c, iter_max, func);
      k++;
    }
  }
}

template <FractalFunction Fractal_t>
void fractal(WindowDim<uint32_t> &screen, WindowDim<double> &fract, uint32_t iter_max,
             int *colors, Fractal_t func, const char *fname, bool smooth_color) {
  auto start = std::chrono::steady_clock::now();
  get_number_iterations(screen, fract, iter_max, colors, func);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Time to generate " << fname << " = "
            << std::chrono::duration<double, std::milli>(end - start).count() << " [ms]"
            << std::endl;

  // Save (show) the result as an image
  plot(screen, colors, iter_max, fname, smooth_color);
}

void mandelbrot(WindowDim<uint32_t> screen, WindowDim<double> fract) {
  // The function used to calculate the fractal

  constexpr auto func = [](const Complex z, const Complex c) noexcept -> Complex {
    return z * z + c;
  };
  /*
    constexpr auto func = [](const Complex z, const Complex c) noexcept -> Complex {
      return z * z * z + c;
    };
  */
  uint32_t iter_max = GlobalConfig::get_iter_max();
  std::string fname = now_to_string() + "_mandelbrot.png";
  bool smooth_color = true;
  int *colors       = new int[screen.size()];

  // Experimental zoom (bugs ?). This will modify the fract window (the domain in which we
  // calculate the fractal function)
  // zoom(-1.28, -1.23, 0.01, 0.08, fract);  // Z2

  fractal(screen, fract, iter_max, colors, func, fname.c_str(), smooth_color);
  delete[] colors;
  return;
}
