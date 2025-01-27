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
    explicit VertexBuffer(const float *data, unsigned int size, const LayoutStack &stack);

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
