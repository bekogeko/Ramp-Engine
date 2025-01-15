//
// Created by Bekir Gulestan on 12/29/24.
//

#include "Engine/HighRenderer.h"
#include "Engine/UILayer.h"
#include "Engine/World.h"


#include <fstream>
#include <iostream>
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
    int index = layers.size() + 1;
    Layer *worldLayer = new World(index);
    layers.push_back(std::unique_ptr<Layer>(worldLayer));


    index = layers.size() + 1;
    Layer *uiLayer = new UILayer(index);
    layers.push_back(std::unique_ptr<Layer>(uiLayer));
}


