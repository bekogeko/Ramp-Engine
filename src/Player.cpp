//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Player.h"
#include "Engine/HighRenderer.h"
#include <iostream>

void Player::Draw() {
//    std::cout << "hi player!" << std::endl;
}

void Player::Update(float deltaTime) {

    // get obj from Renderer
    auto obj = getObject();
    obj->position.x += 0.1f * deltaTime;

}
