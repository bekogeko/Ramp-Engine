//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_INPUT_H
#define RAY_GAME_INPUT_H

#include "Window.h"


class Input {
public:
    static void setWindow(Window *win) {
        m_window = win->getRawWindow();
    }

    // Get Key pres
    static bool getKeyPress(int key) {
        if (m_window) {
            return glfwGetKey(m_window, key) == GLFW_PRESS;
        }
        return false;
    }


private:
    static GLFWwindow *m_window;
};

#endif //RAY_GAME_INPUT_H
