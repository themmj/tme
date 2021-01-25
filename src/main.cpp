#include <iostream>
#include "core/log.hpp"

#include <GLFW/glfw3.h>

int main() {
    tme::core::Log::init();
    TME_INFO("starting application");

    GLFWwindow* window;

    if (!glfwInit()) {
        TME_CRITICAL("failed to initialize glfw");
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    if (!window) {
        TME_CRITICAL("failed to create window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    TME_INFO("terminated glfw successfully");

    return 0;
}
