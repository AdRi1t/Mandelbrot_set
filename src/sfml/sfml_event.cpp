#include "sfml_mandelbrot.hpp"

#include <cstdint>
#include <iostream>
#include <thread>

#include "core/window_utils.hpp"
#include "core/global_config.hpp"
#include "fractal/mandelbrot.hpp"
#include "render/save_image.hpp"

void handle_event(sf::RenderWindow *const window) {
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
        const double moveSpeed = 0.15 / GlobalConfig::get_zoom_level();

        if (event.key.code == sf::Keyboard::Left) {
          GlobalConfig::move_center(-moveSpeed, 0);
        }
        if (event.key.code == sf::Keyboard::Right) {
          GlobalConfig::move_center(moveSpeed, 0);
        }
        if (event.key.code == sf::Keyboard::Up) {
          GlobalConfig::move_center(0, -moveSpeed);
        }
        if (event.key.code == sf::Keyboard::Tab) {
          GlobalConfig::switch_color_scheme();
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
          auto [c_x, c_y]   = GlobalConfig::get_center();
          double zoom_level = GlobalConfig::get_zoom_level();

          std::string position = pos_to_string(c_x, c_y, zoom_level);
          std::string filename = now_to_string() + position + ".png";

          WindowDim<uint32_t> screen(0, 1920, 0, 1080);
          WindowDim<double> fract(0, 1920, 0, 1080);
          WindowUtils::zoom(c_x, c_y, zoom_level, &fract);

          uint32_t *escape_step = new uint32_t[screen.size()];
          sf::Color *pixelArray = new sf::Color[screen.size()];

          mandelbrot(screen, fract, escape_step, GlobalConfig::get_iter_max());
          save_image(screen, escape_step, GlobalConfig::get_iter_max(), filename.c_str());
          delete[] escape_step;
          delete[] pixelArray;
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
          sf::Vector2u windowSize    = window->getSize();
          sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
          sf::Vector2f mousePos(static_cast<float>(mousePosition.x),
                                static_cast<float>(mousePosition.y));

          auto [frac_width, frac_height] = GlobalConfig::get_fractal_dim();
          double zoomFactor = event.mouseWheelScroll.delta > 0 ? 1.2 : 1 / 1.2;

          double ratio_x = frac_width / static_cast<double>(windowSize.x);
          double ratio_y = frac_height / static_cast<double>(windowSize.y);

          double delta_x =
              0.5 * (mousePos.x - static_cast<float>(windowSize.x) * 0.5) * ratio_x;
          double delta_y =
              0.5 * (mousePos.y - static_cast<float>(windowSize.y) * 0.5) * ratio_y;

          GlobalConfig::change_zoom(zoomFactor);
          GlobalConfig::move_center(delta_x, delta_y);
          GlobalConfig::need_redraw();
        }
      }
    }
  }
}
