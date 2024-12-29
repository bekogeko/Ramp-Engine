//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_OBJECT_H
#define RAY_GAME_OBJECT_H

#include "VertexArray.h"
#include "Shader.h"
#include <string>

class Object {
private:
    std::unique_ptr<VertexArray> m_vertexArray;
    std::unique_ptr<Shader> m_shader;

private:
    bool m_isRegistered = false;
    unsigned int m_Id = 0;
public:
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

        m_shader->Bind();
        m_vertexArray->Bind();

        m_vertexArray->DrawElements();

        m_shader->Unbind();
        m_vertexArray->Unbind();
    }

};

#endif //RAY_GAME_OBJECT_H
