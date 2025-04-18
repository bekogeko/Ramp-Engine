//
// Created by Bekir Gulestan on 1/9/25.
//

#ifndef RAY_GAME_TEXT_H
#define RAY_GAME_TEXT_H

#include <glm/glm.hpp>
#include <string>

class Text {

public:
    glm::vec4 color;
    std::string value;
    unsigned char fontId = 0;
    unsigned char fontSize = 16;
    unsigned char lineHeight = 16;
    unsigned char letterSpacing = 16;

    glm::vec2 position;
    glm::vec2 size;
};

#endif //RAY_GAME_TEXT_H
