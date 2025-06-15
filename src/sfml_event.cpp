
#include "sfml_mandelbrot.hpp"
#include "window_utils.h"
#include "global_config.hpp"

#include <iostream>
#include <thread>

void handle_event(sf::Window* const window) {
  sf::Event event;
  window->setActive(false);

  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window->close();
      return;
    }

    if (event.type == sf::Event::KeyPressed) {
      const double moveSpeed = 1.0 * GlobalConfig::get_zoom_level();

      if (event.key.code == sf::Keyboard::Left) {
        GlobalConfig::move_center(-moveSpeed, 0);
      }
      if (event.key.code == sf::Keyboard::Right) {
        GlobalConfig::move_center(moveSpeed, 0);
      }
      if (event.key.code == sf::Keyboard::Up) {
        GlobalConfig::move_center(0, moveSpeed);
      }
      if (event.key.code == sf::Keyboard::Down) {
        GlobalConfig::move_center(0, -moveSpeed);
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          GlobalConfig::change_zoom(event.mouseWheelScroll.delta);
        }
      }
    }
  }
}
