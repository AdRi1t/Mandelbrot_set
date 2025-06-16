#ifndef SAVE_IMAGE__H
#define SAVE_IMAGE__H

#include "window_dim.hpp"
#include <string>

std::string now_to_string();

void plot(WindowDim<uint32_t> &scr, int* colors, uint32_t iter_max, const char *fname,
          bool smooth_color);

#endif
