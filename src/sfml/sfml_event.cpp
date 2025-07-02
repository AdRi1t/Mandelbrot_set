#include "sfml_mandelbrot.hpp"

#include <cstdint>
#include <iostream>
#include <thread>

#include "render/save_image.hpp"
#include "core/window_utils.hpp"
#include "core/global_config.hpp"

void handle_event(sf::RenderWindow* const window) {
  sf::Event event;
  window->setActive(false);
  while (window->isOpen()) {
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        GlobalConfig::need_redraw();
        window->close();
        // Avoid deadlock
        return;
      }

      if (event.type == sf::Event::KeyPressed) {
        const double moveSpeed = 0.15 * (1 / GlobalConfig::get_zoom_level());

        if (event.key.code == sf::Keyboard::Left) {
          GlobalConfig::move_center(-moveSpeed, 0);
        }
        if (event.key.code == sf::Keyboard::Right) {
          GlobalConfig::move_center(moveSpeed, 0);
        }
        if (event.key.code == sf::Keyboard::Up) {
          GlobalConfig::move_center(0, -moveSpeed);
        }
        if (event.key.code == sf::Keyboard::Down) {
          GlobalConfig::move_center(0, moveSpeed);
        }
        if (event.key.code == sf::Keyboard::A) {
          GlobalConfig::change_iter_max(2);
        }
        if (event.key.code == sf::Keyboard::E) {
          GlobalConfig::change_iter_max(-2);
        }
        if (event.key.code == sf::Keyboard::P) {
          WindowDim<uint32_t> screenDim(0, window->getSize().x, 0, window->getSize().y);
          std::string timestamp = now_to_string();
          std::string filename = "mandelbrot_" + timestamp + ".png";

          sf::Vector2u dim = window->getSize();
          WindowDim<uint32_t> screen(0, dim.x, 0, dim.y);
        }
        GlobalConfig::need_redraw();
      }

      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea({0.f, 0.f},
                                  sf::Vector2f(event.size.width, event.size.height));
        window->setView(sf::View(visibleArea));
        GlobalConfig::set_window_resized(true);
        GlobalConfig::need_redraw();
      }

      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          sf::Vector2u windowSize = window->getSize();
          sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
          sf::Vector2f mousePos(static_cast<float>(mousePosition.x),
                                static_cast<float>(mousePosition.y));

          auto [frac_width, frac_height] = GlobalConfig::get_fractDim();
          float zoomFactor = event.mouseWheelScroll.delta > 0 ? 1.2 : 1 / 1.2;
          GlobalConfig::change_zoom(zoomFactor);

          sf::Vector2f ratio(
              static_cast<float>(frac_width) / static_cast<float>(windowSize.x),
              static_cast<float>(frac_height) / static_cast<float>(windowSize.y));

          double delta_x =
              0.5 * (mousePos.x - static_cast<float>(windowSize.x) / 2) * ratio.x;
          double delta_y =
              0.5 * (mousePos.y - static_cast<float>(windowSize.y) / 2) * ratio.y;

          GlobalConfig::move_center(delta_x, delta_y);
          GlobalConfig::need_redraw();
        }
      }
    }
  }
}
