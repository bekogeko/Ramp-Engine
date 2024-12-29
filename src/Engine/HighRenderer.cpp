//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"

std::vector<std::unique_ptr<Object>> HighRenderer::m_objects;


void HighRenderer::RegisterObject(std::unique_ptr<Object> object) {
    object->registerObject();
    m_objects.push_back(std::move(object));
}

void HighRenderer::Draw() {
    for (auto &object: m_objects) {
        object->Draw();
    }
}
