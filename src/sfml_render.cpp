#include "sfml_mandelbrot.hpp"
#include "window_utils.h"
#include "global_config.hpp"

#include <iostream>
#include <thread>

void render_thread(sf::Window* window) {
  window->setActive(true);

  while (window->isOpen()) {
    window->display();
  }
}
