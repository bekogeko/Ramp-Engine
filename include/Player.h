//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_PLAYER_H
#define RAY_GAME_PLAYER_H

#include "Engine/Object/Component.h"

class Player : public Component {

    void Draw() override;

    void Update(float deltaTime) override;

};


#endif //RAY_GAME_PLAYER_H
