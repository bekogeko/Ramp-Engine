//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"

std::map<unsigned int, std::shared_ptr<Object>> HighRenderer::m_objects;

OrthoCamera HighRenderer::m_Camera(4, 3);


unsigned int HighRenderer::RegisterObject(const std::shared_ptr<Object> &object) {
    object->registerObject();
    m_objects[object->getId()] = object;
    std::cout << "Object registered with id: " << object->getId() << std::endl;

    return object->getId();
}

void HighRenderer::Draw() {

    for (auto [i, object]: m_objects) {
        object->Draw(m_Camera.getCameraMatrix());
    }
}

std::shared_ptr<Object> HighRenderer::getById(unsigned int id) {
    return m_objects[id];
}

unsigned int
HighRenderer::RegisterObject(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize) {
    // create
    std::shared_ptr<Object> newObj = std::make_shared<Object>(vertices, size, indices, indicesSize);

    newObj->registerObject();
    m_objects[newObj->getId()] = newObj;
    return newObj->getId();
}


OrthoCamera &HighRenderer::getCamera() {
    return m_Camera;
}

