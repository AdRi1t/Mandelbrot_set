#include "global_config.hpp"

namespace GlobalConfig {
namespace {
ConfigData config_data;
std::mutex config_mutex;
}  // namespace

void parse_from_argv(int argc, char* argv[]) {
  std::lock_guard<std::mutex> lock(config_mutex);
  config_data.iter_max   = 250;
  config_data.zoom_level = 1.0;
  config_data.center_x   = 0.0;
  config_data.center_y   = 0.0;
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
      config_data.iter_max = std::stoi(argv[++i]);
    } else if (arg == "-C" && i + 2 < argc) {
      config_data.center_x = std::stod(argv[++i]);
      config_data.center_y = std::stod(argv[++i]);
    } else if (arg == "-Z" && i + 1 < argc) {
      config_data.zoom_level = std::stod(argv[++i]);
    }
  }
}

void change_iter_max(int delta_iter_max) {
  std::lock_guard<std::mutex> lock(config_mutex);
  if (config_data.iter_max + delta_iter_max > 0) {
    config_data.iter_max += delta_iter_max;
  }
}

void change_zoom(double zoom_factor) {
  std::lock_guard<std::mutex> lock(config_mutex);
  config_data.zoom_level *= zoom_factor;
}

void move_center(double dx, double dy) {
  std::lock_guard<std::mutex> lock(config_mutex);
  config_data.center_x += dx;
  config_data.center_y += dy;
}

// getters
uint32_t get_iter_max() { return config_data.iter_max; }

double get_zoom_level() { return config_data.zoom_level; }

std::pair<double, double> get_center() {
  return std::make_pair(config_data.center_x, config_data.center_y);
}

void print_configuration() {
  std::cout << "Current Configuration:\n"
            << "  Max iterations: " << config_data.iter_max << "\n"
            << "  Zoom level: " << config_data.zoom_level << "\n"
            << "  Center: (" << config_data.center_x << ", " << config_data.center_y
            << ")\n";
}

}  // namespace GlobalConfig