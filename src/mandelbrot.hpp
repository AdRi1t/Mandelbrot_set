#pragma once

#include "window_dim.hpp"

void mandelbrot(const WindowDim<uint32_t> screen, const WindowDim<double> fract, uint32_t iter_max,
                uint32_t *escape_step);
void mandelbrot(const WindowDim<uint32_t> screen, const WindowDim<double> fract,
                uint32_t *escape_step);
