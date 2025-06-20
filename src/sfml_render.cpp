#include "sfml_mandelbrot.hpp"
#include "window_utils.h"
#include "global_config.hpp"
#include "mandelbrot.hpp"
#include "save_image.h"

#include <iostream>
#include <thread>

void updateTextureFromColorArray(sf::Texture &texture, const sf::Color *colorArray) {
  texture.update(reinterpret_cast<const sf::Uint8 *>(colorArray));
  return;
}

void makeTexture(const WindowDim<uint32_t> &screen, sf::Texture &texture) {
  bool success = texture.create(screen.width(), screen.height());
  if (!success) {
    std::cerr << "Error " << __FILE__ << ":" << __LINE__ << std::endl;
  }
  return;
}

void plot(const WindowDim<uint32_t> &screen, uint32_t *escape_step, uint32_t iter_max,
          sf::Sprite &fractal_sprite, sf::Texture &texture, sf::Color *pixelArray,
          bool smooth_color = true) {
  size_t k = 0;
  for (unsigned int y = 0; y < screen.height(); y++) {
    for (unsigned int x = 0; x < screen.width(); x++) {
      uint32_t n      = escape_step[k];
      auto [r, g, b]  = get_rgb_smooth(n, iter_max);
      pixelArray[k].r = r;
      pixelArray[k].g = g;
      pixelArray[k].b = b;
      pixelArray[k].a = 255;
      k++;
    }
  }

  makeTexture(screen, texture);
  updateTextureFromColorArray(texture, pixelArray);
  fractal_sprite.setTexture(texture);
  return;
}

void render_handle(sf::RenderWindow *renderWindow, WindowDim<double> *fract) {
  renderWindow->setActive(true);
  sf::Vector2u dim = renderWindow->getSize();
  WindowDim<uint32_t> screen(0, dim.x, 0, dim.y);

  auto [center_x, center_y] = GlobalConfig::get_center();
  WindowUtils::zoom(center_x, center_y, GlobalConfig::get_zoom_level(), fract);

  WindowUtils::adjust_ratio(&screen, fract);
  GlobalConfig::set_fractDim(fract->width(), fract->height());

  uint32_t *escape_step = new uint32_t[screen.size()];
  sf::Color *pixelArray = new sf::Color[screen.size()];

  sf::Sprite fractal_sprite;
  sf::Texture fractal_texture;

  while (renderWindow->isOpen()) {
    auto [x, y] = GlobalConfig::get_center();
    auto zoom   = GlobalConfig::get_zoom_level();
    renderWindow->clear(sf::Color::Black);

    sf::Vector2u current_dim = renderWindow->getSize();
    if (current_dim.x != dim.x || current_dim.y != dim.y) {
      dim    = current_dim;
      screen = WindowDim<uint32_t>(0, dim.x, 0, dim.y);

      delete[] escape_step;
      delete[] pixelArray;
      escape_step = new uint32_t[screen.size()];
      pixelArray  = new sf::Color[screen.size()];

      WindowUtils::adjust_ratio(&screen, fract);
      GlobalConfig::set_fractDim(fract->width(), fract->height());
    }

    WindowUtils::zoom(x, y, zoom, fract);
    GlobalConfig::set_fractDim(fract->width(), fract->height());

    mandelbrot(screen, *fract, escape_step);

    auto start = std::chrono::steady_clock::now();
    plot(screen, escape_step, GlobalConfig::get_iter_max(), fractal_sprite,
         fractal_texture, pixelArray);
    auto end = std::chrono::steady_clock::now();
    /*
     std::cout << "Time to sprite " << " = "
               << std::chrono::duration<double, std::milli>(end - start).count() << "
     [ms]"
               << std::endl;
 */
    renderWindow->draw(fractal_sprite);
    renderWindow->display();
  }

  delete[] escape_step;
  delete[] pixelArray;
}
