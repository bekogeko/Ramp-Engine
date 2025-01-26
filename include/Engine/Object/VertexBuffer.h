//
// Created by Bekir Gulestan on 1/19/25.
//

#ifndef RAY_GAME_VERTEXBUFFER_H
#define RAY_GAME_VERTEXBUFFER_H

#include <glad/glad.h>

#include <utility>
#include "Engine/Object/LayoutStack.h"

class VertexBuffer {
public:
    explicit VertexBuffer(LayoutStack stack) : m_stack(std::move(stack)) {
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

    static void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &m_vboId);
    }

    void Enable(int startLocation) {
        int i = startLocation;
        for (auto &layout: m_stack) {
            glVertexAttribPointer(i, layout.getDimension(), GL_FLOAT, GL_FALSE,
                                  m_stack.getDimentionCount() * sizeof(float),
                                  reinterpret_cast<const void *>(m_stack.getOffsetOfIndex(i)));

            if (layout.IsInstanced())
                glVertexAttribDivisor(i, 1); // Tell OpenGL this is an attribute per instance.
            else
                glVertexAttribDivisor(i, 0); // Tell OpenGL this is an attribute per vertex.

            glEnableVertexAttribArray(i);
            i++;
        }
    }

private:
    unsigned int m_vboId;
    LayoutStack m_stack;
};

#endif //RAY_GAME_VERTEXBUFFER_H
