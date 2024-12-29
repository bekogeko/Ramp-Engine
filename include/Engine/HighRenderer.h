//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_HIGHRENDERER_H
#define RAY_GAME_HIGHRENDERER_H

#include <glad/glad.h>
#include <iostream>

#include "Object.h"

class HighRenderer {
private:
    // array of game objects
    static std::vector<std::unique_ptr<Object>> m_objects;


public:
    static void RegisterObject(std::unique_ptr<Object> object);

    static void Draw();


};

#endif //RAY_GAME_HIGHRENDERER_H
