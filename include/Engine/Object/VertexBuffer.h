//
// Created by Bekir Gulestan on 1/19/25.
//

#ifndef RAY_GAME_VERTEXBUFFER_H
#define RAY_GAME_VERTEXBUFFER_H

#include <glad/glad.h>
#include "Engine/Object/LayoutStack.h"

class VertexBuffer {
public:
    VertexBuffer(LayoutStack stack) {
        glGenBuffers(1, &m_vboId);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

        // TODO: glBufferData
        //  - size , vertices, GL_DRAW_METHOD

        // TODO: VertexAttribs
        //  - glVertexAttrib
        //      - location, layoutDimension, offset, stackDimension
        //  - glVertexAttribDivisor
        //  - glEnableVertexAttribArray()
    }

    void Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    }

    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &m_vboId);
    }
private:
    unsigned int m_vboId;
};

#endif //RAY_GAME_VERTEXBUFFER_H
