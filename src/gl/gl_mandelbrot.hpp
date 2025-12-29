#pragma once

// #include <SFML/Graphics.hpp>
// #include <SFML/Window.hpp>

#include "glfw/glfw_utils.hpp"
#include "core/window_dim.hpp"

void handle_event(GLFWwindow* const window);
void handle_render(GLFWwindow* const window, WindowDim<double>* frac);

/*
void sfml_handle(WindowDim<unsigned int> window_size, WindowDim<double> fract);
void handle_event(sf::RenderWindow* const window);
void render_handle(sf::RenderWindow* window, WindowDim<double>* frac);
void julia_handle();
*/