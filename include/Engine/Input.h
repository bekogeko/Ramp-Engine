//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_INPUT_H
#define RAY_GAME_INPUT_H

#include "Window.h"
#include "HighRenderer.h"
#include "glm/vec2.hpp"
#include <iostream>


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

    static glm::vec2 getMousePosition() {
        if (m_window) {
            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);

            //TODO
            // get real width and height through api
            int width = 640;
            int height = 480;

//            std::cout << xpos << "," << ypos << std::endl;

            // Convert screen coordinates to normalized device coordinates (NDC)
            float xNDC = (2.0f * xpos) / width - 1.0f;
            float yNDC = 1.0f - (2.0f * ypos) / height;


            // Convert NDC to world coordinates (assuming an orthographic projection for simplicity)
            glm::vec2 worldPos = glm::vec2(xNDC, yNDC);
            worldPos += HighRenderer::getCamera().position;

            return worldPos;

        }
        return {0.0f, 0.0f};
    }

    static bool getMouseButton(int button) {
        if (m_window) {
            return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
        }
        return false;
    }


private:
    static GLFWwindow *m_window;
};

#endif //RAY_GAME_INPUT_H
