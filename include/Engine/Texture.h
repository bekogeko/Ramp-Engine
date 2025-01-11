//
// Created by Bekir Gulestan on 1/11/25.
//

#ifndef RAY_GAME_TEXTURE_H
#define RAY_GAME_TEXTURE_H

#include <string>

class Texture {
public:
    Texture(std::string path);

    void Bind(int slot = 0);

private:
    unsigned int m_textureId;
    unsigned int m_slot;

};

#endif //RAY_GAME_TEXTURE_H
