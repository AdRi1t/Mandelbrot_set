#if USE_HIP

#include <hip/hip_runtime.h>

using Complex = hipDoubleComplex;

void plot(const WindowDim<uint32_t>& screen, uint32_t* d_escape_map, uint32_t iter_max,
          sf::Sprite& fractal_sprite, sf::Texture, sf::Color* pixelArray,
          bool smooth_color) {
  sf::Color* d_pixelArray;
  size_t alloc_size = screen.size() * sizeof(uint32_t);
  HIP_CHECK(hipSetDevice(0));
  HIP_CHECK(hipMalloc((void**)&d_pixelArray, alloc_size))
}

#endif
