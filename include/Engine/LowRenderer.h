//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_LOWRENDERER_H
#define RAY_GAME_LOWRENDERER_H


#include "Shader.h"

class LowRenderer {

public:
    static Shader CreateShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);
};


#endif //RAY_GAME_LOWRENDERER_H
