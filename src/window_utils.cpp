#include "window_utils.h"

namespace WindowUtils  {

// Experimental zoom functions - use with care
void zoom(const double window_ratio, const double x0, const double x1, const double y0,
          [[maybe_unused]] const double y1, WindowDim<double> &fract) {
  double height = (x1 - x0) / window_ratio;
  fract.reset(x0, x1, y0, y0 + height);
}

void zoom(const double x0, const double x1, const double y0, const double y1,
          WindowDim<double> &fract) {
  fract.reset(x0, x1, y0, y1);
}

void zoom(const double center_x, const double center_y, const double new_zoom_level,
          WindowDim<double> &fract) {
  const double zoom_factor = fract.get_zoom_level() / new_zoom_level;
  double current_width     = fract.width();
  double current_height    = fract.height();

  double new_width  = current_width * zoom_factor;
  double new_height = current_height * zoom_factor;

  double new_x0 = center_x - new_width / 2;
  double new_x1 = center_x + new_width / 2;
  double new_y0 = center_y - new_height / 2;
  double new_y1 = center_y + new_height / 2;

  fract.reset(new_x0, new_x1, new_y0, new_y1, new_zoom_level);
}

void move(const double dx, const double dy, WindowDim<double> &fract) {
  const double x0 = fract.x_min();
  const double x1 = fract.x_max();
  const double y0 = fract.y_min();
  const double y1 = fract.y_max();

  fract.reset(x0 + dx, x1 + dx, y0 + dy, y1 + dy);
}

}  // namespace WindowDim