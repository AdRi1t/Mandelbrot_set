#include <cstdint>
#include <iostream>
#include <thread>

#include "glfw/glfw_utils.hpp"

#include <hip/hip_runtime.h>
#include <hip/hip_gl_interop.h>

#include "core/window_dim.hpp"
#include "core/window_utils.hpp"
#include "fractal/mandelbrot.hpp"
#include "sfml/sfml_mandelbrot.hpp"

#ifndef USE_HIP
import core.global_config;
#else
#include "core/global_config.hpp"
#endif

#define DEBUG(msg)                                    \
  {                                                   \
    std::cout << __LINE__ << ":" << msg << std::endl; \
  }

#define HIP_CHECK(condition)                                             \
  {                                                                      \
    hipError_t err = condition;                                          \
    if (err != hipSuccess) {                                             \
      std::cerr << __FILE__ << ":" << __LINE__ << "\t"                   \
                << "HIP error: " << hipGetErrorString(err) << std::endl; \
    }                                                                    \
  }

void sfml_handle(WindowDim<unsigned int> window_size, WindowDim<double> fract) {
  /*sf::ContextSettings settings;
  settings.depthBits         = 16;
  settings.stencilBits       = 8;
  settings.antialiasingLevel = 0;
  settings.majorVersion      = 4;
  settings.minorVersion      = 5;

  sf::RenderWindow window(sf::VideoMode({window_size.width(), window_size.height()}),
                          "Mandelbrot", sf::Style::Default, settings);
  */

  GLFWUtils::initialize();
  GLFWwindow* const window =
      GLFWUtils::create_window(window_size.width(), window_size.height());
  GLFWUtils::print_configuration();

  DEBUG("")
  // OpenGL experiment
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.f);
  DEBUG("")

  int device_count = 0;
  HIP_CHECK(hipGetDeviceCount(&device_count));
  DEBUG("")

  unsigned int gl_device_count = 0;
  int hip_devices[8];
  hipStream_t hip_stream;
  DEBUG("")
  HIP_CHECK(
      hipGLGetDevices(&gl_device_count, hip_devices, 1, hipGLDeviceListCurrentFrame));
  DEBUG("")

  HIP_CHECK(hipSetDevice(hip_devices[0]));
  HIP_CHECK(hipStreamCreate(&hip_stream));
  HIP_CHECK(hipDeviceSynchronize());
  DEBUG("")

  hipGraphicsResource_t resource;
  GLuint image = 0;
  GLuint buffers[1];
  GLuint target = 0;
  DEBUG("")

  glGenBuffers(std::size(buffers), buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

  const size_t buffer_size = window_size.width() * window_size.height() * sizeof(float);
  glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
  DEBUG("After glBufferData")

  HIP_CHECK(hipGraphicsGLRegisterBuffer(&resource, buffers[0],
                                        hipGraphicsRegisterFlagsWriteDiscard));

  DEBUG("")

  glViewport(0, 0, static_cast<GLsizei>(window_size.width()),
             static_cast<GLsizei>(window_size.height()));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const GLfloat ratio =
      static_cast<float>(window_size.width()) / static_cast<float>(window_size.height());
  glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

  /*
  window.setActive(false);

  // std::thread julia_thread(&julia_handle);
  std::thread rendering_thread(&render_handle, &window, &fract);

  handle_event(&window);

  rendering_thread.join();
  // julia_thread.join();
  */
  GLFWUtils::finalize();
}

int main(int argc, char* argv[]) {
  GlobalConfig::parse_from_argv(argc, argv);
  GlobalConfig::print_configuration();

  // Dimension of SFML window
  WindowDim<uint32_t> screen(0, 1200, 0, 1200);

  // WindowDim : where we focus in the fractal
  WindowDim<double> fract(-2.2, 1.2, -1.7, 1.7);

  DEBUG("")
  GlobalConfig::set_fractal_dim(fract.width(), fract.height());
  DEBUG("")
  sfml_handle(screen, fract);
  DEBUG("")

  return 0;
}
