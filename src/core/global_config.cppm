module;

#include "global_config.hpp"

export module core.global_config;

export namespace GlobalConfig {
using ::GlobalConfig::change_iter_max;
using ::GlobalConfig::change_zoom;
using ::GlobalConfig::get_center;
using ::GlobalConfig::get_color_scheme;
using ::GlobalConfig::get_fractal_dim;
using ::GlobalConfig::get_iter_max;
using ::GlobalConfig::get_zoom_level;
using ::GlobalConfig::is_window_resized;
using ::GlobalConfig::move_center;
using ::GlobalConfig::need_redraw;
using ::GlobalConfig::parse_from_argv;
using ::GlobalConfig::print_configuration;
using ::GlobalConfig::set_fractal_dim;
using ::GlobalConfig::set_window_resized;
using ::GlobalConfig::switch_color_scheme;
using ::GlobalConfig::wait_to_draw;
}  // namespace GlobalConfig