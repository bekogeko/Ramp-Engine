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

struct ParsedObject {
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> texCoords;
    bool isTextured = false;
};

class ResourceManager {

public:
    static std::shared_ptr<ShaderProgram> LoadShader(const char *vertexPath, const char *fragmentPath);

    static std::shared_ptr<Texture> LoadTexture(const std::string &path);

    static std::shared_ptr<Texture>
    LoadTextureFromBytes(const std::string &cacheId, const unsigned char *data, int w, int h);

    static ParsedObject &LoadObject(const std::string &path);

    static std::shared_ptr<Font> LoadFont(const std::string &path, int fontSize);

    static std::shared_ptr<Font> LoadFontById(unsigned short fontId);

    static unsigned char GetFontId(const std::string &cacheId, int fontSize);

    static void Destroy();

private:

    static ParsedObject ParseObjectFromFile(const std::string &pathName);


    static std::map<std::string, std::shared_ptr<Texture>> m_Textures;

    // map of shader sources
    static std::map<std::string, std::shared_ptr<ShaderSource>> m_Shaders;

    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<ShaderProgram>> m_Programs;

    // map of Objects
    static std::map<std::string, std::shared_ptr<ParsedObject>> m_Objects;

    // map of shaderPrograms and their sources
    static std::map<std::string, std::shared_ptr<Font>> m_Fonts;

    static std::map<unsigned short, std::string> m_FontIdToCacheId;

    // fontId gen
    static std::hash<std::string> hash;
};

#endif //RAY_GAME_RESOURCEMANAGER_H
