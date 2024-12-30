//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_OBJECT_H
#define RAY_GAME_OBJECT_H

#include "VertexArray.h"
#include "Shader.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Component.h"
#include "LowRenderer.h"
#include "Camera.h"
#include <string>
#include <iostream>

class Object {
private:
    std::unique_ptr<VertexArray> m_vertexArray;
    std::unique_ptr<Shader> m_shader;

    std::vector<std::unique_ptr<Component>> m_components;
private:
    bool m_isRegistered = false;
    unsigned int m_Id = 0;
public:

    glm::vec2 position{0, 0};
    glm::vec3 color{0, 0, 0};


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
    explicit Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize);

    void Draw(glm::mat4 camera);

    // attachComponent
    //  - Each Component type is only attachable once
    template<class T>
    void attachComponent() {

        // do a static assertion
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");


        std::unique_ptr<Component> comp = std::make_unique<T>();
        comp->attach(m_Id);
        m_components.push_back(std::move(comp));

    }


};

#endif //RAY_GAME_OBJECT_H
