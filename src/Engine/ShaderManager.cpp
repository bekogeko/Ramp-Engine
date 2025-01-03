//
// Created by Bekir Gulestan on 1/3/25.
//

#include "Engine/ShaderManager.h"

std::map<std::string, std::shared_ptr<Shader>> ShaderManager::m_Shaders;

std::shared_ptr<Shader> ShaderManager::LoadShader(const char *vertexPath, const char *fragmentPath) {


    // FIXME: this is not a good way to create id
    //  we should hash each file and create id from that
    //  m_shaders[vertPath] = vertexShader;
    //  m_shaders[fragPath] = fragmentShader;
    //  m_programs[vertPath + fragPath] = program;

    //create id from paths
    std::string id = std::string(vertexPath) + "_" + std::string(fragmentPath);

    // check if shader already exists
    if (m_Shaders.find(id) != m_Shaders.end()) {
        return m_Shaders[id];
    }

    auto shader = std::make_shared<Shader>(std::string(vertexPath), fragmentPath);

    // add to map
    m_Shaders[id] = shader;

    return shader;
}


void ShaderManager::Clear() {
    m_Shaders.clear();
}
