//
// Created by Bekir Gulestan on 12/31/24.
//

#ifndef RAY_GAME_PHYSICS_H
#define RAY_GAME_PHYSICS_H

#include <map>
#include "box2d/box2d.h"
#include "box2d/base.h"

class Physics {
public:
    ~Physics();

    static void Init();

    static void Update();

    static void RemoveObject(unsigned int m_Id);

    static b2BodyId AddObject(unsigned int m_Id);

private:
    static b2WorldId worldId;

    static std::map<unsigned int, b2BodyId> m_Objects;

};

#endif //RAY_GAME_PHYSICS_H
