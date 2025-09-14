#include "global_config.hpp"

#include <iomanip>

namespace GlobalConfig {
namespace {
ConfigData _config_data{.iter_max       = 400,
                        .color_scheme   = 1,
                        .zoom_level     = 1.0,
                        .center_x       = 0.0,
                        .center_y       = 0.0,
                        .fract_width    = 0.0,
                        .fract_height   = 0.0,
                        .need_redraw    = true,
                        .window_resized = false};
std::mutex _config_mutex;

std::condition_variable _render_condition;
std::mutex _render_mutex;
}  // namespace

#define TRY_CATCH_DEFAULT_VALUE(var, expr, value)        \
  try {                                                  \
    var = expr;                                          \
  } catch (const std::exception& e) {                    \
    std::cerr << __FILE__ << ":" << __LINE__ << "\t"     \
              << "Exception: \"" << e.what() << "\"\t"   \
              << "Default value set: " << value << "\n"; \
    var = value;                                         \
  }

void parse_from_argv(int argc, char* argv[]) {
  std::lock_guard<std::mutex> lock(_config_mutex);

  for (int i = 1; i < argc; i++) {
    std::string_view arg(argv[i]);
    if (arg == "-h" || arg == "--help") {
      std::cout << "Mandelbrot \n"
                << "Options: \n"
                << " [-i <int>]            iter max\n"
                << " [-c <float> <float>]  center \n"
                << " [-z <float>]          zoom level \n"
                << " [-h, --help]          for help" << std::endl;
      exit(0);
    } else if (arg == "-i" && i + 1 < argc) {
      TRY_CATCH_DEFAULT_VALUE(_config_data.iter_max, std::stoi(argv[++i]), 500)
    } else if (arg == "-c" && i + 2 < argc) {
      TRY_CATCH_DEFAULT_VALUE(_config_data.center_x, std::stod(argv[++i]), 0.0)
      TRY_CATCH_DEFAULT_VALUE(_config_data.center_y, std::stod(argv[++i]), 0.0)
    } else if (arg == "-z" && i + 1 < argc) {
      TRY_CATCH_DEFAULT_VALUE(_config_data.zoom_level, std::stod(argv[++i]), 0.0)
    }
  }
}

void change_iter_max(int delta_iter_max) {
  std::lock_guard lock(_config_mutex);
  if (_config_data.iter_max + delta_iter_max > 0) {
    _config_data.iter_max += delta_iter_max;
  }
}

void change_zoom(double zoom_factor) {
  std::lock_guard lock(_config_mutex);
  _config_data.zoom_level *= zoom_factor;
}

void set_fractal_dim(double new_width, double new_height) {
  std::lock_guard lock(_config_mutex);
  _config_data.fract_width  = new_width;
  _config_data.fract_height = new_height;
}

void set_window_resized(bool resized) {
  std::lock_guard lock(_config_mutex);
  _config_data.window_resized = resized;
}

void switch_color_scheme() {
  std::lock_guard lock(_config_mutex);
  _config_data.color_scheme++;
}

bool is_window_resized() { return _config_data.window_resized; }

std::pair<double, double> get_fractal_dim() {
  return std::make_pair(_config_data.fract_width, _config_data.fract_height);
}

void need_redraw() {
  {
    std::lock_guard lock(_render_mutex);
    _config_data.need_redraw = true;
  }
  _render_condition.notify_all();
}

void wait_to_draw() {
  std::unique_lock lock(_render_mutex);
  _render_condition.wait(lock, []() { return _config_data.need_redraw; });
  _config_data.need_redraw = false;
}

void move_center(double dx, double dy) {
  std::lock_guard lock(_config_mutex);
  _config_data.center_x += dx;
  _config_data.center_y += dy;
}

// getters
uint32_t get_iter_max() { return _config_data.iter_max; }

double get_zoom_level() { return _config_data.zoom_level; }

std::pair<double, double> get_center() {
  return std::make_pair(_config_data.center_x, _config_data.center_y);
}

uint32_t get_color_scheme() { return _config_data.color_scheme; }

void print_configuration() {
  std::cout << "Current Configuration:\n"
            << "  Max iterations: " << _config_data.iter_max << "\n"
            << "  Zoom level: " << _config_data.zoom_level << "\n"
            << "  Center: (" << _config_data.center_x << ", " << _config_data.center_y
            << ")\n";
}

}  // namespace GlobalConfig

namespace LogInfo {
namespace {
LogData _log_data{.fractal_time_ms = 0.0, .display_time_ms = 0.0};
std::mutex _log_mutex;
}  // namespace

void print_log() {
  std::lock_guard<std::mutex> lock(_log_mutex);
  std::cout << std::scientific << std::setprecision(6);
  std::cout << "Log Information:\n"
            << "  Center:       ( " << GlobalConfig::get_center().first << ", "
            << GlobalConfig::get_center().second << ")\n"
            << "  Zoom level:     " << GlobalConfig::get_zoom_level() << "\n"
            << std::defaultfloat << "  Iterations max: " << GlobalConfig::get_iter_max()
            << "\n\n"
            << "  Fractal time: " << _log_data.fractal_time_ms << " ms\n"
            << "  Display time: " << _log_data.display_time_ms << " ms\n";

  if (!std::cout) {
    std::cerr << "Error: Failed to write log information to standard output.\n";
  }
}

void set_fractal_time(double time_ms) {
  std::lock_guard<std::mutex> lock(_log_mutex);
  _log_data.fractal_time_ms = time_ms;
}

void set_display_time_ms(double time_ms) {
  std::lock_guard<std::mutex> lock(_log_mutex);
  _log_data.display_time_ms = time_ms;
}

}  // namespace LogInfo