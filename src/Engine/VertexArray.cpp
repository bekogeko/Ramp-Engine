//
// Created by Bekir Gulestan on 12/28/24.
//

#include "Engine/VertexArray.h"
#include <algorithm>

//VertexArray::VertexArray(const float *vertices, const unsigned int size, const LayoutStack &stack) : m_size(size) {
//
//    printf("VertexArray w/o EBO \n");
//    // Generate VAO
//    glGenVertexArrays(1, &m_VAO);
//    glBindVertexArray(m_VAO);
//
//    // Generate VBO
//    m_VBOs.emplace_back(vertices, size, stack);
//    // Copy vertices to buffer
//
////    if (stack.IsInstanced()) {
////        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
////    } else {
////    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
////    }
//
//    for (auto &m_VBO: m_VBOs) {
//        m_VBO.Enable(0);
//        // Unbind VBO
//        VertexBuffer::Unbind();
//    }
//}

/// \brief
/// Constructor with EBO
/// \param vertices
/// \param size
/// \param indices
/// \param indicesSize
VertexArray::VertexArray(const unsigned int size, const unsigned int *indices,
                         unsigned int indicesSize) {
//    static int ebo_Call = 0;
//    printf("VertexArray w/ EBO %d\n", ebo_Call++);

    // size
    // warning get this later
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
    VertexArray::Unbind();
}

void VertexArray::AddBuffer(const float *data, const unsigned int size, const LayoutStack &stack) {


//    void VertexArray::AddBuffer(const float *data, const unsigned int size, const LayoutStack &stack) {
    m_VBOs.emplace_back(std::make_unique<VertexBuffer>(data, size, stack));
    glBindVertexArray(m_VAO);
    m_VBOs.back()->Bind();
// FIXME maybe better options
    m_VBOs.back()->Enable(m_VBOs.size() - 1);
    VertexBuffer::Unbind();
    glBindVertexArray(0);
}

void VertexArray::Bind() const {

    // bind the Vertex Array Object first,
    // then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    int i = 0;
    for (auto m_VBO: m_VBOs) {
        m_VBO.Bind();
        m_VBO.Enable(i);

        i += m_VBO.getLayoutCount();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}
