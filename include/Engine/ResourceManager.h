//
// Created by Bekir Gulestan on 1/12/25.
//

#ifndef RAY_GAME_RESOURCEMANAGER_H
#define RAY_GAME_RESOURCEMANAGER_H

#include "Engine/ShaderProgram.h"
#include "Engine/Texture.h"
#include "Engine/Font.h"

#include <memory>
#include <map>


class ResourceManager {

public:
    static std::shared_ptr<ShaderProgram> LoadShader(const char *vertexPath, const char *fragmentPath);

    static std::shared_ptr<Texture> LoadTexture(const std::string &path);

    static std::shared_ptr<Font> LoadFont(const std::string &path, int fontSize);


private:

    static std::map<std::string, std::shared_ptr<Texture>> m_Textures;

    // map of shader sources
    static std::map<std::string, std::shared_ptr<ShaderSource>> m_Shaders;

    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<ShaderProgram>> m_Programs;


    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<Font>> m_Fonts;
};

#endif //RAY_GAME_RESOURCEMANAGER_H
