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

    [[nodiscard]] glm::mat4 getProjectionMatrix() const override {

        float left = -1.0f * zoom * m_size.x;
        float right = 1.0f * zoom * m_size.x;
        float bottom = -1.0f * zoom * m_size.y;
        float top = 1.0f * zoom * m_size.y;

        glm::mat4 projection = glm::ortho(left, right, bottom, top);

        return projection;
    }

    //
    [[nodiscard]] glm::mat4 getViewMatrix() const override {
        glm::vec3 cameraPos = glm::vec3(position, 1.0f);
        glm::vec3 target = glm::vec3(position, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(cameraPos, target, up);
    }

    [[nodiscard]] glm::vec2 getSize() const { return m_size; }

    float zoom = 1.0f;
    glm::vec2 position{0, 0};
private:
    // in half size // [-x,x] , [-y,y]
    glm::vec2 m_size;

};

#endif //RAY_GAME_ORTHOCAMERA_H
