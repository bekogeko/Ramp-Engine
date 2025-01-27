//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Engine/Object/Component.h"
#include "Engine/HighRenderer.h"
#include "Engine/World.h"

std::weak_ptr<Object> Component::getObject() const {
    auto obj = World::getById(m_ObjectId);
    return obj;
}
