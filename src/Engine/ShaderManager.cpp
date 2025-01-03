//
// Created by Bekir Gulestan on 1/3/25.
//

#include "Engine/ShaderManager.h"

std::map<std::string, std::shared_ptr<ShaderSource>> ShaderManager::m_Shaders;

std::shared_ptr<ShaderProgram> ShaderManager::LoadShader(const char *vertexPath, const char *fragmentPath) {


    // FIXME: this is not a good way to create id
    //  we should hash each file and create id from that
    //  m_shaders[vertPath] = vertexShader;
    //  m_shaders[fragPath] = fragmentShader;
    //  m_programs[vertPath + fragPath] = program;

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

    return shader;
}


void ShaderManager::Clear() {
    m_Shaders.clear();
}
