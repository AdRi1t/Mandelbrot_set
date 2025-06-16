#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "window_dim.hpp"

void sfml_handle(WindowDim<unsigned int> window_size);
void handle_event(sf::RenderWindow* const window);
void render_thread(sf::RenderWindow* window);
