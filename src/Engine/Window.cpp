//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/Window.h"
#include "Engine/Input.h"

#include "glad/glad.h"
#include "Engine/UILayer.h"
#include <iostream>

int Window::s_width = 0;
int Window::s_height = 0;


void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Window::s_width = width / 2;
    Window::s_height = height / 2;
    UILayer::UpdateWindowSize();
}

Window::Window(int width, int height, const char *title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    std::cout << "GLFW initialized successfully\n";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    p_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    s_height = height;
    s_width = width;

    if (!p_window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    std::cout << "GLFW window created successfully\n";

    glfwMakeContextCurrent(p_window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(p_window);
        glfwTerminate();
        return;
    }
    std::cout << "GLAD initialized successfully\n";

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);


    // Init Input system
    Input::setWindow(this);
}

