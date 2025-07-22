//
// Created by Bekir Gulestan on 7/20/25.
//

#ifndef RAY_GAME_SCENE_H
#define RAY_GAME_SCENE_H

#include <entt/entt.hpp>

class Entity;


class Scene {
public:

    Entity CreateEntity();

    void Draw();

private:
    friend class Entity;

    entt::registry m_registry;

};

#endif //RAY_GAME_SCENE_H
