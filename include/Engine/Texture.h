//
// Created by Bekir Gulestan on 1/11/25.
//

#ifndef RAY_GAME_TEXTURE_H
#define RAY_GAME_TEXTURE_H

#include <string>

class Texture {
public:
    explicit Texture(const std::string &path);

    // TODO add multiChannel support
    explicit Texture(const unsigned char *data, int w, int h, int channelCount = 1);

    void Bind(int slot = 0);

    static void Unbind();

    [[nodiscard]] int slot() const {
        return m_slot;
    }

private:
    unsigned int m_textureId = 0;
    unsigned int m_slot = 0;

};

#endif //RAY_GAME_TEXTURE_H
