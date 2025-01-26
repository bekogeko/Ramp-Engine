//
// Created by Bekir Gulestan on 1/26/25.
//

#include "Engine/Object/VertexBuffer.h"


VertexBuffer::VertexBuffer(const float *data, const unsigned int size, LayoutStack stack) : m_stack(std::move(stack)) {
    glGenBuffers(1, &m_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

    // TODO: glBufferData
    //  -  GL_DRAW_METHOD

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * m_stack.getDimentionCount() * size,
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
