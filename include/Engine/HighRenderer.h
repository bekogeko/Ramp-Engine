//
// Created by Bekir Gulestan on 12/29/24.
//

#ifndef RAY_GAME_HIGHRENDERER_H
#define RAY_GAME_HIGHRENDERER_H

#include <iostream>
#include <map>

#include "Engine/Object/Object.h"
#include "OrthoCamera.h"
#include "Layer.h"

class HighRenderer {
private:

    static OrthoCamera m_Camera;
    static std::vector<std::unique_ptr<Layer>> layers;

public:

    static OrthoCamera &getCamera();

    static void Update(float deltaTime);

    static void Init();

    static void Draw();

    static void Destroy();


};

#endif //RAY_GAME_HIGHRENDERER_H

