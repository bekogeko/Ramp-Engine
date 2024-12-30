//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_CAMERA_H
#define RAY_GAME_CAMERA_H


#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"

class Camera {

public:
    virtual glm::mat4 getCameraMatrix() = 0;

    glm::vec2 position;
};


#endif //RAY_GAME_CAMERA_H
