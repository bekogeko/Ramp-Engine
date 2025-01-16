//
// Created by Bekir Gulestan on 1/12/25.
//

#include "Engine/ResourceManager.h"

std::map<std::string, std::shared_ptr<ShaderSource>> ResourceManager::m_Shaders;

std::map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;

std::map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::m_Programs;

std::map<std::string, std::shared_ptr<Font>> ResourceManager::m_Fonts;


std::shared_ptr<ShaderProgram> ResourceManager::LoadShader(const char *vertexPath, const char *fragmentPath) {


    //create id from paths
    // first vertex shader path
    std::string vertId = vertexPath;
    std::string fragId = fragmentPath;
    // check if program exists
    if (m_Programs.find(vertId + fragId) != m_Programs.end()) {
        return m_Programs[vertId + fragId];
    }


    std::shared_ptr<ShaderSource> vertex;
    // check if
    if (m_Shaders.find(vertId) != m_Shaders.end()) {
        vertex = m_Shaders[vertId];
    } else {
        vertex = std::make_shared<ShaderSource>(vertId, ShaderSource::Type::VERTEX);
        m_Shaders[vertId] = vertex;
    }


    std::shared_ptr<ShaderSource> fragment;
    // check if
    if (m_Shaders.find(fragId) != m_Shaders.end()) {
        fragment = m_Shaders[fragId];
    } else {
        fragment = std::make_shared<ShaderSource>(fragId, ShaderSource::Type::FRAGMENT);
        m_Shaders[fragId] = fragment;
    }

    auto program = std::make_shared<ShaderProgram>(vertex, fragment);
    m_Programs[vertId + fragId] = program;

    return program;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string &path) {
    // check if string exists in the map
    if (m_Textures.find(path) != m_Textures.end()) {
        return m_Textures[path];
    }

    auto texture = std::make_shared<Texture>(path);
    m_Textures[path] = texture;
    return texture;

}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string &path, int fontSize) {
    if (m_Fonts.find(path) != m_Fonts.end()) {
        return m_Fonts[path];
    }

    auto font = std::make_shared<Font>(path, fontSize);

    m_Fonts[path] = font;
    return font;
}
