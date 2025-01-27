//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/VertexArray.h"
#include <algorithm>

VertexArray::VertexArray(const float *vertices, unsigned int size, const LayoutStack &stack) : m_size(size) {


    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    m_VBOs.emplace_back(vertices, size, stack);


    for (auto &m_VBO: m_VBOs) {
        m_VBO.Enable(0);
        // Unbind VBO
        VertexBuffer::Unbind();
    }
}

/// \brief
/// Constructor with EBO
/// \param vertices
/// \param size
/// \param indices
/// \param indicesSize
VertexArray::VertexArray(const float *vertices, unsigned int size, const unsigned int *indices,
                         unsigned int indicesSize,
                         const LayoutStack &stack) {

    // size
    m_size = size;

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    m_VBOs.emplace_back(vertices, size, stack);

    // Generate EBO
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Copy indices to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // Unbind VBO
    int i = 0;
    for (auto &m_VBO: m_VBOs) {
        m_VBO.Enable(i);
        i++;
        // Unbind VBO
        VertexBuffer::Unbind();
    }

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
