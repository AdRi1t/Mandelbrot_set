#pragma once

#define INLINE_FUNCTION inline

#ifdef USE_HIP
#define DEVICE_HOST_INLINE INLINE_FUNCTION __host__ __device__
#else
#define DEVICE_HOST_INLINE INLINE_FUNCTION
#endif

#define DEVICE_INLINE_FUNCTION __device__ __forceinline__

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

#define GL_CHECK()                                                      \
  {                                                                     \
    GLenum err = glGetError();                                          \
    if (err != GL_NO_ERROR) {                                           \
      std::cerr << __FILE__ << ":" << __LINE__ << "\t"                  \
                << "GL error: ";                                        \
      switch (err) {                                                    \
        case GL_INVALID_ENUM:                                           \
          std::cerr << "GL_INVALID_ENUM" << std::endl;                  \
          break;                                                        \
        case GL_INVALID_VALUE:                                          \
          std::cerr << "GL_INVALID_VALUE" << std::endl;                 \
          break;                                                        \
        case GL_INVALID_OPERATION:                                      \
          std::cerr << "GL_INVALID_OPERATION" << std::endl;             \
          break;                                                        \
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          \
          std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; \
          break;                                                        \
        case GL_OUT_OF_MEMORY:                                          \
          std::cerr << "GL_OUT_OF_MEMORY" << std::endl;                 \
          break;                                                        \
        case GL_STACK_UNDERFLOW:                                        \
          std::cerr << "GL_STACK_UNDERFLOW" << std::endl;               \
          break;                                                        \
        case GL_STACK_OVERFLOW:                                         \
          std::cerr << "GL_STACK_OVERFLOW" << std::endl;                \
          break;                                                        \
        default:                                                        \
          std::cerr << "Uknown Error" << std::endl;                     \
      }                                                                 \
    }                                                                   \
  }