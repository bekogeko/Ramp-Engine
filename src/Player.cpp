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
    ImGui::Text("Cam zoom: (%.3f)", cam.zoom);


    if (ImGui::Button("Reset Camera")) {
        cam.position = {0, 0};
        cam.zoom = 1.0f;
    }


    ImGui::DragFloat("Rotation of obj", &getObject()->rotation, 0, 0, 360);

    ImGui::End();

}

void Player::Update(float deltaTime) {
    auto myPhys = getObject()->getComponent<PhysicsComponent>();

    // if physics component exists and shift is not pressed
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


    OrthoCamera &cam = HighRenderer::getCamera();
//    if (Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {

    // if A or D pressed move camera
    if (Input::getKeyPress(GLFW_KEY_A) && Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        cam.position.x -= 100.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_D) && Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        cam.position.x += 100.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_W) && Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        cam.position.y += 100.0f * deltaTime;
    }
    if (Input::getKeyPress(GLFW_KEY_S) && Input::getKeyPress(GLFW_KEY_LEFT_SHIFT)) {
        cam.position.y -= 100.0f * deltaTime;
    }
//    }

    if (Input::getKeyPress(GLFW_KEY_COMMA)) {

        // decrease zoom by 1 per second
        cam.zoom -= 10.0f * deltaTime;

    }


}
