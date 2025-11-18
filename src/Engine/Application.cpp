//
// Created by Bekir Gulestan on 11/17/25.
//

#include "Engine/Application.h"

#include <iostream>

#include "Engine/Entity.h"
#include "Engine/LowRenderer.h"
#include "Engine/Scene.h"
#include "Engine/Window.h"
#include "Engine/Components/MeshComponent.h"
#include "Engine/Components/Rigidbody2D.h"
#include "Engine/Components/BoxCollider2D.h"
#include "Engine/Physics/PhysicsSystem.h"

std::vector<std::unique_ptr<Layer>> Ramp::Application::layers;


Ramp::Application::Application() {
    // TODO actually get these parameters
    m_window = std::make_unique<Window>(640,480,"Ramp-Engine");
}

Ramp::Application::~Application() {

    for (auto &layer: layers) {
        layer.reset();
    }
    layers.clear();
}

void Ramp::Application::Run() {
// // //    system("export MallocStackLogging=1;");
     std::cout << "Starting application...\n";
// //     // enable alpha blending
// //     // 0 means opaque
// //     // 1 means transparent
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glDepthMask(GL_TRUE);
// //     // disable vSync
// //     glfwSwapInterval(0);

     while (!m_window->shouldClose()) {
         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT);

         // Update Layer
         for (const auto &layer: layers) {
             layer->Update(LowRenderer::getDeltaTime());
         }

         // update
         for (const auto &layer: layers) {
             layer->Draw();
         }

         LowRenderer::swapTime();
         m_window->swapBuffers();
         m_window->pollInputs();
     }
}

// gets a layer type, and creates it with next index. every layer just
void Ramp::Application::PushLayer(Layer *layer) {
    std::unique_ptr<Layer> layerPtr = std::unique_ptr<Layer>(layer);
    layerPtr->OnAttach({m_window.get()});
    layers.push_back(std::move(layerPtr));
}
