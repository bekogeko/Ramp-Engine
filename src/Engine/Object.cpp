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

void Object::Draw(glm::mat4 camera) {
    ///
    /// Update Stage
    ///
    float deltaTime = LowRenderer::getDeltaTime();
    for (const auto &component: m_components) {
        component->Update(deltaTime);
    }



    ///
    /// Render Stage
    ///
    for (auto &firstComponent: m_components) {
        // get first component
        firstComponent->Draw();
    }

    m_shader->Bind();

    // set uPosition
    m_shader->SetUniform2f("uPosition", position.x, position.y);

    // set uColor
    m_shader->SetUniform3f("uColor", color.r, color.g, color.b);

    // set Camera Matrix
    m_shader->SetUniformMat4("uProjection", &camera[0][0]);


    m_vertexArray->Bind();

    m_vertexArray->DrawElements();

    m_shader->Unbind();
    m_vertexArray->Unbind();


}
