//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_LOWRENDERER_H
#define RAY_GAME_LOWRENDERER_H


#include "Engine/Renderer/Rectangle.h"
#include "Engine/Renderer/Text.h"


class LowRenderer {
private:
    static double lastTime;
    static double currentTime;
public:
    static float getDeltaTime();

    static float getFPS();

    static void DrawRectangle(Rectangle rectangle);

    static void DrawText(Text text);

    static void swapTime();

    static void updateTime();
};


#endif //RAY_GAME_LOWRENDERER_H
