//
// Created by Bekir Gulestan on 1/3/25.
//
#include "Engine/ShaderProgram.h"
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>


void ShaderProgram::SetUniform3f(const std::string &name, float x, float y, float z) {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
    } else {
        glUniform3f(location, x, y, z);
    }
}

void ShaderProgram::SetUniform2f(const std::string &name, float x, float y) {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
    } else {
        glUniform2f(location, x, y);
    }
}

void ShaderProgram::SetUniformMat4(const std::string &name, const float *matrix) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }
}

ShaderProgram::ShaderProgram(std::shared_ptr<ShaderSource> vertSource, std::shared_ptr<ShaderSource> fragSource) {

    programID = glCreateProgram();

    glAttachShader(programID, vertSource->m_shaderID);
    glAttachShader(programID, fragSource->m_shaderID);
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    std::cout << "Shader created with id (prog,vert,frag) " << programID << "," << vertSource->m_shaderID << ","
              << fragSource->m_shaderID << std::endl;


}

void ShaderProgram::Bind() const {
    glUseProgram(programID);
}

void ShaderProgram::Unbind() const {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
    // glDeleteShader(vertexShaderID);
    // glDeleteShader(fragmentShaderID);
    // FIXME: delete shaders
    // its owned by the ShaderManager now
    // we should decrease the ref count
    // and delete if it reaches 0


}
