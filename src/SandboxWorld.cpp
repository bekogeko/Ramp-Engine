//
// Created by Bekir Gulestan on 11/17/25.
//

#include "SandboxWorld.h"

SandboxWorld::SandboxWorld() :physics(m_Scene.Registry())  {
    for (int i = 0; i < 7; i++){
        Entity square = m_Scene.CreateEntity();
        square.GetComponent<Transform>().position.x = 1.1f * i -3.35f;
        square.AddComponent<MeshComponent>(MeshComponent::LoadFromFile("square.obj"));
        square.AddComponent<RigidBody2D>(RigidBody2D{
            .body = b2_nullBodyId,
            .type = b2_dynamicBody,
            .fixedRotation = false,
        });
        square.AddComponent<BoxCollider2D>(BoxCollider2D{
            .hx = .5f,
            .hy = .5f,
            .density = 1.0f,
            .friction = 0.3f,
            .restitution = 0.0f,
            .isSensor = false,
        });
    }
    // Static ground
    {
        Entity ground = m_Scene.CreateEntity();
        // ground.GetTransform().scale.x = 7.0f;
        // ground.GetTransform().scale.y = 1.0f;
        ground.GetTransform().position.y -= 3.0f;
        ground.AddComponent<MeshComponent>(MeshComponent::LoadFromRectangle(7.0f, 1.0f));
        ground.AddComponent<RigidBody2D>(RigidBody2D{.type=b2_staticBody });
        ground.AddComponent<BoxCollider2D>(BoxCollider2D{ .hx=3.5f, .hy=0.5f, .density=0.0f, .friction=0.8f });
    }
}


void SandboxWorld::Draw() {
    m_Scene.Draw();
}

void SandboxWorld::Update(float deltaTime) {
    physics.Step(deltaTime);
}
