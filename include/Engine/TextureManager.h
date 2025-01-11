//
// Created by Bekir Gulestan on 1/11/25.
//

#ifndef RAY_GAME_TEXTUREMANAGER_H
#define RAY_GAME_TEXTUREMANAGER_H

#include <memory>
#include "Texture.h"
#include <string>
#include <map>

class TextureManager {
public:

    static std::shared_ptr<Texture> LoadTexture(const std::string &path) {
        // check if string exists in the map
        if (m_Textures.find(path) != m_Textures.end()) {
            return m_Textures[path];
        }

        auto texture = std::make_shared<Texture>(path);
        m_Textures[path] = texture;
        return texture;
    }

private:
    static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
};

#endif //RAY_GAME_TEXTUREMANAGER_H
