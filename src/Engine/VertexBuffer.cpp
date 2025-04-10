//
// Created by Bekir Gulestan on 1/26/25.
//

#include "Engine/Object/VertexBuffer.h"


VertexBuffer::VertexBuffer(const float *data, const unsigned int size, const LayoutStack &stack) : m_stack(
        std::move(stack)) {
    glGenBuffers(1, &m_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

    // TODO: glBufferData
    //  -  GL_DRAW_METHOD

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * m_stack.getDimensionCount() * size,
                 data,
                 GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete() {
    glDeleteBuffers(1, &m_vboId);
}

void VertexBuffer::Enable(int startLocation) {
    int attribIndex = startLocation;
    for (auto &layout: m_stack) {
        glEnableVertexAttribArray(attribIndex);
        glVertexAttribPointer(attribIndex, layout.getDimension(), GL_FLOAT, GL_FALSE,
                              m_stack.getDimensionCount() * sizeof(float),
                              reinterpret_cast<const void *>(m_stack.getOffsetOfIndex(attribIndex)));

        if (layout.IsInstanced())
            glVertexAttribDivisor(attribIndex, 1); // Tell OpenGL this is an attribute per instance.
        else
            glVertexAttribDivisor(attribIndex, 0); // Tell OpenGL this is an attribute per vertex.

        attribIndex++;
    }

}
