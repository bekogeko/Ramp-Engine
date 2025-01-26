//
// Created by Bekir Gulestan on 12/28/24.
//

#ifndef RAY_GAME_VERTEXARRAY_H
#define RAY_GAME_VERTEXARRAY_H

#include <glad/glad.h>
#include "Engine/Object/LayoutStack.h"
#include "Engine/Object/VertexBuffer.h"

class VertexArray {
private:
    unsigned int m_VAO = 0;
    std::vector<std::unique_ptr<VertexBuffer>> m_VBOs;
    unsigned int m_EBO = 0;

    // size
    unsigned int m_size;

public:
    // without EBO Constructor
//    VertexArray(const float *vertices, unsigned int size, const LayoutStack &stack);

    //  with EBO Constructor
    VertexArray(unsigned int size, const unsigned int *indices, unsigned int indicesSize);

    //    VertexArray(const float *vertices, unsigned int size, const unsigned int *indices, unsigned int indicesSize,
    //                const LayoutStack &stack);
    void AddBuffer(const float *data, unsigned int size, const LayoutStack &stack);

//    rule of five
// Rule of five
    // Copy Constructor
    VertexArray(const VertexArray &other) {
        printf("Copy Constructor VA\n");
        m_size = other.m_size;
        m_EBO = other.m_EBO;
        m_VAO = other.m_VAO;

        // warning what about m_VBOs
        // warning: what about m_VBOs
        for (auto &vbo: m_VBOs) {
            vbo.reset();
        }
        m_VBOs.clear();
        for (const auto &vbo: other.m_VBOs) {
            m_VBOs.emplace_back(std::make_unique<VertexBuffer>(*vbo));
        }
    }


    // Copy Assignment Operator
    VertexArray &operator=(const VertexArray &other) {
        printf("Copy Assignment VA\n");
        if (m_EBO != other.m_EBO) {
            //TODO delete our EBO
            // - assign other EBO
        }

        if (m_VAO != other.m_VAO) {
            // TODO delete our v_VAO
            //  - assign other VAO
        }

        m_size = other.m_size;

        m_VBOs.clear();
        for (const auto &vbo: other.m_VBOs) {
            m_VBOs.emplace_back(std::make_unique<VertexBuffer>(*vbo));
        }

        return *this;
    }

    // Move Constructor
    VertexArray(VertexArray &&other) noexcept {
        printf("Move Constructor VA\n");
        m_size = other.m_size;
        m_VAO = other.m_VAO;
        m_EBO = other.m_EBO;

        m_VBOs = std::move(other.m_VBOs);
    }

    // Move Assignment Operator
    VertexArray &operator=(VertexArray &&other) noexcept {
        printf("Move Assignment VA\n");
        m_size = other.m_size;
        m_VAO = other.m_VAO;
        m_EBO = other.m_EBO;

        m_VBOs = std::move(other.m_VBOs);
        return *this;
    }


    ~VertexArray() {
        [[maybe_unused]] static int deleteCall = 0;
        printf("Vertex Array Deleted %d\n", deleteCall++);
        Delete();
    }

    void Bind() const {

        // bind the Vertex Array Object first,
        // then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        int i = 0;
        for (auto &m_VBO: m_VBOs) {
            m_VBO->Bind();
            m_VBO->Enable(i);
            VertexBuffer::Unbind();
            i++;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    }

    static void Unbind() {
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
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void DrawElementsInstanced(unsigned int instanceCount) const {
        unsigned int count = m_size / sizeof(unsigned int);
        glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
    }


    void Delete() {
        if (m_VAO != 0) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }

        for (auto &m_VBO: m_VBOs) {
            m_VBO->Delete();
        }

        if (m_EBO != 0) {
            glDeleteBuffers(1, &m_EBO);
            m_EBO = 0;
        }

    }

};

#endif //RAY_GAME_VERTEXARRAY_H
