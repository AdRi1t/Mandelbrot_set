cmake_minimum_required(VERSION 3.28.0)

project(mandelbrot LANGUAGES CXX VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_SEQUENTIAL "Enable sequential CPU implementation" ON)
option(ENABLE_OPENMP "Enable OpenMP acceleration" OFF)
option(ENABLE_HIP "Enable HIP GPU acceleration" OFF)
option(ENABLE_SANITIZERS "Enable Address and Memory Sanitizers" OFF)

if(ENABLE_OPENMP AND ENABLE_HIP)
  message(FATAL_ERROR "HIP and OPENMP can't be enable together")
endif()

if(ENABLE_HIP)
  list(APPEND CMAKE_PREFIX_PATH "/opt/rocm/hip" "/opt/rocm")
  find_package(HIP REQUIRED)
  if(HIP_FOUND)
    message(STATUS "Found HIP package")
    set(CMAKE_CXX_COMPILER ${HIPCC})
  endif()
  find_program(HIPCC hipcc)
  if(HIPCC)
    message(STATUS "Found hipcc: ${HIPCC}")
    set(CMAKE_CXX_COMPILER ${HIPCC})
  endif()
endif()

# Try sanitizers
if(ENABLE_SANITIZERS)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT ENABLE_HIP)
    message(STATUS "Enabling Address and Memory Sanitizers")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
  endif()
  if(ENABLE_HIP)
    add_compile_options(-fgpu-sanitize -Rpass-analysis=loop-vectorize
                        -Rpass-analysis=kernel-resource-usag -Rpass=loop-vectorize)
  endif()
endif()

# FREEIMAGE stuff
find_library(FREEIMAGE_LIBRARY freeimage)
find_path(FREEIMAGE_INCLUDE_DIR FreeImage.h)
message(STATUS "Found FreeImage library: ${FREEIMAGE_LIBRARY}")
message(STATUS "Found FreeImage headers: ${FREEIMAGE_INCLUDE_DIR}")

#SFML 2
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

set(MANDELBROT_SRC
  src/core/global_config.cpp
  src/core/window_utils.cpp
  src/sfml/sfml_event.cpp
  src/sfml/sfml_render.cpp
  src/save_image.cpp
  src/sfml_main.cpp
)

if(ENABLE_HIP)
  list(APPEND MANDELBROT_SRC src/fractal/mandel_hip.cpp)
else()
  list(APPEND MANDELBROT_SRC src/fractal/mandel_sequential.cpp)
endif()

add_compile_options(-Wall -Wextra)

add_executable(mandelbrot ${MANDELBROT_SRC})
target_link_libraries(mandelbrot PRIVATE ${FREEIMAGE_LIBRARY} sfml-graphics sfml-window sfml-system)

target_include_directories(mandelbrot PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(ENABLE_HIP)
  message(STATUS "HIP platform ${CMAKE_HIP_PLATFORM}")
  target_compile_definitions(mandelbrot PUBLIC USE_HIP)
  target_include_directories(mandelbrot PUBLIC ${HIP_INCLUDE_DIRS})
  target_link_libraries(mandelbrot PRIVATE ${HIP_LIBRARIES})
endif()
if(ENABLE_SEQUENTIAL)
  target_compile_definitions(mandelbrot PUBLIC USE_SEQUENTIAL)
endif()

