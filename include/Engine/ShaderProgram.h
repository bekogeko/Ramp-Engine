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

    explicit ShaderProgram(const std::shared_ptr<ShaderSource> &vertSource,
                           const std::shared_ptr<ShaderSource> &fragSource);

    ~ShaderProgram();

    void Bind() const;

    static void Unbind();

    [[nodiscard]] int getAttribLocation(const std::string &name) const {
        return glGetAttribLocation(programID, name.c_str());
    }


    void SetUniform4f(const std::string &name, float x, float y, float z, float w) const;

    void SetUniform3f(const std::string &name, float x, float y, float z) const;

    void SetUniform2f(const std::string &name, float x, float y) const;

    void SetUniform1i(const std::string &name, int x) const;

    void SetUniformMat4(const std::string &name, const float *matrix) const;

};

#endif //RAY_GAME_SHADERPROGRAM_H
