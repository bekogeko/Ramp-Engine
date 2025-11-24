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
    // 0 is null fontId
    unsigned char fontId = 0;

    unsigned char fontSize = 16;
    // factor for font size (1.25 means 20 for font size 16)
    unsigned char lineHeight = 20;
    // spacing between letters in pixels (i.e., 1 px)
    unsigned char letterSpacing = 1;

    glm::vec2 position;
    glm::vec2 size;
};

#endif //RAY_GAME_TEXT_H
