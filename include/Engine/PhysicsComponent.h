//
// Created by Bekir Gulestan on 12/31/24.
//

#ifndef RAY_GAME_PHYSICSCOMPONENT_H
#define RAY_GAME_PHYSICSCOMPONENT_H

#include "Component.h"
#include "box2d/id.h"
#include "glm/vec2.hpp"

class PhysicsComponent : public Component {
public:
    ~PhysicsComponent() override;

    void onAttached() override;

    void applyForce(glm::vec2 force);
    
private:
    void Draw() override;

    void Update(float deltaTime) override;

    b2BodyId m_BodyId;

};

#endif //RAY_GAME_PHYSICSCOMPONENT_H
