//
// Created by Bekir Gulestan on 1/3/25.
//

#ifndef RAY_GAME_SHADERMANAGER_H
#define RAY_GAME_SHADERMANAGER_H

#include "Shader.h"
#include <map>

class ShaderManager {

public:
    static std::shared_ptr<Shader> LoadShader(const char *vertexPath, const char *fragmentPath);

    static std::shared_ptr<Shader> GetShader(const char *name);

    // TODO: automatic maybe ?
    static void Clear();

private:
    static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;

};


#endif //RAY_GAME_SHADERMANAGER_H
