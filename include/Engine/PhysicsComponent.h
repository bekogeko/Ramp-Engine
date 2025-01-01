//
// Created by Bekir Gulestan on 12/31/24.
//

#ifndef RAY_GAME_PHYSICSCOMPONENT_H
#define RAY_GAME_PHYSICSCOMPONENT_H

#include "Component.h"
#include "box2d/id.h"

class PhysicsComponent : public Component {
public:
    ~PhysicsComponent() override;

    void onAttached() override;

private:
    void Draw() override;

    void Update(float deltaTime) override;

    b2BodyId m_BodyId;

};

#endif //RAY_GAME_PHYSICSCOMPONENT_H
