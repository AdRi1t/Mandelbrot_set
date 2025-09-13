#include "sfml_mandelbrot.hpp"

#include <iostream>
#include <thread>

#include "core/window_utils.hpp"
#include "core/global_config.hpp"
#include "fractal/mandelbrot.hpp"
#include "render/save_image.hpp"

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

void set_texture(const WindowDim<uint32_t> &screen, uint32_t *escape_step,
                 uint32_t iter_max, sf::Texture &texture, sf::Color *pixelArray) {
  size_t k = 0;
  for (unsigned int y = 0; y < screen.height(); y++) {
    for (unsigned int x = 0; x < screen.width(); x++) {
      uint32_t n      = escape_step[k];
      auto [r, g, b]  = get_rgb_scheme_3(n, iter_max);
      pixelArray[k].r = r;
      pixelArray[k].g = g;
      pixelArray[k].b = b;
      pixelArray[k].a = 255;
      k++;
    }
  }

  updateTextureFromColorArray(texture, pixelArray);
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

  makeTexture(screen, fractal_texture);
  fractal_sprite.setTexture(fractal_texture);

  while (renderWindow->isOpen()) {
    GlobalConfig::wait_to_draw();
    renderWindow->clear(sf::Color::Black);

    if (GlobalConfig::is_window_resized()) {
      dim    = renderWindow->getSize();
      screen = WindowDim<uint32_t>(0, dim.x, 0, dim.y);

      delete[] escape_step;
      delete[] pixelArray;
      escape_step = new uint32_t[screen.size()];
      pixelArray  = new sf::Color[screen.size()];

      makeTexture(screen, fractal_texture);
      fractal_sprite.setTexture(fractal_texture);

      WindowUtils::adjust_ratio(&screen, fract);
      GlobalConfig::set_fractDim(fract->width(), fract->height());
      GlobalConfig::set_window_resized(false);
    }
    std::tie(center_x, center_y) = GlobalConfig::get_center();
    auto zoom                    = GlobalConfig::get_zoom_level();
    GlobalConfig::set_fractDim(fract->width(), fract->height());
    WindowUtils::zoom(center_x, center_y, zoom, fract);

    auto fractal_start = std::chrono::steady_clock::now();
    mandelbrot(screen, *fract, escape_step, GlobalConfig::get_iter_max());

    LogInfo::set_fractal_time(std::chrono::duration<float, std::milli>(
                                  std::chrono::steady_clock::now() - fractal_start)
                                  .count());

    auto display_start = std::chrono::steady_clock::now();
    set_texture(screen, escape_step, GlobalConfig::get_iter_max(), fractal_texture,
                pixelArray);

    renderWindow->draw(fractal_sprite);
    renderWindow->display();

    LogInfo::set_display_time_ms(std::chrono::duration<float, std::milli>(
                                     std::chrono::steady_clock::now() - display_start)
                                     .count());
    LogInfo::printLog();
  }

  delete[] escape_step;
  delete[] pixelArray;
}
