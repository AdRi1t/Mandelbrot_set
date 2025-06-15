#include "sfml_mandelbrot.hpp"
#include "mandelbrot.hpp"
#include "window_dim.hpp"

#include <iostream>
#include <thread>

void sfml_handle(WindowDim<unsigned int> window_size) {
  sf::ContextSettings settings;
  settings.depthBits         = 24;
  settings.stencilBits       = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion      = 3;
  settings.minorVersion      = 0;
  sf::Window window(sf::VideoMode({window_size.width(), window_size.height()}),
                    "Mandelbort", sf::Style::Default, settings);
  window.setActive(false);
  std::thread rendering_thread(&render_thread, &window);

  handle_event(&window);

  rendering_thread.join();
}

int main(int argc, char* argv[]) {
  GlobalConfig::parse_from_argv(argc, argv);
  GlobalConfig::print_configuration();

  // Define the size of the image
  WindowDim<uint32_t> scr(0, 1200, 0, 1200);
  // The domain in which we test for points
  WindowDim<double> fract(-2.2, 1.2, -1.7, 1.7);
  mandelbrot(scr, fract);
  sfml_handle(scr);

  return 0;
}
