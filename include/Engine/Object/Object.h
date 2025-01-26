//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_OBJECT_H
#define RAY_GAME_OBJECT_H

#include "Engine/VertexArray.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Component.h"
#include "Engine/LowRenderer.h"
#include "Engine/Camera.h"
#include "Engine/ObjectInterface.h"
#include "box2d/math_functions.h"

#include <string>
#include <iostream>
#include <typeindex>

class Object : public ObjectInterface {
private:
    std::unique_ptr<VertexArray> m_vertexArray;

    std::map<std::type_index, std::shared_ptr<Component> > m_components;


    std::vector<glm::vec2> m_vertices;
    std::vector<unsigned int> m_indices;
private:
    bool m_isRegistered = false;
    unsigned int m_Id = 0;

public:
    // readonly getVertices
    [[nodiscard]] std::vector<glm::vec2> getVertices() const {
        return m_vertices;
    }

    // get vertexCount
    // size
    [[nodiscard]] unsigned int getVertexCount() const {
        return m_vertices.size();
    }


    // in radians
    float rotation{0};
    glm::vec2 position{0, 0};
    glm::vec2 scale{1, 1};
    glm::vec4 color{0, 0, 0, 1};


    // 'registered' event called by HighRenderer
    [[nodiscard]] bool isRegistered() const {
        return m_isRegistered;
    }

    // register callback (sets 'registered' to true always)
    void registerObject() {
        m_isRegistered = true;
    }

    // get object id
    [[nodiscard]] unsigned int getId() const {
        return m_Id;
    }

    // with EBO
    explicit Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize,
                    const LayoutStack &stack);


    ~Object();

    void Draw();

    // attachComponent
    //  - Each Component type is only attachable once
    template<class T = Component>
    void attachComponent() {
        // do a static assertion
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
        std::shared_ptr<T> comp = std::make_shared<T>();
        comp->attach(m_Id);
// warning this might be released accidentally
        m_components[std::type_index(typeid(*comp))] = comp;
    }

    // getComponent
    //  - returns the first and only component of type T
    template<class T = Component>
    std::shared_ptr<T> getComponent() {
        // do a static assertion
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

        std::type_index index(typeid(T));
        if (m_components.count(std::type_index(typeid(T))) != 0) {
            return std::static_pointer_cast<T>(m_components[index]);
        } else {
            assert(("Component not found! ", false));
        }

    }


};

#endif //RAY_GAME_OBJECT_H
