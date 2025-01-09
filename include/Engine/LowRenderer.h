//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_LOWRENDERER_H
#define RAY_GAME_LOWRENDERER_H


#include "Engine/Renderer/Rectangle.h"

class LowRenderer {
private:
    static float lastTime;
public:
    static float getDeltaTime();

    static void DrawRectangle(Rectangle rectangle);
};


#endif //RAY_GAME_LOWRENDERER_H
