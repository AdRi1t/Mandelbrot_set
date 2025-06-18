#include "global_config.hpp"
#include "sfml_mandelbrot.hpp"
#include "mandelbrot.hpp"
#include "window_dim.hpp"
#include "window_utils.h"


#include <iostream>
#include <thread>

void sfml_handle(WindowDim<unsigned int> window_size, WindowDim<double> fract) {
  sf::ContextSettings settings;
  settings.depthBits         = 24;
  settings.stencilBits       = 8;
  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode({window_size.width(), window_size.height()}),
                    "Mandelbort", sf::Style::Default, settings);
  
  window.setActive(false);
  std::thread rendering_thread(&render_handle, &window, &fract);

  handle_event(&window);

  rendering_thread.join();
}

int main(int argc, char* argv[]) {
  GlobalConfig::parse_from_argv(argc, argv);
  GlobalConfig::print_configuration();

  // Define the size of the image
  WindowDim<uint32_t> screen(0, 1200, 0, 1200);
  // The domain in which we test for points
  WindowDim<double> fract(-2.2, 1.2, -1.7, 1.7);
  sfml_handle(screen, fract);

  return 0;
}
