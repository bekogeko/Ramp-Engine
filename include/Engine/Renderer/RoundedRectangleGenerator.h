//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_ROUNDEDRECTANGLEGENERATOR_H
#define RAY_GAME_ROUNDEDRECTANGLEGENERATOR_H
#include "CircleGenerator.h"
#include "glm/vec2.hpp"

struct RoundedRectangleGenerator {
    glm::vec2 const size;
    glm::vec2 const centers[4];
    uint32_t const arc_quality;
    CircleGenerator const generator;

    RoundedRectangleGenerator(glm::vec2 const &size, float radius, uint32_t arc_quality) : size(size), arc_quality(arc_quality/4)
    ,centers{
    {size.x - radius, size.y - radius}, // bottom right
    {radius, size.y - radius}, // bottom left
    {radius, radius}, // top left
    {size.x - radius, radius}, // top right
    },
    generator(radius, arc_quality - 4)
    {
    }

    glm::vec2 getPoint(uint32_t index) const {
        uint32_t const corner_idx {index/arc_quality};
        return centers[corner_idx] + generator.generatePoint(index - corner_idx);
    }

};
#endif //RAY_GAME_ROUNDEDRECTANGLEGENERATOR_H