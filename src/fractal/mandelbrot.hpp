#pragma once

#include <cstdint>

#include "core/window_dim.hpp"
#include "core/window_utils.hpp"

// For each pixel of the screen compute the number of step to 
// escape form sphere of radius 2 and center 0.
void mandelbrot(const WindowDim<uint32_t> screen, const WindowDim<double> fract,
                uint32_t *escape_step, uint32_t iter_max);
