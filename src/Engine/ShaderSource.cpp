//
// Created by Bekir Gulestan on 1/3/25.
//

#include "Engine/ShaderSource.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>


ShaderSource::ShaderSource(std::string path, ShaderSource::Type type) {

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string shaderFullPath = std::filesystem::current_path().c_str();
    shaderFullPath += &RESOURCES_PATH[1];
    shaderFullPath += shaderPath;
ßß
#elif PRODUCTION_BUILD == 0
    std::string shaderFullPath = RESOURCES_PATH + path;
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

    m_source = shaderSource.c_str();

    // set type
    m_type = type;

    // create shader
    m_shaderID = glCreateShader(static_cast<GLenum>(m_type));

    const char *c_str = m_source.c_str();
    glShaderSource(m_shaderID, 1, &c_str, nullptr);
    glCompileShader(m_shaderID);


    /// error checking
    int success;
    char infoLog[512];
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(m_shaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

ShaderSource::~ShaderSource() {
    glDeleteShader(m_shaderID);

//    std::cout << "Shader deleted with id " << m_shaderID << std::endl;
}
