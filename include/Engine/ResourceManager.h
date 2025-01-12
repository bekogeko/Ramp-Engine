//
// Created by Bekir Gulestan on 1/12/25.
//

#ifndef RAY_GAME_RESOURCEMANAGER_H
#define RAY_GAME_RESOURCEMANAGER_H

#include "Engine/ShaderProgram.h"
#include "Engine/Texture.h"

#include <memory>
#include <map>


class ResourceManager {

public:
    static std::shared_ptr<ShaderProgram> LoadShader(const char *vertexPath, const char *fragmentPath);

    static std::shared_ptr<Texture> LoadTexture(const std::string &path);


private:

    static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
    
    // map of shader sources
    static std::map<std::string, std::shared_ptr<ShaderSource>> m_Shaders;

    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<ShaderProgram>> m_Programs;
};

#endif //RAY_GAME_RESOURCEMANAGER_H
