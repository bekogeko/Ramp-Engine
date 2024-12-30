//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Player.h"
#include "Engine/HighRenderer.h"
#include <iostream>
#include "imgui.h"

void Player::Draw() {

    ImGui::Begin("Player");

    
    ImGui::End();

}

void Player::Update(float deltaTime) {

    // get obj from Renderer
    auto obj = getObject();
    obj->position.x += 0.1f * deltaTime;

}
