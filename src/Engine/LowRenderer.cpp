//
// Created by Bekir Gulestan on 12/28/24.
//

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Engine/LowRenderer.h"

Shader LowRenderer::CreateShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    Shader shader(vertexShaderSource, fragmentShaderSource);

    shader.CompileShader();

    return shader;
}

Shader LowRenderer::CreateShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {

#if PRODUCTION_BUILD == 1


    // solve for relative path
    std::string vertexShaderFullPath = std::filesystem::current_path().c_str();
    vertexShaderFullPath += &RESOURCES_PATH[1];
    vertexShaderFullPath += vertexShaderPath;

    std::string fragmentShaderFullPath = std::filesystem::current_path().c_str();
    fragmentShaderFullPath += &RESOURCES_PATH[1];
    fragmentShaderFullPath += fragmentShaderPath;


#elif PRODUCTION_BUILD == 0
    std::string vertexShaderFullPath = RESOURCES_PATH + vertexShaderPath;
    std::string fragmentShaderFullPath = RESOURCES_PATH + fragmentShaderPath;
#endif

    std::cerr << "Vertex Shader Path: " << vertexShaderFullPath << std::endl;
    std::cerr << "Fragment Shader Path: " << fragmentShaderFullPath << std::endl;




    // source code Variables
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    // read vertex shader source code
    std::ifstream vertexShaderFile;
    vertexShaderFile.open(vertexShaderFullPath.c_str());
    std::stringstream vertexShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    vertexShaderSource = vertexShaderStream.str();


    std::cout << "Vertex Shader Source: " << vertexShaderSource.c_str() << std::endl;

    // close file
    vertexShaderFile.close();

    // read fragment shader source code
    std::ifstream fragmentShaderFile;
    fragmentShaderFile.open(fragmentShaderFullPath.c_str());
    std::stringstream fragmentShaderStream;
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    fragmentShaderSource = fragmentShaderStream.str();

    std::cout << "Fragment Shader Source: " << fragmentShaderSource.c_str() << std::endl;


    // close file
    fragmentShaderFile.close();


    Shader shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    shader.CompileShader();

    return shader;
}
