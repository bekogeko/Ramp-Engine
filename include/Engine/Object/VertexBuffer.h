//
// Created by Bekir Gulestan on 1/19/25.
//

#ifndef RAY_GAME_VERTEXBUFFER_H
#define RAY_GAME_VERTEXBUFFER_H

#include <glad/glad.h>

#include <utility>
#include "Engine/Object/LayoutStack.h"

class VertexBuffer {
public:
    /// \brief
    /// Constructor for VertexBuffer using Layoutstack
    /// \param data
    /// const pointer to float data
    /// \param count
    /// number of vertices in the VertexBuffer
    /// \param stack
    /// layout of each vertex also used for size calculation
    explicit VertexBuffer(const float *data, unsigned int count, const LayoutStack &stack);

    void Bind() const;

    static void Unbind();

    void Delete();

    void Enable(int startLocation);

    LayoutStack getStack() {
        return m_stack;
    }

    bool IsInstanced() const {
        return m_stack.IsInstanced();
    }

private:
    unsigned int m_vboId;
    LayoutStack m_stack;
};


#endif //RAY_GAME_VERTEXBUFFER_H
