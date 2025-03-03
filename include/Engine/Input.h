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

    static glm::vec2 getMouseWorldPosition() {
        if (!m_window)
            return {0.0f, 0.0f};
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        // this will give us the screen coordinates
        // (0,0) is the top left corner
        // we need to convert this to world coordinates
        // we will assume that the screen is 640x480
        // and the camera is at (0,0) and has a size of 4x3

        // get real width and height through api
        int width = Window::getWidth();
        int height = Window::getHeight();

//            std::cout << xpos << "," << ypos << std::endl;

        // Convert screen coordinates to normalized device coordinates (NDC)

        float x = (float) (2 * xpos / width) - 1;
        float y = (float) (-2 * ypos / height) + 1;

        x *= HighRenderer::getCamera().getSize().x;
        y *= HighRenderer::getCamera().getSize().y;

        // Convert NDC to world coordinates (assuming an orthographic projection for simplicity)
        glm::vec2 worldPos = glm::vec2(x, y);

        worldPos.x += HighRenderer::getCamera().position.x * HighRenderer::getCamera().getSize().x;

        worldPos.y += HighRenderer::getCamera().position.y * HighRenderer::getCamera().getSize().y;

        worldPos *= HighRenderer::getCamera().zoom;

        return worldPos;
    }

    static glm::vec2 getMousePosition() {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);

        return {xpos, ypos};
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
