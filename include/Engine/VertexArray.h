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


public:
    // without EBO Constructor
    VertexArray(float *vertices, unsigned int size);

    //  with EBO Constructor
    VertexArray(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize);


    ~VertexArray() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void Bind() const {
        glBindVertexArray(m_VAO);
    }

    void Unbind() {
        glBindVertexArray(0);
    }

    void Draw() const {
        unsigned int count = m_size / sizeof(float);
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    void DrawElements() const {
        unsigned int count = m_size / sizeof(float);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }

};

#endif //RAY_GAME_VERTEXARRAY_H
