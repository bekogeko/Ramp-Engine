//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_SHADER_H
#define RAY_GAME_SHADER_H

#include <string>

struct ShaderCompilationStatus {
    bool isCompiled;
    bool isErrored;
    bool isLinked;
    bool isVertexShader;
    bool isFragmentShader;
};

class Shader {

private:
    unsigned int programID;
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;

    bool isCompiled;
    bool isErrored;
    // if errored is true, then we can check if it is vertex or fragment shader
    bool isFragmentShader;
    bool isVertexShader;
    bool isLinked;

    // read shader source code from file
    static std::string ReadShaderSource(const std::string &shaderPath);


public:
    void SetVertexShader(const char *vertexShaderSource);

    void SetFragmentShader(const char *fragmentShaderSource);

    void CompileShader();

    ShaderCompilationStatus GetCompilationStatus() const;

    void Delete() const;


    [[nodiscard]] unsigned int GetID() const {
        return programID;
    }

    void Bind();

    void Unbind();

    Shader();

    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

    ~Shader();

};


#endif //RAY_GAME_SHADER_H
