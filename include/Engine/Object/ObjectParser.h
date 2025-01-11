//
// Created by Bekir Gulestan on 1/9/25.
//

#ifndef RAY_GAME_OBJECTPARSER_H
#define RAY_GAME_OBJECTPARSER_H

#include <string>
#include <glm/vec2.hpp>

struct ParsedObject {
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> texCoords;
    bool isTextured = false;
};

class ObjectParser {
public:


    static ParsedObject LoadObject(std::string pathName);
};

#endif //RAY_GAME_OBJECTPARSER_H
