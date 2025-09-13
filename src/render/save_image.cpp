#include "save_image.hpp"

#include <ctime>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <concepts>

#include <FreeImage.h>

#include "core/window_dim.hpp"

template <typename F, typename T>
concept ColorSchemeFunction =
    std::invocable<F, T, T> && std::integral<T> && requires(F f, T n, T max) {
      { f(n, max) } -> std::convertible_to<std::tuple<uint8_t, uint8_t, uint8_t>>;
    };

std::tuple<uint8_t, uint8_t, uint8_t> get_rgb_piecewise_linear(uint32_t n,
                                                               uint32_t iter_max) {
  int N  = 256;  // colors per element
  int N3 = N * N * N;
  // map n on the 0..1 interval (real numbers)
  double t = (double)n / (double)iter_max;
  // expand n on the 0 .. 256^3 interval (integers)
  n = (int)(t * (double)N3);

  uint8_t b = n / (N * N);
  int nn    = n - b * N * N;
  uint8_t r = nn / N;
  uint8_t g = nn - r * N;
  return std::make_tuple(r, g, b);
}

std::tuple<uint8_t, uint8_t, uint8_t> get_rgb_scheme_1(uint32_t n, uint32_t iter_max) {
  // map n on the 0..1 interval
  double t = (double)n / (double)iter_max;

  // Use smooth polynomials for r, g, b
  uint8_t r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
  uint8_t g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
  uint8_t b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
  return std::make_tuple(r, g, b);
}

std::tuple<uint8_t, uint8_t, uint8_t> get_rgb_scheme_2(uint32_t n, uint32_t iter_max) {
  // map n on the 0..1 interval
  double t = (double)n / (double)iter_max;

  // Use smooth polynomials for r, g, b
  uint8_t b = (uint8_t)(9 * (1 - t) * t * t * t * 255);
  uint8_t g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
  uint8_t r = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
  return std::make_tuple(r, g, b);
}

std::tuple<uint8_t, uint8_t, uint8_t> get_rgb_scheme_3(uint32_t n, uint32_t iter_max) {
  // map n on the 0..1 interval
  double t = (double)n / (double)iter_max;

  // Use smooth polynomials for r, g, b
  uint8_t r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
  uint8_t g = (uint8_t)(8 * (1 - t) * (1 - t) * t * t * 255);
  uint8_t b = (uint8_t)(9 * (1 - t) * (1 - t) * (1 - t) * t * 255);
  return std::make_tuple(r, g, b);
}

std::string now_to_string() {
  time_t t    = time(nullptr);
  tm *tm_time = localtime(&t);

  if (tm_time == nullptr) {
    std::cerr << "Error: localtime() \n";
    return std::string("DATE");
  }

  std::ostringstream string_stream;
  string_stream << tm_time->tm_year + 1900 << "-" << std::setfill('0') << std::setw(2)
                << tm_time->tm_mon + 1 << "-" << std::setfill('0') << std::setw(2)
                << tm_time->tm_mday << "_" << std::setfill('0') << std::setw(2)
                << tm_time->tm_hour << ":" << std::setfill('0') << std::setw(2)
                << tm_time->tm_min << ":" << std::setfill('0') << std::setw(2)
                << tm_time->tm_sec << std::setfill('0') << std::setw(2);

  std::string result = string_stream.str();
  return result;
}

std::string pos_to_string(double x, double y, double zoom_level) {
  std::ostringstream string_stream;
  string_stream << "(" << x << "," << y << ")x" << zoom_level;
  std::string result = string_stream.str();
  return result;
}

void save_image(WindowDim<uint32_t> &scr, uint32_t *escape_step, uint32_t iter_max,
                const char *fname) {
// active only for static linking
#ifdef FREEIMAGE_LIB
  FreeImage_Initialise();
#endif

  uint32_t width   = scr.width();
  uint32_t height  = scr.height();
  FIBITMAP *bitmap = FreeImage_Allocate(width, height, 32);  // RGBA

  int k = 0;
  std::tuple<uint8_t, uint8_t, uint8_t> rgb;

  for (uint32_t i = scr.y_min(); i < scr.y_max(); ++i) {
    for (uint32_t j = scr.x_min(); j < scr.x_max(); ++j) {
      int n = escape_step[k];

      rgb = get_rgb_scheme_1(n, iter_max);

      RGBQUAD col;
      col.rgbRed      = std::get<0>(rgb);
      col.rgbGreen    = std::get<1>(rgb);
      col.rgbBlue     = std::get<2>(rgb);
      col.rgbReserved = 255;

      FreeImage_SetPixelColor(bitmap, j, i, &col);
      k++;
    }
  }

  // Save the image in a PNG file
  FreeImage_Save(FIF_PNG, bitmap, fname, PNG_DEFAULT);

  // Clean bitmap;
  FreeImage_Unload(bitmap);

// active only for static linking
#ifdef FREEIMAGE_LIB
  FreeImage_DeInitialise();
#endif
}
