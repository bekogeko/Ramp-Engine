//
// Created by Bekir Gulestan on 11/17/25.
//

#ifndef RAY_GAME_SANDBOXWORLD_H
#define RAY_GAME_SANDBOXWORLD_H


#include "Engine/Entity.h"
#include "Engine/Components/BoxCollider2D.h"
#include "Engine/Components/MeshComponent.h"
#include "Engine/Components/Rigidbody2D.h"
#include "Engine/Physics/PhysicsSystem.h"

#include "Engine/Layer.h"

class SandboxWorld: public Layer {
public:
    SandboxWorld();
    ~SandboxWorld() override = default;

    void Draw() override;
    void Update(float deltaTime) override;

private:
    Scene m_Scene;
    PhysicsSystem physics;

};


#endif //RAY_GAME_SANDBOXWORLD_H
