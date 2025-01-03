//
// Created by Bekir Gulestan on 12/31/24.
//
#include "Engine/PhysicsComponent.h"
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
