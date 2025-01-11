//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_OBJECT_H
#define RAY_GAME_OBJECT_H

#include "VertexArray.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Component.h"
#include "LowRenderer.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "box2d/math_functions.h"
#include <string>
#include <iostream>

class Object {
private:
    std::unique_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<ShaderProgram> m_shader;

    std::vector<std::unique_ptr<Component>> m_components;

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
                    LayoutStack stack);

    // TODO: we should be able to reAttachShader
    //  load other shaders on go
    // Load a new shader program
    void LoadShader(const std::string &vertexPath, const std::string &fragmentPath);

    ~Object();

    void Draw();

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

    // getComponent
    //  - returns the first and only component of type T
    template<class T>
    T *getComponent() {
        for (auto &comp: m_components) {
            if (auto *t = dynamic_cast<T *>(comp.get())) {
                return t;
            }
        }
        return nullptr;
    }


};

#endif //RAY_GAME_OBJECT_H
