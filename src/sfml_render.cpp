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

void makeTexture(WindowDim<uint32_t> &screen, sf::Texture &texture) {
  bool success = texture.create(screen.width(), screen.height());
  if (!success) {
    std::cerr << "Error " << __FILE__ << ":" << __LINE__ << std::endl;
  }
  return;
}

void plot(WindowDim<uint32_t> &screen, uint32_t *escape_step, uint32_t iter_max,
          sf::Sprite &fractal_sprite, sf::Texture &texture, sf::Color* pixelArray, bool smooth_color = true) {
  size_t k              = 0;
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
  WindowUtils::zoom(center_x, center_y, GlobalConfig::get_zoom_level(), *fract);

  uint32_t *escape_step = new uint32_t[screen.size()];
  sf::Color *pixelArray = new sf::Color[screen.size()];

  sf::Sprite fractal_sprite;
  sf::Texture fractal_texture;
  while (renderWindow->isOpen()) {
    auto [x ,y] = GlobalConfig::get_center();
    auto zoom = GlobalConfig::get_zoom_level();
    renderWindow->clear(sf::Color::Black);
    mandelbrot(screen, *fract, escape_step);
    plot(screen, escape_step, GlobalConfig::get_iter_max(), fractal_sprite,
         fractal_texture, pixelArray);
    renderWindow->draw(fractal_sprite);
    WindowUtils::zoom(x, y, zoom, *fract);
    renderWindow->display();
  }
  delete[] escape_step;
  delete[] pixelArray;
}
