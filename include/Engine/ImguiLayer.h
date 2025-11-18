//
// Created by Bekir Gulestan on 11/18/25.
//

#ifndef RAY_GAME_IMGUILAYER_H
#define RAY_GAME_IMGUILAYER_H

#include "Layer.h"

class ImguiLayer : public Layer {
public:
    ImguiLayer();
    void Update(float deltaTime) override;

    void OnAttach(EngineContext ctx) override;
    void Draw() override;

    ~ImguiLayer() override;
};

#endif //RAY_GAME_IMGUILAYER_H