//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_LOWRENDERER_H
#define RAY_GAME_LOWRENDERER_H


#include "Engine/Renderer/Rectangle.h"
#include "Engine/Renderer/Text.h"

#include <map>


class LowRenderer {
private:
    static double lastTime;
    static double currentTime;
    static std::map<uint32_t, Rectangle> m_rectBatch;
    static std::map<uint32_t, Text> m_textBatch;

    static std::map<uint32_t, Text> m_prevTextBatch;

    // cache for text batch vbos
    static std::map<uint32_t, unsigned int> m_textVBOs;

public:
    static float getDeltaTime();

    static float getFPS();


    static void AddRectangle(uint32_t id, const Rectangle &rectangle);

    static void DrawRectangle(Rectangle rectangle);

    static void DrawRectangleBatched();

    static void AddText(uint32_t id, const Text &text);

    static void DrawTextBatched();

    static void DrawText(uint32_t id, Text text);

    static void swapTime();

    static void updateTime();
};


#endif //RAY_GAME_LOWRENDERER_H
