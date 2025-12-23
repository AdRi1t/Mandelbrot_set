#include "glad.h"
#include <GLFW/glfw3.h>

#include <hip/hip_runtime.h>
#include <hip/hip_gl_interop.h>

namespace GLFWUtils {

GLFWwindow* create_window(const int initial_width, const int initial_height);
int initialize();
void finalize();
const char* get_glfw_platform_name();
void print_configuration();

}  // namespace GLFWUtils

void print_device_prop(int deviceIndex);
