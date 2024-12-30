//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Player.h"
#include "Engine/HighRenderer.h"
#include "Engine/Input.h"

#include <iostream>
#include "imgui.h"

void Player::Draw() {

    ImGui::Begin("Player");
    
    auto &cam = HighRenderer::getCamera();

    ImGui::Text("Cam Position: (%.1f, %.1f)", cam.position.x, cam.position.y);

    ImGui::End();

}

void Player::Update(float deltaTime) {

    // get obj from Renderer
    auto obj = getObject();
    obj->position.x += 0.1f * deltaTime;

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


}
