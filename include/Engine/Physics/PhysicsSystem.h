//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_PHYSICS_H
#define RAY_GAME_PHYSICS_H


#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "glm/vec2.hpp"

class PhysicsSystem {
public:
    explicit PhysicsSystem(entt::registry& registry, const glm::vec2& gravity = glm::vec2(0.0f, -9.81f));

    // Fixed-step is preferable in production; for simplicity we step with dt here.
    void Step(float dt);

private:
    entt::registry& m_registry;
    b2WorldId m_world;

    void connectSignals();
    void onRigidBodyConstruct(entt::registry& reg, entt::entity e) const;
    void onRigidBodyDestroy(entt::registry& reg, entt::entity e) const;
    void onTransformUpdated(entt::registry&, entt::entity);

    void createOrRefreshBody(entt::entity e) const;
    void destroyBodyIfAny(entt::entity e) const;

    void syncTransformsFromPhysics() const;
};


#endif //RAY_GAME_PHYSICS_H