//
// Created by Bekir Gulestan on 1/3/25.
//

#ifndef RAY_GAME_SHADERPROGRAM_H
#define RAY_GAME_SHADERPROGRAM_H

#include <string>
#include "ShaderSource.h"

class ShaderProgram {
private:
    unsigned int programID = 0;

public:

    explicit ShaderProgram(std::shared_ptr<ShaderSource> vertSource, std::shared_ptr<ShaderSource> fragSource);

    ~ShaderProgram();

    void Bind() const;

    void Unbind() const;


    void SetUniform3f(const std::string &name, float x, float y, float z);

    void SetUniform2f(const std::string &name, float x, float y);

    void SetUniformMat4(const std::string &name, const float *matrix) const;

};

#endif //RAY_GAME_SHADERPROGRAM_H
