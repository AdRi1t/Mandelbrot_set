#include <cstdint>
#include <iostream>
#include <thread>

#include "core/window_dim.hpp"
#include "core/window_utils.hpp"
#include "fractal/mandelbrot.hpp"
#include "sfml/sfml_mandelbrot.hpp"

#ifndef USE_HIP
import core.global_config;
#else
#include "core/global_config.hpp"
#endif

void sfml_handle(WindowDim<unsigned int> window_size, WindowDim<double> fract) {
  sf::ContextSettings settings;
  settings.depthBits         = 24;
  settings.stencilBits       = 8;
  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode({window_size.width(), window_size.height()}),
                          "Mandelbrot", sf::Style::Default, settings);

  window.setActive(false);

  // std::thread julia_thread(&julia_handle);
  std::thread rendering_thread(&render_handle, &window, &fract);

  handle_event(&window);

  rendering_thread.join();
  // julia_thread.join();
}

int main(int argc, char* argv[]) {
  GlobalConfig::parse_from_argv(argc, argv);
  GlobalConfig::print_configuration();

  // Dimension of SFML window
  WindowDim<uint32_t> screen(0, 1200, 0, 1200);

  // WindowDim : where we focus in the fractal
  WindowDim<double> fract(-2.2, 1.2, -1.7, 1.7);

  GlobalConfig::set_fractal_dim(fract.width(), fract.height());

  sfml_handle(screen, fract);

  return 0;
}
