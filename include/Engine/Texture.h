//
// Created by Bekir Gulestan on 1/11/25.
//

#ifndef RAY_GAME_TEXTURE_H
#define RAY_GAME_TEXTURE_H

#include <string>

class Texture {
public:
    explicit Texture(const std::string &path);

    explicit Texture(const unsigned char *data, int w, int h, int channelCount = 1);

    void Bind(int slot = 0);

    static void Unbind();

    [[nodiscard]] int slot() const {
        return m_slot;
    }

private:
    unsigned int m_textureId;
    unsigned int m_slot;

};

#endif //RAY_GAME_TEXTURE_H
