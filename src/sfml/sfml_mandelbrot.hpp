#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "core/window_dim.hpp"

void sfml_handle(WindowDim<unsigned int> window_size, WindowDim<double> fract);
void handle_event(sf::RenderWindow* const window);
void render_handle(sf::RenderWindow* window, WindowDim<double>* frac);
