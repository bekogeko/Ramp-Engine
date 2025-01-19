//
// Created by Bekir Gulestan on 1/16/25.
//

#ifndef RAY_GAME_OBJECTINTERFACE_H
#define RAY_GAME_OBJECTINTERFACE_H

#include "Engine/ShaderProgram.h"
#include "Engine/ResourceManager.h"


class ObjectInterface {

protected:
    std::shared_ptr<ShaderProgram> m_shader;
    bool m_isInstanced = false;


public:
    std::shared_ptr<ShaderProgram> getShader() const {
        return m_shader;
    }

    bool isInstanced() const { return m_isInstanced; }


    // TODO: we should be able to reAttachShader
    //  load other shaders on go
    // Load a new shader program
    void useShader(const std::string &vertexPath, const std::string &fragmentPath) {
        m_shader = ResourceManager::LoadShader(vertexPath.c_str(), fragmentPath.c_str());
    }

};

#endif //RAY_GAME_OBJECTINTERFACE_H
