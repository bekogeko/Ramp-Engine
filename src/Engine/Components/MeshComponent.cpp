//
// Created by Bekir Gulestan on 7/22/25.
//

#include "Engine/Components/MeshComponent.h"

#include <iostream>
#include <fstream>
#include <sstream>

MeshComponent MeshComponent::LoadFromFile(const std::string &filePath) {

#if PRODUCTION_BUILD
    std::string fullPath = std::filesystem::current_path().c_str();
    fullPath += &RESOURCES_PATH[1];
    fullPath += filePath;
#else
    std::string fullPath = RESOURCES_PATH + filePath;
#endif

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open mesh file: " << fullPath << std::endl;
        throw std::runtime_error("Failed to open mesh file");
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        // if line starts with -v
        // read the next 2 floats
        // floats are in the format of x,y
        if (line.substr(0, 2) == "-v") {
            line = line.substr(2);
            std::string x, y;
            std::istringstream iss(line);
            std::getline(iss, x, ',');
            std::getline(iss, y);
            vertices.push_back(std::stof(x));
            vertices.push_back(std::stof(y));
            continue;
        }

        // if line starts with -f
        // read the next 3 integers
        if (line.substr(0, 2) == "-f") {
            line = line.substr(2);
            std::string i1, i2, i3;
            std::istringstream iss(line);
            std::getline(iss, i1, ',');
            std::getline(iss, i2, ',');
            std::getline(iss, i3);
            indices.push_back(std::stoi(i1));
            indices.push_back(std::stoi(i2));
            indices.push_back(std::stoi(i3));
        }
    }

    if (vertices.empty() || indices.empty()) {
        std::cerr << "Mesh file is empty or malformed: " << fullPath << std::endl;
        throw std::runtime_error("Mesh file is empty or malformed");
    }

    return MeshComponent(vertices, indices);
}
