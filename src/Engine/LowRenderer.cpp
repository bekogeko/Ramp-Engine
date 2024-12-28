//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/LowRenderer.h"

Shader LowRenderer::CreateShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    Shader shader(vertexShaderSource, fragmentShaderSource);

    shader.CompileShader();

    return shader;
}
