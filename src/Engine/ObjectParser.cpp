//
// Created by Bekir Gulestan on 1/9/25.
//

#include "Engine/ObjectParser.h"

#include <iostream>
#include <sstream>
#include <fstream>

ParsedObject ObjectParser::LoadObject(std::string pathName) {

    ParsedObject obj;

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string fullPath = std::filesystem::current_path().c_str();
    fullPath += &RESOURCES_PATH[1];
    fullPath += pathName;
#elif PRODUCTION_BUILD == 0
    std::string fullPath = RESOURCES_PATH + pathName;
#endif

    // 1- read file
    std::ifstream file(fullPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open object file: " << fullPath << std::endl;
        throw;
    }

    std::string line;

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


            obj.vertices.emplace_back(std::stof(x), std::stof(y));
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
            obj.indices.push_back(std::stoi(i1));
            obj.indices.push_back(std::stoi(i2));
            obj.indices.push_back(std::stoi(i3));

            continue;
        }

        // TODO: add support for colors
    }

    return obj;
}
