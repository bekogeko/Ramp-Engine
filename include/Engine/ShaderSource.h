//
// Created by Bekir Gulestan on 1/3/25.
//

#ifndef RAY_GAME_SHADERSOURCE_H
#define RAY_GAME_SHADERSOURCE_H

#include "glad/glad.h"
#include <string>

class ShaderSource {
public:
    enum class Type {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER
    };

    ShaderSource(std::string shaderPath, Type type);

private:
    std::string m_source;
    Type m_type;
    unsigned int m_shaderID;

    friend class ShaderProgram;
};

#endif //RAY_GAME_SHADERSOURCE_H
