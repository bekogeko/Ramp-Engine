//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_ORTHOCAMERA_H
#define RAY_GAME_ORTHOCAMERA_H

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

class OrthoCamera : public Camera {
public:
    OrthoCamera(int width, int height) : m_size(width, height) {}

    glm::mat4 getCameraMatrix() override {
        glm::mat4 projection = glm::ortho(0.0f, m_size.x, 0.0f, m_size.y);
        glm::mat4 view = getViewMatrix();
        return projection * view;
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        glm::vec3 cameraPos = glm::vec3(position, 1.0f);
        glm::vec3 target = glm::vec3(position, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(cameraPos, target, up);
    }


    glm::vec2 position{0, 0};
private:
    glm::vec2 m_size;

};

#endif //RAY_GAME_ORTHOCAMERA_H
