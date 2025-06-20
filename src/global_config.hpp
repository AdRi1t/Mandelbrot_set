#pragma once

#include <iostream>
#include <string>
#include <mutex>

namespace GlobalConfig {
namespace {
struct ConfigData {
  uint32_t iter_max;
  double zoom_level;
  double center_x;
  double center_y;
  double fract_width;
  double fract_height;
};

extern ConfigData _config_data;
extern std::mutex _config_mutex;
}  // namespace

void parse_from_argv(int argc, char* argv[]);
void change_iter_max(int delta_iter_max);
void change_zoom(double change_factor);
void set_fractDim(double fract_width, double fract_height);
uint32_t get_iter_max();
double get_zoom_level();
void move_center(double x, double y);
std::pair<double, double> get_center();
std::pair<double, double> get_fractDim();
void print_configuration();

}  // namespace GlobalConfig

namespace LogInfo {

namespace {
struct LogData {
  double fractal_time_ms;
  double display_time_ms;
  double zoom;
  double fps;
};

extern LogData _log_data;
extern std::mutex _log_mutex;
}  // namespace

void printLog();
void set_fractal_time(double time_ms);
void set_display_time_ms(double time_ms);
void set_zoom(double time_ms);

}  // namespace LogInfo