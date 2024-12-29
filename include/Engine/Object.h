//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_OBJECT_H
#define RAY_GAME_OBJECT_H

#include "VertexArray.h"
#include "Shader.h"
#include "LowRenderer.h"
#include <string>

class Object {
private:
    std::unique_ptr<VertexArray> m_vertexArray;
    Shader m_shader;

private:
    bool m_isRegistered = false;
public:
    // 'registered' event called by HighRenderer
    [[nodiscard]] bool isRegistered() const {
        return m_isRegistered;
    }

    // register callback (sets 'registered' to true always)
    void registerObject() {
        m_isRegistered = true;
    }

//    Object(float *vertices, unsigned int size) : m_vertexArray(vertices, size) {
//
//        m_shader = LowRenderer::CreateShaderProgram((std::string) "default.vert", "shaders/fragment.glsl");
//
//    }

    // with EBO
    explicit Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize) {

        m_vertexArray = std::make_unique<VertexArray>(vertices, size, indices, indicesSize);
        m_shader = LowRenderer::CreateShaderProgram((std::string) "default.vert", "default.frag");

    }


    void Draw() {
//        m_shader.Bind();
        m_vertexArray->Bind();

        m_vertexArray->DrawElements();
//        m_vertexArray.Draw();

        m_shader.Unbind();
        m_vertexArray->Unbind();
    }

};

#endif //RAY_GAME_OBJECT_H
