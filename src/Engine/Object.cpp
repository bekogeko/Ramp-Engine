//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/Object.h"
#include "Engine/HighRenderer.h"
#include "Engine/ShaderManager.h"

Object::Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize) {

    m_vertexArray = std::make_unique<VertexArray>(vertices, size, indices, indicesSize);

    m_shader = std::move(ShaderManager::LoadShader("shaders/default.vert", "shaders/default.frag"));
    m_Id = HighRenderer::GetNextId();
}

Object::~Object() {
    // Clean up components
    m_components.clear();

    // Clean up shader
    m_shader.reset();

    // Clean up vertex array
    m_vertexArray.reset();
}

void Object::Draw(glm::mat4 camera) {
    ///
    /// Update Stage
    ///
    float deltaTime = LowRenderer::getDeltaTime();
    for (auto &component: m_components) {
        component->Update(deltaTime);
    }



    ///
    /// Render Stage
    ///
    for (auto &component: m_components) {
        // get first component
        component->Draw();
    }

    m_shader->Bind();

    // set uColor
    m_shader->SetUniform3f("uColor", color.r, color.g, color.b);

    // set Camera Matrix
    m_shader->SetUniformMat4("uProjection", &camera[0][0]);

    // create model matrix from
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // TODO: scaling factor
    // model = glm::scale(model, glm::vec3(scale, 1.0f));


    m_shader->SetUniformMat4("uModel", &model[0][0]);


    m_vertexArray->Bind();

    m_vertexArray->DrawElements();

    m_shader->Unbind();
    m_vertexArray->Unbind();


}
