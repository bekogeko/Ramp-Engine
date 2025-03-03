//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_WINDOW_H
#define RAY_GAME_WINDOW_H

#include "GLFW/glfw3.h"
#include <memory>

class Window {
private:
    GLFWwindow *p_window;
    static int s_width, s_height;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

public:
    Window(int width, int height, const char *title);


    [[nodiscard]] GLFWwindow *getRawWindow() const {
        return p_window;
    }

    bool shouldClose() {
        return glfwWindowShouldClose(p_window);
    }

    void swapBuffers() {
        glfwSwapBuffers(p_window);
    }

    void pollInputs() {
        glfwPollEvents();
    }


    static int getWidth() { return s_width; }

    static int getHeight() { return s_height; }

    ~Window() {
        glfwDestroyWindow(p_window);
        glfwTerminate();
    }
};


#endif //RAY_GAME_WINDOW_H
