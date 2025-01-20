//
// Created by Bekir Gulestan on 1/3/25.
//
#include "Engine/ShaderProgram.h"
#include <glad/glad.h>
#include <iostream>

void ShaderProgram::SetUniform3f(const std::string &name, float x, float y, float z) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
#ifdef GL_ERROR_SILENTa
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
#endif
    } else {
        glUniform3f(location, x, y, z);
    }
}

void ShaderProgram::SetUniform2f(const std::string &name, float x, float y) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
#ifdef GL_ERROR_SILENT
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
#endif
    } else {
        glUniform2f(location, x, y);
    }
}

void ShaderProgram::SetUniformMat4(const std::string &name, const float *matrix) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
#ifdef GL_ERROR_SILENT
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
#endif
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }
}

ShaderProgram::ShaderProgram(const std::shared_ptr<ShaderSource> &vertSource,
                             const std::shared_ptr<ShaderSource> &fragSource) {

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

void ShaderProgram::Unbind() {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);

    // ShaderSource owned by the ShaderManager now
    // we don't need to delete them

//    std::cout << "Shader Program deleted with id " << programID << std::endl;
}

void ShaderProgram::SetUniform4f(const std::string &name, float x, float y, float z, float w) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
#ifdef GL_ERROR_SILENT
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
#endif
    } else {
        glUniform4f(location, x, y, z, w);
    }
}


void ShaderProgram::SetUniform1i(const std::string &name, int x) const {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
#ifdef GL_ERROR_SILENT
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
#endif
    } else {
        glUniform1i(location, x);
    }
}