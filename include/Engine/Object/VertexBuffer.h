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
    // TODO
    //  - add draw type (GL_STATIC_DRAW etc)
    explicit VertexBuffer(const float *vertices, unsigned int size, LayoutStack stack) : m_stack(std::move(stack)) {
        glGenBuffers(1, &m_vboId);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

        // TODO: glBufferData
        //  - size , vertices, GL_DRAW_METHOD
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);


        // TODO: VertexAttribs
        //  - glVertexAttrib
        //      - location, layoutDimension, offset, stackDimension
        //  - glVertexAttribDivisor
        //  - glEnableVertexAttribArray()
    }

    void Bind() const;

    static void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &m_vboId);
    }

    void Enable(int startLocation);

private:
    unsigned int m_vboId;
    LayoutStack m_stack;
};

#endif //RAY_GAME_VERTEXBUFFER_H
