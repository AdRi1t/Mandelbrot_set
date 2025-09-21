#pragma once

#include <cstdint>
#include <tuple>

namespace ColorSchemes {

// [r,g,b]
using RGB_t = std::tuple<uint8_t, uint8_t, uint8_t>;

RGB_t scheme_1(uint32_t n, uint32_t iter_max);
RGB_t scheme_2(uint32_t n, uint32_t iter_max);
RGB_t scheme_3(uint32_t n, uint32_t iter_max);
RGB_t scheme_4(uint32_t n, uint32_t iter_max);
RGB_t scheme_5(uint32_t n, uint32_t iter_max);
// Ugly piecewise linear color mapping
RGB_t piecewise_linear(uint32_t n, uint32_t iter_max);

RGB_t get_color(uint32_t n, uint32_t iter_max);

}  // namespace ColorSchemes
