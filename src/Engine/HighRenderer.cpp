//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"
#include "Engine/UILayer.h"
#include "Engine/World.h"
#include "Engine/LowRenderer.h"
#include <sstream>

OrthoCamera HighRenderer::m_Camera(4, 3);
std::vector<std::unique_ptr<Layer>> HighRenderer::layers;

void HighRenderer::Draw() {
    LowRenderer::swapTime();
    for (auto &layer: layers) {
        layer->Draw();
    }

}

OrthoCamera &HighRenderer::getCamera() {
    return m_Camera;
}

void HighRenderer::Update(float deltaTime) {

}

void HighRenderer::Init() {
    int nextIndex = layers.size() + 1;

    Layer *worldLayer = new World(nextIndex);
    layers.push_back(std::unique_ptr<Layer>(worldLayer));


    nextIndex = layers.size() + 1;
    Layer *uiLayer = new UILayer(nextIndex);
    layers.push_back(std::unique_ptr<Layer>(uiLayer));
}

void HighRenderer::Destroy() {
    std::cout << "High Renderer: Destroyed\n";
    layers.clear();
}


