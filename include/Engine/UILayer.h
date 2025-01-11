//
// Created by Bekir Gulestan on 1/8/25.
//

#ifndef RAY_GAME_UILAYER_H
#define RAY_GAME_UILAYER_H

#include "Layer.h"

class UILayer : public Layer {

public:
    explicit UILayer(int index);

    void Update(float deltaTime) override;

    void Draw() override;

};

#endif //RAY_GAME_UILAYER_H
