//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_VERTEXARRAY_H
#define RAY_GAME_VERTEXARRAY_H

#include <glad/glad.h>

class VertexArray {
private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;

    // size
    unsigned int m_size;
    unsigned int m_indexSize;
    unsigned int *m_indices;


public:
    // without EBO Constructor
    VertexArray(float *vertices, unsigned int size);

    //  with EBO Constructor
    VertexArray(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize);


    ~VertexArray() {
        Delete();
    }

    void Bind() const {

        // bind the Vertex Array Object first,
        // then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    }

    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw() const {
        unsigned int count = m_size / sizeof(float);
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    void DrawElements() const {
        unsigned int count = m_size / sizeof(unsigned int);
        unsigned int indCount = m_indexSize / sizeof(unsigned int);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void Delete() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

};

#endif //RAY_GAME_VERTEXARRAY_H
