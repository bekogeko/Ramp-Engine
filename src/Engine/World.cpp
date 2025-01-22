//
// Created by Bekir Gulestan on 1/8/25.
//

#include "Engine/World.h"
#include "Engine/Object/Object.h"
#include "Engine/VertexArray.h"

#include <sstream>
#include <utility>

std::map<unsigned int, std::shared_ptr<Object>> World::m_objects;


void World::Update(float deltaTime) {

}


unsigned int World::RegisterObject(const std::shared_ptr<Object> &object) {
    object->registerObject();
    m_objects[object->getId()] = object;
    std::cout << "Object registered with id: " << object->getId() << std::endl;

    return object->getId();
}


std::shared_ptr<Object> World::getById(unsigned int id) {
    return m_objects[id];
}

unsigned int
World::RegisterObject(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize,
                      const LayoutStack &stack) {
    // create
    std::shared_ptr<Object> newObj = std::make_shared<Object>(vertices, size, indices, indicesSize, stack);

    newObj->registerObject();
    m_objects[newObj->getId()] = newObj;
    return newObj->getId();
}


unsigned int World::RegisterObject(const std::string &pathName) {

    // 1- read file
    // 2- create object
    // 3- register object
    // 4- return id

    auto objParsed = ResourceManager::LoadObject(pathName);
    LayoutStack stack = {
            VertexLayout(2)
    };
    // 2.1- create object
    auto *verticesArray = new float[objParsed.vertices.size() * 2];
    auto *indicesArray = new unsigned int[objParsed.indices.size()];
    unsigned int i = 0;

    // for each vertex in vertices
    // start from 0
    // add x and y to the array
    for (auto &vert: objParsed.vertices) {
        verticesArray[i++] = vert.x;
        verticesArray[i++] = vert.y;
    }

    i = 0;
    // for each index in indices
    // start from 0
    // add to the array
    for (auto &index: objParsed.indices) {
        indicesArray[i++] = index;
    }

    // 2.2- create object
    std::shared_ptr<Object> newObj = std::make_shared<Object>(verticesArray,
                                                              objParsed.vertices.size() * stack.getSize(),
                                                              indicesArray,
                                                              objParsed.indices.size() * sizeof(unsigned int),
                                                              stack);

    // 3- register object
    newObj->registerObject();


    m_objects[newObj->getId()] = newObj;
    delete[] indicesArray;
    delete[] verticesArray;

    return newObj->getId();
}

World::World(int index) : Layer(index) {

}

void World::Draw() {
    for (auto [i, object]: m_objects) {
        object->Draw();
    }
}
