//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_CIRCLEGENERATOR_H
#define RAY_GAME_CIRCLEGENERATOR_H
#include <cstdint>
#include <vector>

#include "glm/vec2.hpp"

struct CircleGenerator {

    static float constexpr pi = 3.14159265358979323846f;

    float const radius;
    // number of points to use
    uint32_t const quality;
    // the angle of variation between two consequitove points
    float const da = 0.0f;

    CircleGenerator(float radius, uint32_t quality) : radius(radius), quality(quality),da{(2 * pi) / static_cast<float>(quality)} {}

    glm::vec2 generatePoint(uint32_t i) const {
        // compute the angle
        float angle = da * static_cast<float>(i);
        return {radius * cos(angle), radius * sin(angle)};
    }
};

#endif //RAY_GAME_CIRCLEGENERATOR_H