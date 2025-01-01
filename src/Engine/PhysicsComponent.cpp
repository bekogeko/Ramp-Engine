//
// Created by Bekir Gulestan on 12/31/24.
//
#include "Engine/PhysicsComponent.h"
#include "Engine/Physics.h"
#include <iostream>

void PhysicsComponent::Draw() {

}


void PhysicsComponent::Update(float deltaTime) {

    // if Space is pressed set position upper
    if (Input::getKeyPress(GLFW_KEY_SPACE)) {
        auto obj = getObject();
        // set velocity up
        b2Body_SetLinearVelocity(m_BodyId, {0, 5.0f});

    }

}

PhysicsComponent::~PhysicsComponent() {
    Physics::RemoveObject(m_ObjectId);
}

void PhysicsComponent::onAttached() {

    // warning
    m_BodyId = Physics::AddObject(m_ObjectId);


}
