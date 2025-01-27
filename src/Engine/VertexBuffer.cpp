//
// Created by Bekir Gulestan on 1/26/25.
//
#include "Engine/Object/VertexBuffer.h"

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
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
