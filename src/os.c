#include "os.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static GLFWwindow* window;

int os_createWindow() {
    if (glfwInit() == GLFW_FALSE) {
        return -1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "idetest", NULL, NULL);
    return window == NULL;
}

int os_windowCloseRequested() {
    return glfwWindowShouldClose(window);
}

void os_poll() {
    glfwPollEvents();
}

void os_destroyWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}