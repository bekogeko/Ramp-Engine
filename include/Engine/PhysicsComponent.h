//
// Created by Bekir Gulestan on 12/31/24.
//

#ifndef RAY_GAME_PHYSICSCOMPONENT_H
#define RAY_GAME_PHYSICSCOMPONENT_H

#include "Component.h"

class PhysicsComponent : public Component {
public:
    ~PhysicsComponent() override;
    
    void onAttached() override;

private:
    void Draw() override;

    void Update(float deltaTime) override;

};

#endif //RAY_GAME_PHYSICSCOMPONENT_H
