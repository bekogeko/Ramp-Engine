//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"
#include <fstream>

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

unsigned int HighRenderer::RegisterObject(const std::string pathName) {

    // 1- read file
    // 2- create object
    // 3- register object
    // 4- return id

    // 1- read file
    std::ifstream file(pathName);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << pathName << std::endl;
        return 0;
    }

    std::string line;
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;

    while (std::getline(file, line)) {

        // if line starts with -v
        // read the next 2 floats
        // floats are in the format of x,y
        // there are in model space [-1,1]


        // 1.1- read vertices
        if (strcmp(line.substr(0, 2).c_str(), "-v") == 0) {
            // read the next 2 floats
            // separated by a comma
            std::string x, y;
            std::getline(file, x, ',');
            std::getline(file, y);

            vertices.emplace_back(std::stof(x), std::stof(y));
        }

        // if line starts with -i
        // read the next 3 integers
        // integers are the indices of the vertices
        // there are in the order of the vertices

        // 1.2- read indices
        if (strcmp(line.substr(0, 2).c_str(), "-i") == 0) {
            // read the next 3 integers
            // separated by a comma
            std::string i1, i2, i3;
            std::getline(file, i1, ',');
            std::getline(file, i2, ',');
            std::getline(file, i3);

            indices.push_back(std::stoi(i1));
            indices.push_back(std::stoi(i2));
            indices.push_back(std::stoi(i3));
        }

        // TODO: add support for colors
    }
    // 2.1- create object
    float *verticesArray = new float[vertices.size() * 2];
    unsigned int *indicesArray = new unsigned int[indices.size()];
    unsigned int i = 0;
    for (auto &v: vertices) {
        verticesArray[i++] = v.x;
        verticesArray[i++] = v.y;

    }

    i = 0;
    for (auto &ind: indices) {
        indicesArray[i++] = ind;
    }

    // 2.2- create object
    std::shared_ptr<Object> newObj = std::make_shared<Object>(verticesArray, vertices.size() * 2, indicesArray,
                                                              indices.size());

    // 3- register object
    newObj->registerObject();

    m_objects[newObj->getId()] = newObj;

    return newObj->getId();
}

