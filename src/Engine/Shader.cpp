//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/Shader.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>


void Shader::SetVertexShader(const char *vertexShaderSource) {
    isLinked = false;
    isCompiled = false;
    isVertexShader = false;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        isCompiled = false;
        isErrored = true;
        isVertexShader = true;
    } else {
        // success
        // no error in vertex shader
        isVertexShader = false;

        // no error in vertex and fragment shader
        if (!isVertexShader && !isFragmentShader) {
            isCompiled = true;
        }
    }
}

void Shader::SetFragmentShader(const char *fragmentShaderSource) {
    isLinked = false;
    isCompiled = false;
    isFragmentShader = false;

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        isCompiled = false;
        isErrored = true;
        isFragmentShader = true;
    } else {
        // success
        // no error in fragment shader
        isFragmentShader = false;

        // no error in vertex and fragment shader
        if (!isVertexShader && !isFragmentShader) {
            isCompiled = true;
            isErrored = false;
        }
    }
}

void Shader::CompileShader() {


    if (!isCompiled) {
        return;
    }

    programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        isCompiled = false;
        isErrored = true;
        isLinked = true;
    } else {
        isCompiled = true;
        isLinked = false;

        // no error in vertex and fragment shader
        if (!isVertexShader && !isFragmentShader) {
            isCompiled = true;
            isErrored = false;
            isLinked = false;
        }


    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::Bind() {
    if (!isCompiled) {
        return;
    }
    // bind
    glUseProgram(programID);
}

Shader::Shader() {
    isCompiled = false;
    isErrored = false;
    isLinked = false;
    isVertexShader = false;
    isFragmentShader = false;

    programID = 0;
    vertexShaderID = 0;
    fragmentShaderID = 0;

}

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {

    SetVertexShader(vertexShaderSource);
    SetFragmentShader(fragmentShaderSource);
    CompileShader();
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {

    std::string vertexShaderSource = ReadShaderSource(vertexShaderPath);
    std::string fragmentShaderSource = ReadShaderSource(fragmentShaderPath);

    SetVertexShader(vertexShaderSource.c_str());
    SetFragmentShader(fragmentShaderSource.c_str());
    CompileShader();
}


void Shader::Delete() const {
    glDeleteProgram(programID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::Unbind() {
    glUseProgram(0);
}


ShaderCompilationStatus Shader::GetCompilationStatus() const {

    ShaderCompilationStatus status{};
    status.isCompiled = isCompiled;
    status.isErrored = isErrored;
    status.isLinked = isLinked;
    status.isVertexShader = isVertexShader;
    status.isFragmentShader = isFragmentShader;

    return status;
}

Shader::~Shader() {
    Delete();
}


std::string Shader::ReadShaderSource(const std::string &shaderPath) {

#if PRODUCTION_BUILD == 1


    // solve for relative path
    std::string shaderFullPath = std::filesystem::current_path().c_str();
    shaderFullPath += &RESOURCES_PATH[1];
    shaderFullPath += shaderPath;
ßß
#elif PRODUCTION_BUILD == 0
    std::string shaderFullPath = RESOURCES_PATH + shaderPath;
#endif

    std::string shaderSource;

    // read shader source code
    std::ifstream shaderFile;

    shaderFile.open(shaderFullPath.c_str());
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderSource = shaderStream.str();

    // close file
    shaderFile.close();

    return shaderSource;
}

void Shader::SetUniform3f(const std::string &name, float x, float y, float z) {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
    } else {
        glUniform3f(location, x, y, z);
    }

}

void Shader::SetUniform2f(const std::string &name, float x, float y) {
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";
    } else {
        glUniform2f(location, x, y);
    }
}