//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_LOWRENDERER_H
#define RAY_GAME_LOWRENDERER_H


#include "Shader.h"

class LowRenderer {

public:
    // from source code
    static Shader CreateShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);

    // from file path
    static Shader CreateShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

};


#endif //RAY_GAME_LOWRENDERER_H
