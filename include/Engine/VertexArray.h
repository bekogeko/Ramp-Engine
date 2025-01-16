//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_VERTEXARRAY_H
#define RAY_GAME_VERTEXARRAY_H

#include <glad/glad.h>
#include <vector>


class VertexLayout {
public:
    template<typename T = float>
    explicit VertexLayout(int count, bool isInstanced = false) {
        m_size = sizeof(T) * count;
        m_Dimension = count;
        m_IsInstanced = isInstanced;
    }

    bool isInstanced() const {
        return m_IsInstanced;
    }

    int getDimension() const {
        return m_Dimension;
    }

    int size() const {
        return m_size;
    }

private:

    int m_size; // size in bytes
    int m_Dimension;
    bool m_IsInstanced;
};

class LayoutStack {
public:
    // list constructor
    LayoutStack(std::initializer_list<VertexLayout> layouts) : m_layout(layouts) {}

    /// Returns total dimentsion count
    int getDimentionCount() {
        int totalDimension = 0;
        for (auto layout: *this) {
            totalDimension += layout.getDimension();
        }
        return totalDimension;
    }

    int getSize() {
        int totalSize = 0;
        for (auto layout: *this) {
            totalSize += layout.size();
        }
        return totalSize;
    }


    int getOffsetOfIndex(int index) {
        int offset = 0;
        for (int i = 0; i < index; ++i) {
            offset += m_layout[i].size();
        }
        return offset;
    }

    // will be able to used in for loop (auto a : stack)
    std::vector<VertexLayout>::iterator begin() { return m_layout.begin(); }

    // will be able to used in for loop (auto a: stack)
    std::vector<VertexLayout>::iterator end() { return m_layout.end(); }

private:
    std::vector<VertexLayout> m_layout;
};

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
    VertexArray(float *vertices, unsigned int size, LayoutStack stack);

    //  with EBO Constructor
    VertexArray(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize, LayoutStack stack);


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

    void DrawElementsInstanced(unsigned int instanceCount) const {
        unsigned int count = m_size / sizeof(unsigned int);
        unsigned int indCount = m_indexSize / sizeof(unsigned int);
        glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
    }


    void Delete() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

};

#endif //RAY_GAME_VERTEXARRAY_H
