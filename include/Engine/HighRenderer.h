//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_HIGHRENDERER_H
#define RAY_GAME_HIGHRENDERER_H

#include <glad/glad.h>
#include <iostream>
#include <map>

#include "Object.h"

class HighRenderer {
private:
    // array of game objects
    static std::map<unsigned int, std::weak_ptr<Object>> m_objects;


public:
    static void RegisterObject(const std::shared_ptr<Object> &object);

    static void Draw();

    static int GetNextId() {
        static int id = 1;
        return id++;
    }


    // Free
    static void FreeAll() {
        m_objects.clear();
    }
};

#endif //RAY_GAME_HIGHRENDERER_H

