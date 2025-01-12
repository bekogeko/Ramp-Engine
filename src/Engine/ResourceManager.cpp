//
// Created by Bekir Gulestan on 1/12/25.
//

#include "Engine/ResourceManager.h"

std::map<std::string, std::shared_ptr<ShaderSource>> ResourceManager::m_Shaders;

std::map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;

std::map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::m_Programs;


std::shared_ptr<ShaderProgram> ResourceManager::LoadShader(const char *vertexPath, const char *fragmentPath) {

    //create id from paths
    // first vertex shader path
    std::string vertId = vertexPath;
    std::string fragId = fragmentPath;

    // check if vertShader exists
    // then do not create it again
    auto vertShader = m_Shaders.find(vertId);

    // if not found create it
    if (vertShader == m_Shaders.end()) {
        m_Shaders[vertId] = std::make_shared<ShaderSource>(vertexPath, ShaderSource::Type::VERTEX);
    }

    // check if fragShader exists
    // then do not create it again
    auto fragShader = m_Shaders.find(fragId);

    // if not found create it
    if (fragShader == m_Shaders.end()) {
        m_Shaders[fragId] = std::make_shared<ShaderSource>(fragmentPath, ShaderSource::Type::FRAGMENT);
    }

    // create program
    auto shader = std::make_shared<ShaderProgram>(m_Shaders[vertId], m_Shaders[fragId]);

    // add it to m_Programs
    m_Programs[vertId + fragId] = shader;

    return shader;
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
