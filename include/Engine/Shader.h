//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_SHADER_H
#define RAY_GAME_SHADER_H


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


public:
    void SetVertexShader(const char *vertexShaderSource);
    void SetFragmentShader(const char *fragmentShaderSource);

    void CompileShader();
    int GetCompilationStatus();
    void Delete() const;



    [[nodiscard]] unsigned int GetID() const {
        return programID;
    }

    void Use();
    Shader();
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);


};


#endif //RAY_GAME_SHADER_H
