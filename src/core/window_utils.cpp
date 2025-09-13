#include "core/window_utils.hpp"

#include <cmath>
#include <iostream>

namespace WindowUtils {

// Experimental zoom functions - use with care
void zoom(const double window_ratio, const double x0, const double x1, const double y0,
          [[maybe_unused]] const double y1, WindowDim<double>* fract) {
  double height = (x1 - x0) / window_ratio;
  fract->reset(x0, x1, y0, y0 + height);
}

void zoom(const double x0, const double x1, const double y0, const double y1,
          WindowDim<double>* fract) {
  fract->reset(x0, x1, y0, y1);
}

void zoom(const double center_x, const double center_y, const double new_zoom_level,
          WindowDim<double>* fract) {
  const double old_zoom_level = fract->get_zoom_level();
  double current_width        = fract->width();
  double current_height       = fract->height();

  double new_width  = current_width * (old_zoom_level / new_zoom_level);
  double new_height = current_height * (old_zoom_level / new_zoom_level);

  const double half_width  = new_width * 0.5;
  const double half_height = new_height * 0.5;

  double new_x0 = center_x - half_width;
  double new_x1 = center_x + half_width;
  double new_y0 = center_y - half_height;
  double new_y1 = center_y + half_height;

  fract->reset(new_x0, new_x1, new_y0, new_y1, new_zoom_level);
}

void move(const double dx, const double dy, WindowDim<double>* fract) {
  const double x0 = fract->x_min();
  const double x1 = fract->x_max();
  const double y0 = fract->y_min();
  const double y1 = fract->y_max();

  fract->reset(x0 + dx, x1 + dx, y0 + dy, y1 + dy);
}

void adjust_ratio(const WindowDim<uint32_t>* screen, WindowDim<double>* fract) {
  double window_ratio =
      static_cast<double>(screen->width()) / static_cast<double>(screen->height());

  double fract_width  = fract->width();
  double fract_height = fract->height();
  double fract_ratio  = fract_width / fract_height;

  double center_x = fract->x_min() + fract_width * 0.5;
  double center_y = fract->y_min() + fract_height * 0.5;
  if (fract_ratio < window_ratio) {
    double new_width = fract_height * window_ratio;
    double new_x_min = center_x - new_width * 0.5;
    double new_x_max = center_x + new_width * 0.5;
    fract->reset(new_x_min, new_x_max, fract->y_min(), fract->y_max());
  } else if (fract_ratio > window_ratio) {
    double new_height = fract_width / window_ratio;
    double new_y_min  = center_y - new_height * 0.5;
    double new_y_max  = center_y + new_height * 0.5;
    fract->reset(fract->x_min(), fract->x_max(), new_y_min, new_y_max);
  }
}

}  // namespace WindowUtils
