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

    glm::vec2 position{};
    glm::vec3 color{};


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

    void Draw() {

        ///
        /// Update Stage
        ///
        // FIXME: delta time is not so delta time
        float deltaTime = 0.16f;

        for (const auto &component: m_components) {
            component->Update(deltaTime);
        }



        ///
        /// Render Stage
        ///
        /// TODO: We should call `Draw()` for each component
        m_shader->Bind();

        // set uPosition
        m_shader->SetUniform2f("uPosition", position.x, position.y);
        // set uColor
        m_shader->SetUniform3f("uColor", color.r, color.g, color.b);


        m_vertexArray->Bind();

        m_vertexArray->DrawElements();

        m_shader->Unbind();
        m_vertexArray->Unbind();
    }

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
