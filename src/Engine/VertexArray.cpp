//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/VertexArray.h"
#include <algorithm>

VertexArray::VertexArray(float *vertices, unsigned int size, LayoutStack stack) : m_size(size) {


    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    m_VBOs.emplace_back(stack);
    // Copy vertices to buffer

//    if (stack.IsInstanced()) {
//        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
//    } else {
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
//    }

    for (auto &m_VBO: m_VBOs) {
        m_VBO.Enable(0);
        // Unbind VBO
        m_VBO.Unbind();
    }
}

/// \brief
/// Constructor with EBO
/// \param vertices
/// \param size
/// \param indices
/// \param indicesSize
VertexArray::VertexArray(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize,
                         LayoutStack stack) {

    // size
    m_size = size;
    m_indexSize = indicesSize;
    // copy whole indicies to myIndicies
    m_indices = new unsigned int[indicesSize];
    std::copy(indices, indices + indicesSize, m_indices);

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    m_VBOs.emplace_back(stack);

    // Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // Generate EBO
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Copy indices to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // Unbind VBO
    for (auto &m_VBO: m_VBOs) {
        m_VBO.Enable(0);
        // Unbind VBO
        VertexBuffer::Unbind();
    }

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
