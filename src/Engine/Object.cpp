//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/Object.h"
#include "Engine/HighRenderer.h"

Object::Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize) {

    m_vertexArray = std::make_unique<VertexArray>(vertices, size, indices, indicesSize);

    m_shader = std::make_unique<Shader>(std::string("default.vert"), std::string("default.frag"));

    m_Id = HighRenderer::GetNextId();
}
