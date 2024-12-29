//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/VertexArray.h"


VertexArray::VertexArray(float *vertices, unsigned int size) {

    // size
    m_size = size;

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // Set vertex attributes
    // Position attribute (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// \brief
/// Constructor with EBO
/// \param vertices
/// \param size
/// \param indices
/// \param indicesSize
VertexArray::VertexArray(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize) {

    // size
    m_size = size;

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // Generate EBO
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Copy indices to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    
    // Set vertex attributes
    // Position attribute (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
