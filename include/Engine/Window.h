//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_WINDOW_H
#define RAY_GAME_WINDOW_H

#include "GLFW/glfw3.h"

class Window {
private:
    GLFWwindow* p_window;

public:
    Window(int width, int height, const char* title);


    [[nodiscard]] GLFWwindow *getRawWindow() const{
        return p_window;
    }

    bool shouldClose(){
        return glfwWindowShouldClose(p_window);
    }

    void swapBuffers(){
        glfwSwapBuffers(p_window);
    }

    void pollInputs(){
        glfwPollEvents();
    }

    ~Window(){
        glfwDestroyWindow(p_window);
        glfwTerminate();
    }
};


#endif //RAY_GAME_WINDOW_H
