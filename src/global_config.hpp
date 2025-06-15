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
};

extern ConfigData config_data;
extern std::mutex config_mutex;
}  // namespace

void parse_from_argv(int argc, char* argv[]);
void change_iter_max(int delta_iter_max);
uint32_t get_iter_max();
void change_zoom(double change_factor);
double get_zoom_level();
void move_center(double x, double y);
std::pair<double, double> get_center();
void print_configuration();

}  // namespace GlobalConfig
