//
// Created by Bekir Gulestan on 12/30/24.
//

#include "Engine/Component.h"
#include "Engine/HighRenderer.h"

std::shared_ptr<Object> Component::getObject() const {
    auto obj = HighRenderer::getById(m_ObjectId);
    return obj;
}
