//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"
#include <fstream>
#include <iostream>
#include <sstream>

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

unsigned int HighRenderer::RegisterObject(const std::string path) {

    // 1- read file
    // 2- create object
    // 3- register object
    // 4- return id

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string fullPath = std::filesystem::current_path().c_str();
    fullPath += &RESOURCES_PATH[1];
    fullPath += shaderPath;
ßß
#elif PRODUCTION_BUILD == 0
    std::string fullPath = RESOURCES_PATH + path;
#endif

    // 1- read file
    std::ifstream file(fullPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return 0;
    }

    std::string line;
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;

    while (std::getline(file, line)) {

        if (line.empty())
            continue;
        // if line starts with -v
        // read the next 2 floats
        // floats are in the format of x,y
        // there are in model space [-1,1]


        // 1.1- read vertices
        if (strcmp(line.substr(0, 2).c_str(), "-v") == 0) {
            // remove -v from the line
            line = line.substr(2);

            // read the next 2 floats
            // separated by a comma
            std::string x, y;
            std::istringstream iss(line);
            std::getline(iss, x, ',');
            std::getline(iss, y);


            vertices.emplace_back(std::stof(x), std::stof(y));
            continue;
        }

        // if line starts with -i
        // read the next 3 integers
        // integers are the indices of the vertices
        // there are in the order of the vertices

        // 1.2- read indices
        if (strcmp(line.substr(0, 2).c_str(), "-f") == 0) {
            // read the next 3 integers
            // separated by a comma

            // remove -i from the line
            line = line.substr(2);

            std::string i1, i2, i3;
            std::istringstream iss(line);
            std::getline(iss, i1, ',');

            std::getline(iss, i2, ',');
            std::getline(iss, i3);

            // add face
            indices.push_back(std::stoi(i1));
            indices.push_back(std::stoi(i2));
            indices.push_back(std::stoi(i3));

            continue;
        }

        // TODO: add support for colors
    }
    // 2.1- create object
    auto *verticesArray = new float[vertices.size() * 2];
    auto *indicesArray = new unsigned int[indices.size()];
    unsigned int i = 0;

    // for each vertex in vertices
    // start from 0
    // add x and y to the array
    for (auto &vert: vertices) {
        verticesArray[i++] = vert.x;
        verticesArray[i++] = vert.y;
    }

    i = 0;
    // for each index in indices
    // start from 0
    // add to the array
    for (auto &index: indices) {
        indicesArray[i++] = index;
    }

    // 2.2- create object
    std::shared_ptr<Object> newObj = std::make_shared<Object>(verticesArray, vertices.size() * 2 * sizeof(float),
                                                              indicesArray, indices.size() * sizeof(unsigned int));

    // 3- register object
    newObj->registerObject();

    m_objects[newObj->getId()] = newObj;

    return newObj->getId();
}

