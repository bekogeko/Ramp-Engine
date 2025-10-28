//
// Created by Bekir Gulestan on 7/20/25.
//

#ifndef RAY_GAME_ENTITY_H
#define RAY_GAME_ENTITY_H

#include "Scene.h"
#include "Engine/Components//Transform.h"

class Entity {

public:
    Entity(entt::entity entity, Scene *scene) : m_entity(entity), m_scene(scene) {}

    // transform of the entity which is guaranteed to exist
    Transform &GetTransform() {
        if (!HasComponent<Transform>()) {
            throw std::runtime_error("Entity does not have a Transform component.");
        }
        return m_scene->m_registry.get<Transform>(m_entity);
    }

    template<typename T, typename ... Args>
    T &AddComponent(Args &&... args) {
        // Check if the component already exists
        if (HasComponent<T>()) {
            throw std::runtime_error("Component already exists on this entity.");
        }
        // Add the component to the entity
        return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    T &GetComponent() {
        return m_scene->m_registry.get<T>(m_entity);
    }

    template<typename T>
    bool HasComponent() const {
        return m_scene->m_registry.all_of<T>(m_entity);
    }

    template<typename T>
    void RemoveComponent() {
        if (!HasComponent<T>()) {
            throw std::runtime_error("Component does not exist on this entity.");
        }
        m_scene->m_registry.remove<T>(m_entity);
    }

private:
    entt::entity m_entity{entt::null};
    Scene *m_scene;
};

#endif //RAY_GAME_ENTITY_H
