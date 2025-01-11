//
// Created by Bekir Gulestan on 12/31/24.
//
#include "Engine/Object/PhysicsComponent.h"
#include "Engine/Physics.h"
#include "Engine/Input.h"
#include <iostream>

void PhysicsComponent::Draw() {

}


void PhysicsComponent::Update(float deltaTime) {

}

PhysicsComponent::~PhysicsComponent() {
    Physics::RemoveObject(m_ObjectId);
}

void PhysicsComponent::onAttached() {

    // warning
    m_BodyId = Physics::AddObject(m_ObjectId);
}

void PhysicsComponent::applyForce(glm::vec2 force) {
    // force point
    b2Vec2 pos = b2Body_GetPosition(m_BodyId);

    b2Body_ApplyForce(m_BodyId, (b2Vec2) {force.x, force.y}, pos, true);
}
