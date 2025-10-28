//
// Created by Bekir Gulestan on 7/20/25.
//

#include "Engine/Scene.h"
#include "Engine/Entity.h"

#include "Engine/Components/Transform.h"
#include "Engine/Components/MeshComponent.h"


#include "Engine/LowRenderer.h"
#include "Engine/HighRenderer.h"
#include "Engine/ResourceManager.h"


Entity Scene::CreateEntity() {
    entt::entity entity = m_registry.create();
    // add Transform component by default

    m_registry.emplace<Transform>(entity,
        glm::vec2(0.0f, 0.0f), // pos
        0.0f, // rotation in rad
        glm::vec2(1.0f, 1.0f) //scale
        );

    return Entity(entity, this);
}

void Scene::Draw() {

    // Iterate over all entities and draw them
    auto meshRenderables = m_registry.view<Transform, MeshComponent>();
    for (auto entity: meshRenderables) {
        auto [transform, mesh] = meshRenderables.get<Transform, MeshComponent>(entity);

        // use mesh data to draw
        VertexArray va(mesh.indices.data(),
                       mesh.indices.size() * sizeof(unsigned int));

        // TODO : LayoutStack should be created based on mesh data
        // For now, we assume the mesh has 2D positions only
        va.AddBuffer(mesh.vertices.data(),
                     mesh.vertices.size() * sizeof(float),
                     LayoutStack{
                             VertexLayout(2, false), // Position
                     });

        va.Bind();

        {
            auto shader_ptr = ResourceManager::LoadShader("shaders/default.vert", "shaders/default.frag");
            auto shader = shader_ptr.lock();
            shader->Bind();

            auto viewMat = HighRenderer::getCamera().getViewMatrix();
            auto projMat = HighRenderer::getCamera().getProjectionMatrix();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(transform.position, 0.0f));
            model = glm::rotate(model, transform.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(transform.scale, 1.0f));

            shader->SetUniformMat4("uProjection", &projMat[0][0]);
            shader->SetUniformMat4("uView", &viewMat[0][0]);
            shader->SetUniformMat4("uModel", &model[0][0]);
            // todo set color based on mesh or entity properties
            shader->SetUniform4f("uColor", 1, 1, 1, 1); // Default color white
        }

        va.Bind();
        va.DrawElements();

        ShaderProgram::Unbind();
        VertexArray::Unbind();
    }
}
