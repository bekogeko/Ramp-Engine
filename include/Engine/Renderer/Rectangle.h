//
// Created by Bekir Gulestan on 1/9/25.
//

#ifndef RAY_GAME_RECTANGLE_H
#define RAY_GAME_RECTANGLE_H

#include "glm/glm.hpp"

class Rectangle {

public:
    glm::vec2 position;
    glm::vec2 size;

    glm::vec4 color;
    float cornerRadius;// 1 value for now not individual setup

};

#endif //RAY_GAME_RECTANGLE_H
