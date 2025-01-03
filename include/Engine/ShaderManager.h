//
// Created by Bekir Gulestan on 1/3/25.
//

#ifndef RAY_GAME_SHADERMANAGER_H
#define RAY_GAME_SHADERMANAGER_H

#include "ShaderSource.h"
#include "ShaderProgram.h"

#include <map>

class ShaderManager {

public:
    static std::shared_ptr<ShaderProgram> LoadShader(const char *vertexPath, const char *fragmentPath);

    // TODO: automatic maybe ?
    static void Clear();

private:

    // map of shader sources
    static std::map<std::string, std::shared_ptr<ShaderSource>> m_Shaders;

    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<ShaderProgram>> m_Programs;


};


#endif //RAY_GAME_SHADERMANAGER_H
