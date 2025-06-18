#ifndef SAVE_IMAGE__H
#define SAVE_IMAGE__H

#include "window_dim.hpp"
#include <string>

std::string now_to_string();

void plot(WindowDim<uint32_t> &scr, uint32_t* colors, uint32_t iter_max, const char *fname,
          bool smooth_color);

std::tuple<uint8_t, uint8_t, uint8_t> get_rgb_smooth(uint32_t n, uint32_t iter_max) ;

#endif
