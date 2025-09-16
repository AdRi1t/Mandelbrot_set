#pragma once

#include <cstdint>
#include <string>

#include "core/window_dim.hpp"

// Used to get generate file names
std::string now_to_string();
std::string pos_to_string(double x, double y, double zoom_level);

void save_image(WindowDim<uint32_t> &scr, uint32_t *colors, uint32_t iter_max,
                const char *fname);

namespace ColorSchemes {

// [r,g,b]
using ColorScheme = std::tuple<uint8_t, uint8_t, uint8_t>;

ColorScheme scheme_1(uint32_t n, uint32_t iter_max);
ColorScheme scheme_2(uint32_t n, uint32_t iter_max);
ColorScheme scheme_3(uint32_t n, uint32_t iter_max);
ColorScheme scheme_4(uint32_t n, uint32_t iter_max);
ColorScheme scheme_5(uint32_t n, uint32_t iter_max);
// Ugly piecewise linear color mapping
ColorScheme piecewise_linear(uint32_t n, uint32_t iter_max);

ColorScheme get_color(uint32_t n, uint32_t iter_max);

}  // namespace ColorSchemes
