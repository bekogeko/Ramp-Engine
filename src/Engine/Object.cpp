//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/Object/Object.h"
#include "Engine/HighRenderer.h"
#include "Engine/World.h"
#include "Engine/ResourceManager.h"

Object::Object(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize,
               const LayoutStack &stack) {

    m_isInstanced = false;
    m_vertexArray = std::make_unique<VertexArray>(vertices, size, indices, indicesSize, stack);

    m_shader = std::move(ResourceManager::LoadShader("shaders/default.vert", "shaders/default.frag"));
    m_Id = World::GetNextId();

    // for later use
    m_indices = std::vector<unsigned int>(indices, indices + (indicesSize / sizeof(unsigned int)));
    for (int i = 0; i < size / sizeof(float); i += 2) {
        m_vertices.emplace_back(vertices[i], vertices[i + 1]);
    }

}

Object::~Object() {
    // Clean up components
    m_components.clear();

    // Clean up shader
    m_shader.reset();

    // Clean up vertex array
    m_vertexArray.reset();
}

void Object::Draw() {
    ///
    /// Update Stage
    ///
    float deltaTime = LowRenderer::getDeltaTime();
    for (auto &[typeInfo, comp]: m_components) {
        assert(comp);
        comp->Update(deltaTime);
    }



    ///
    /// Render Stage
    ///
    for (auto &[typeInfo, comp]: m_components) {
        // get first component
        assert(comp);
        comp->Draw();
    }

    m_shader->Bind();

    // set uColor
    m_shader->SetUniform4f("uColor", color.r, color.g, color.b, color.a);

    // get camera
    auto viewMat = HighRenderer::getCamera().getViewMatrix();
    auto projMat = HighRenderer::getCamera().getProjectionMatrix();
    // set Camera Matrix
    m_shader->SetUniformMat4("uProjection", &projMat[0][0]);
    m_shader->SetUniformMat4("uView", &viewMat[0][0]);


    // create model matrix from
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // TODO: scaling factor
    model = glm::scale(model, glm::vec3(scale, 1.0f));


    m_shader->SetUniformMat4("uModel", &model[0][0]);


    m_vertexArray->Bind();

    if (this->isInstanced())
        m_vertexArray->DrawElementsInstanced(1);
    else
        m_vertexArray->DrawElements();

    m_shader->Unbind();
    m_vertexArray->Unbind();

}
