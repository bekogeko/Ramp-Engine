//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"

std::map<unsigned int, std::weak_ptr<Object>> HighRenderer::m_objects;


void HighRenderer::RegisterObject(const std::shared_ptr<Object> &object) {
    object->registerObject();
    m_objects[object->getId()] = object;
    std::cout << "Object registered with id: " << object->getId() << std::endl;
}

void HighRenderer::Draw() {
    for (auto [i, object]: m_objects) {
        object.lock()->Draw();
    }
}
