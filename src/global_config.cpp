#include "global_config.hpp"

namespace GlobalConfig {
namespace {
ConfigData _config_data;
std::mutex _config_mutex;
}  // namespace

void parse_from_argv(int argc, char* argv[]) {
  std::lock_guard<std::mutex> lock(_config_mutex);
  _config_data.iter_max   = 400;
  _config_data.zoom_level = 1.0;
  _config_data.center_x   = 0.0;
  _config_data.center_y   = 0.0;
  for (int i = 1; i < argc; i++) {
    std::string_view arg(argv[i]);
    if (arg == "-h") {
      std::cout << "Mandelbrot \n"
                << "Options: \n"
                << " [-I <int>]           iter max\n"
                << " [-C <float> <float>] center \n"
                << " [-Z <float>]         zoom level \n"
                << " [-h]                 for help" << std::endl;
      exit(0);
    } else if (arg == "-I" && i + 1 < argc) {
      _config_data.iter_max = std::stoi(argv[++i]);
    } else if (arg == "-C" && i + 2 < argc) {
      _config_data.center_x = std::stod(argv[++i]);
      _config_data.center_y = std::stod(argv[++i]);
    } else if (arg == "-Z" && i + 1 < argc) {
      _config_data.zoom_level = std::stod(argv[++i]);
    }
  }
}

void change_iter_max(int delta_iter_max) {
  std::lock_guard<std::mutex> lock(_config_mutex);
  if (_config_data.iter_max + delta_iter_max > 0) {
    _config_data.iter_max += delta_iter_max;
  }
}

void change_zoom(double zoom_factor) {
  std::lock_guard<std::mutex> lock(_config_mutex);
  _config_data.zoom_level *= zoom_factor;
}

void set_fractDim(double new_width, double new_height) {
  std::lock_guard<std::mutex> lock(_config_mutex);
  _config_data.fract_width  = new_width;
  _config_data.fract_height = new_height;
}

std::pair<double, double> get_fractDim() {
  return std::make_pair(_config_data.fract_width, _config_data.fract_height);
}

void move_center(double dx, double dy) {
  std::lock_guard<std::mutex> lock(_config_mutex);
  _config_data.center_x += dx;
  _config_data.center_y += dy;
}

// getters
uint32_t get_iter_max() { return _config_data.iter_max; }

double get_zoom_level() { return _config_data.zoom_level; }

std::pair<double, double> get_center() {
  return std::make_pair(_config_data.center_x, _config_data.center_y);
}

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
LogData _log_data{0, 0, 1, 0};
std::mutex _log_mutex;
}  // namespace

void printLog() {
  std::lock_guard<std::mutex> lock(_log_mutex);
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Log Information:\n"
            << "  Fractal time: " << _log_data.fractal_time_ms << " ms\n"
            << "  Display time: " << _log_data.display_time_ms << " ms\n"
            << "  Zoom level:   " << _log_data.zoom << "\n"
            << "  FPS:          " << _log_data.fps << "\n";

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

void set_zoom(double zoom_level) {
  std::lock_guard<std::mutex> lock(_log_mutex);
  _log_data.zoom = zoom_level;
}

}  // namespace LogInfo