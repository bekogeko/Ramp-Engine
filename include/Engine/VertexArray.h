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
    unsigned int m_size = 0;

public:
    // without EBO Constructor
//    VertexArray(const float *vertices, unsigned int size, const LayoutStack &stack);
    enum class DrawMode {
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
    };

    //  with EBO Constructor
    VertexArray(const unsigned int *indices, unsigned int indicesSize);


    /// \brief
    /// Add buffer to the VertexArray
    /// \param data
    /// const pointer to float data
    /// \param size
    /// size of the data in bytes
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
       
        m_size = other.m_size;
        m_VAO = other.m_VAO;
        m_EBO = other.m_EBO;

        m_VBOs = std::move(other.m_VBOs);
        return *this;
    }


    ~VertexArray() {
        Delete();
    }

    void Bind() const;

    static void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw(DrawMode mode = DrawMode::TRIANGLES) const {
        unsigned int count = m_size / sizeof(float);
        glDrawArrays((GLenum)mode, 0, count);
    }

    void DrawElements(DrawMode mode = DrawMode::TRIANGLES) const {
        unsigned int count = m_size / sizeof(unsigned int);
        glDrawElements((GLenum)mode, count, GL_UNSIGNED_INT, nullptr);
    }


    void DrawElementsInstanced(unsigned int instanceCount,DrawMode mode = DrawMode::TRIANGLES) const {
        unsigned int count = m_size / sizeof(unsigned int);
        glDrawElementsInstanced((GLenum)mode, count, GL_UNSIGNED_INT, nullptr, instanceCount);
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
