//
// Created by Bekir Gulestan on 11/17/25.
//

#include "SandboxUI.h"
#include "Engine/UI.h"

#include "Engine/Input.h"
#include "Engine/ResourceManager.h"

SandboxUI::SandboxUI() {
    // nothing to do for now
}

void SandboxUI::BuildUI() {

    const uint32_t monoFontId = ResourceManager::GetFontId("fonts/JetBrainsMono-Regular.ttf", 16);
    const uint32_t sansFontId = ResourceManager::GetFontId("fonts/DefaultSansRegular.ttf", 16);

    UI::Box("OuterContainer", {
        .layout = {
            .width = Sizing::Grow(),
            .height = Sizing::Grow(),
            .padding = Padding::All(16),
            .childGap = 16,
            .alignX = AlignX::Left,
            .alignY = AlignY::Top,
            .direction = LayoutDirection::LeftToRight
        }
    }, [&] {
        UI::Box("ContentBox", {
            .layout = {
                .width = Sizing::Fixed(160),
                .height = Sizing::Fixed(160)
            }
        }, [&] {
            UI::Box("Content", {
                .layout = {
                    .width = Sizing::Grow(),
                    .height = Sizing::Grow(),
                    .padding = Padding::All(16)
                },
                .backgroundColor = {110, 85, 38, 255},
                .cornerRadius = CornerRadius::All(8)
            }, [&] {
                UI::Box("LilCube", {
                    .layout = {
                        .width = Sizing::Grow(),
                        .height = Sizing::Grow()
                    },
                    .backgroundColor = {28, 50, 202, 255}
                }, [&] {
                    const bool isHovered = UI::Hovered();
                    const bool isClicked = isHovered && Input::getMouseButton(0);

                    if (isClicked) {
                        UI::Text("Clicked!", {
                            .textColor = {255, 120, 12, 255},
                            .fontId = monoFontId,
                            .fontSize = 16
                        });
                    } else if (isHovered) {
                        UI::Text("hello Mouse cursor", {
                            .textColor = {255, 120, 12, 255},
                            .fontId = monoFontId,
                            .fontSize = 16
                        });
                    } else {
                        UI::Text("Im away", {
                            .textColor = {255, 120, 12, 255},
                            .fontId = monoFontId,
                            .fontSize = 16
                        });
                    }
                });
            });
        });

        UI::Box("ContentBox2", {
            .layout = {
                .width = Sizing::Fixed(240),
                .height = Sizing::Grow()
            }
        }, [&] {
            UI::Box("Content2", {
                .layout = {
                    .width = Sizing::Fit(),
                    .height = Sizing::Fit(),
                    .padding = Padding::All(16)
                },
                .backgroundColor = {255, 255, 255, 255}
            }, [&] {
                UI::Text("hello world yanki gap wallahi billahi essalami salami kardecim", {
                    .textColor = {12, 120, 255, 255},
                    .fontId = sansFontId,
                    .fontSize = 16,
                    .wrap = WrapMode::Words
                });
            });
        });
    });
}
