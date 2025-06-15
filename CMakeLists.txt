cmake_minimum_required(VERSION 3.28.0)

project(mandelbrot VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

option(ENABLE_HIP OFF)
option(ENABLE_SEQUENTIAL ON)
option(ENABLE_OPENMP OFF)


find_program(HIPCC hipcc)
if(HIPCC)
  message(STATUS "Found hipcc: ${HIPCC}")
  set(CMAKE_CXX_COMPILER ${HIPCC})
  add_compile_definitions(USE_HIP)
else()
  list(APPEND CMAKE_PREFIX_PATH "/opt/rocm/hip" "/opt/rocm")
  find_package(HIP REQUIRED)
  if(HIP_FOUND)
    message(STATUS "Found HIP package")
    message(STATUS "HIP platform ${MAKE_HIP_PLATFORM}")
    set(CMAKE_CXX_COMPILER ${HIPCC})
    add_compile_definitions(USE_HIP)
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
  src/global_config.cpp
  src/save_image.cpp
  src/window_utils.cpp
  src/mandel_sequential.cpp
  src/sfml_main.cpp
  src/sfml_render.cpp
  src/sfml_event.cpp
)

add_executable(mandelbrot ${MANDELBROT_SRC})
target_link_libraries(mandelbrot PRIVATE ${FREEIMAGE_LIBRARY} sfml-graphics sfml-window sfml-system)

# needed ?
target_include_directories(mandelbrot PUBLIC ${FREEIMAGE_INCLUDE_DIR})

