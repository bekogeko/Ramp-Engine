//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Player.h"
#include "Engine/HighRenderer.h"
#include "Engine/Input.h"

#include <iostream>
#include "imgui.h"
#include "Engine/PhysicsComponent.h"

void Player::Draw() {

    ImGui::Begin("Player");

    auto &cam = HighRenderer::getCamera();

    ImGui::Text("Cam Position: (%.1f, %.1f)", cam.position.x, cam.position.y);

    if (ImGui::Button("Reset Camera")) {
        cam.position = {0, 0};
        cam.zoom = 1.0f;
    }
    ImGui::DragFloat("Rotation of obj", &getObject()->rotation, 0, 0, 360);

    ImGui::End();

}

void Player::Update(float deltaTime) {
    auto myPhys = getObject()->getComponent<PhysicsComponent>();
    if (myPhys && !Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        if (Input::getKeyPress(GLFW_KEY_SPACE)) {
            myPhys->applyForce({0, 15});
        }
        if (Input::getKeyPress(GLFW_KEY_A)) {
            myPhys->applyForce({-5, 0});
        }
        if (Input::getKeyPress(GLFW_KEY_D)) {
            myPhys->applyForce({5, 0});
        }
    }

    if (!Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        return;
    }

    OrthoCamera &cam = HighRenderer::getCamera();


    // if A or D pressed move camera
    if (Input::getKeyPress(GLFW_KEY_A)) {
        cam.position.x -= 1.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_D)) {
        cam.position.x += 1.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_W)) {
        cam.position.y += 1.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_S)) {
        cam.position.y -= 1.0f * deltaTime;
    }

    if (Input::getKeyPress(GLFW_KEY_PERIOD)) {
        cam.zoom += 0.1f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_COMMA)) {
        cam.zoom -= 0.1f * deltaTime;
    }


}
