//
// Created by Bekir Gulestan on 7/20/25.
//

#ifndef RAY_GAME_TRANSFORM_H
#define RAY_GAME_TRANSFORM_H

#include <glm/vec2.hpp>

struct Transform {
    glm::vec2 position;
    float rotation; // in radians
    glm::vec2 scale;

    Transform(const glm::vec2 &pos = {0.0f, 0.0f}, float rot = 0.0f, const glm::vec2 &scl = {1.0f, 1.0f})
            : position(pos), rotation(rot), scale(scl) {}
};

#endif //RAY_GAME_TRANSFORM_H
