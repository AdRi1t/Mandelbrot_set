#include "sfml_mandelbrot.hpp"
#include "window_utils.h"
#include "global_config.hpp"

#include <iostream>
#include <thread>

sf::Texture createTextureFromColorArray(sf::Color* colorArray, unsigned int width, unsigned int height) {
    sf::Texture texture;
    texture.create(width, height);
    texture.update(reinterpret_cast<sf::Uint8*>(colorArray));
    return texture;
}

void render_thread(sf::RenderWindow * renderWindow) {
  renderWindow->setActive(true);

  while (renderWindow->isOpen()) {
    renderWindow->clear(sf::Color::Black);
    renderWindow->display();
  }
}



void plot(WindowDim<uint32_t> &screen, uint8_t *colors, uint32_t iter_max, const char *fname,
          bool smooth_color) {

  sf::Color* pixelArray = new sf::Color[screen.size()];
  sf::Texture texture = createTextureFromColorArray(pixelArray, screen.width(), screen.height());
  sf::Sprite sprite;
  sprite.setTexture(texture);
  delete[]  pixelArray;
}