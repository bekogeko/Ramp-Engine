//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/Shader.h"
#include "glad/glad.h"
#include <iostream>


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
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        isCompiled = false;
        isErrored = true;
        isVertexShader= true;
    }
    else {
        // success
        // no error in vertex shader
        isVertexShader = false;

        // no error in vertex and fragment shader
        if(!isVertexShader && !isFragmentShader) {
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
        }
    }
}

void Shader::CompileShader() {


    if(!isCompiled){
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
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::Use() {
    if(!isCompiled){
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

void Shader::Delete() const {
    glDeleteProgram(programID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}


