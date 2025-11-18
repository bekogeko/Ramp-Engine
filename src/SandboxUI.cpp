//
// Created by Bekir Gulestan on 11/17/25.
//

#include "SandboxUI.h"

#include "clay.h"
#include "Engine/ResourceManager.h"

SandboxUI::SandboxUI() {
    // nothing to do for now
}

void SandboxUI::BuildUI() {
    CLAY(CLAY_ID("OuterContainer"), {
         .layout = {
             .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
             .padding = {16, 16, 16, 16},
             .childGap = 16,
             .childAlignment = {
                 CLAY_ALIGN_X_LEFT,
                 CLAY_ALIGN_Y_TOP
             },
             .layoutDirection = CLAY_LEFT_TO_RIGHT
         }
    }) {
        CLAY(CLAY_ID("ContentBox"), {
             .layout = {
                .sizing = {
                    CLAY_SIZING_FIXED(160), CLAY_SIZING_FIXED(160)
                }
             }
        }) {
            CLAY(CLAY_ID("Content"), {
                 .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = {16, 16, 16, 16}},
                 // .backgroundColor = {110, 85, 38, 255},
                 .backgroundColor = {110, 85, 38, 255},
                .cornerRadius = {8,8,8,8 }

            }) {
                CLAY(CLAY_ID("lilCube"), {
                     .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}},
                     .backgroundColor = {28, 50, 202, 255}
                }) {
                    bool isHovered = Clay_Hovered();
                    if (isHovered) {
                        CLAY_TEXT(CLAY_STRING("hello world"),
                                  CLAY_TEXT_CONFIG({
                                      .textColor = {255, 120, 12, 255}, .fontId = ResourceManager::GetFontId(
                                          "fonts/JetBrainsMono-Regular.ttf", 16),
                                      .fontSize = 16,
                                      }));
                    } else {
                        CLAY_TEXT(CLAY_STRING("a bc def"),
                                  CLAY_TEXT_CONFIG({
                                      .textColor = {255, 120, 12, 255}, .fontId = ResourceManager::GetFontId(
                                          "fonts/JetBrainsMono-Regular.ttf", 16),
                                      .fontSize = 16,
                                      }));
                    }
                }
            }
        }
        CLAY(CLAY_ID("ContentBox2"), {
             .layout = {.sizing = {CLAY_SIZING_FIXED(240), CLAY_SIZING_GROW()}}
             }) {
            CLAY(CLAY_ID("Content2"), {
                 .layout = {.sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()}, .padding = {16, 16, 16, 16}},
                 .backgroundColor = {255, 255, 255, 255}
                 }
            ) {
                //                CLAY_TEXT(CLAY_STRING("hello world\nyanki gap\nwallahi billahi\nessalami salami\nkardecim"),
                CLAY_TEXT(CLAY_STRING("hello world yanki gap wallahi billahi essalami salami kardecim"),
                          CLAY_TEXT_CONFIG({
                              .textColor = {12, 120, 255, 255},
                              .fontId = ResourceManager::GetFontId(
                                  "fonts/JetBrainsMono-Regular.ttf", 12),
                              .fontSize = 12,

                              .wrapMode = CLAY_TEXT_WRAP_WORDS,
                              }));
            }
        }
    }
}
