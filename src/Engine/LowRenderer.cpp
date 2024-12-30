//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Engine/LowRenderer.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

float LowRenderer::lastTime = 0.0f; // Initialization of lastTime
float LowRenderer::getDeltaTime() {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}
