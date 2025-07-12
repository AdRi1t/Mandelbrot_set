#include "sfml_mandelbrot.hpp"

void julia_handle() {
  sf::RenderWindow window(sf::VideoMode(600, 600), "Julia Set - Axes");

  sf::Color backgroundColor = sf::Color::Black;
  sf::Color axisColor = sf::Color::White;

  sf::VertexArray xAxis(sf::Lines, 2);
  sf::VertexArray yAxis(sf::Lines, 2);

  xAxis[0].position = sf::Vector2f(0, 300);
  xAxis[0].color = axisColor;
  xAxis[1].position = sf::Vector2f(600, 300);
  xAxis[1].color = axisColor;

  yAxis[0].position = sf::Vector2f(300, 0);
  yAxis[0].color = axisColor;
  yAxis[1].position = sf::Vector2f(300, 600);
  yAxis[1].color = axisColor;

  sf::CircleShape origin_point(11);
  origin_point.setFillColor(sf::Color::Red);
  origin_point.setOrigin(6, 6);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        origin_point.setPosition(static_cast<float>(mousePosition.x),
                                 static_cast<float>(mousePosition.y));
      }
    }
    window.clear(backgroundColor);
    window.draw(xAxis);
    window.draw(yAxis);
    window.draw(origin_point);
    window.display();
  }
}