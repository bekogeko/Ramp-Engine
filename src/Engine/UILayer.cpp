//
// Created by Bekir Gulestan on 1/8/25.
//

#include "Engine/UILayer.h"
#include "Engine/Window.h"
#include "Engine/Input.h"

// rectangle shape
#include "Engine/Renderer/Rectangle.h"
#include "Engine/Renderer/Text.h"


// Must be defined in one file, _before_ #include "clay.h"
#define CLAY_IMPLEMENTATION

#include "clay.h"

const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void OnUserInterfaceError(Clay_ErrorData e) {

    // log error
    std::cerr << "Error: An error message here " << e.errorText.chars << std::endl;

}

// Example measure text function
static inline Clay_Dimensions MeasureText(Clay_String *text, Clay_TextElementConfig *config) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated
    return {};
}


UILayer::UILayer(int index) : Layer(index) {
    // Note: malloc is only used here as an example, any allocator that provides
    // a pointer to addressable memory of at least totalMemorySize will work
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    Clay_ErrorHandler errorHandler;
    errorHandler.userData = index;
    errorHandler.errorHandlerFunction = OnUserInterfaceError;

    Clay_Initialize(arena, (Clay_Dimensions) {static_cast<float>(Window::getWidth()),
                                              static_cast<float>(Window::getHeight())}, errorHandler);

    // Tell clay how to measure text
    Clay_SetMeasureTextFunction(MeasureText);

    auto mousePos = Input::getMousePosition();
    auto isMouseDown = Input::getMouseButton(0);

    // Update internal pointer position for handling mouseover / click / touch events
    Clay_SetPointerState((Clay_Vector2) {mousePos.x, mousePos.y}, isMouseDown);
}

void UILayer::Draw() {

    std::string val = "hello world\nyanki gap\nwallahi billahi\nessalami salami\nkardecimhello world\nyanki gap\nwallahi billahi\nessalami salami\nkardecimhello world\nyanki gap\nwallahi billahi\nessalami salami\nkardecimhello world\nyanki gap\nwallahi billahi\nessalami salami\nkardecim";
//    std::string val = "hello";

    Clay_BeginLayout();

    // An example of laying out a UI with a fixed width sidebar and flexible width main content
    CLAY(CLAY_ID("OuterContainer"),
         CLAY_LAYOUT({
                             .sizing={CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                             .padding={16, 16},
                             .childGap=16,
                             .childAlignment={
                                     CLAY_ALIGN_X_LEFT,
                                     CLAY_ALIGN_Y_TOP
                             },
                             .layoutDirection = CLAY_LEFT_TO_RIGHT,

                     }),
//         CLAY_RECTANGLE({.color={255, 0, 50, 40}})
         CLAY_RECTANGLE({.color={0, 0, 0, 255}})
    ) {


        CLAY(CLAY_ID("ContentBox"),
             CLAY_LAYOUT({.sizing={CLAY_SIZING_FIXED(82),
                                   CLAY_SIZING_FIXED(82)}})) {
            CLAY(CLAY_ID("Content"),
                 CLAY_LAYOUT({.sizing={CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding={16, 16}},),
                 CLAY_RECTANGLE({.color={0, 255, 0, 255}})) {

                CLAY(CLAY_ID("lilCube"),
                     CLAY_LAYOUT({.sizing={CLAY_SIZING_FIXED(32), CLAY_SIZING_FIXED(32)}, .padding={16, 16}}),
                     CLAY_RECTANGLE({.color={255, 120, 12, 255}})) {
                    CLAY_TEXT(CLAY_STRING("a"),
                              CLAY_TEXT_CONFIG({.textColor={255, 120, 12, 255}, .fontId=ResourceManager::GetFontId(
                                      "fonts/JetBrainsMono-Regular.ttf", 16), .fontSize = 16}));
                }
            }
        }
        CLAY(CLAY_ID("ContentBox2"),
             CLAY_LAYOUT({.sizing={CLAY_SIZING_FIXED(240),
                                   CLAY_SIZING_FIXED(240)}})) {
            CLAY(CLAY_ID("Content2"),
                 CLAY_LAYOUT({.sizing={CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding={16, 16}}),
                 CLAY_RECTANGLE({.color={255, 255, 255, 255}})) {
                CLAY_TEXT(CLAY_STRING(val.c_str()),
                          CLAY_TEXT_CONFIG({.textColor={12, 120, 255, 255},
                                                   .fontId=ResourceManager::GetFontId(
                                                           "fonts/JetBrainsMono-Regular.ttf", 16),
                                                   .fontSize = 16}));


            }
        }

    }

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
//    static int callTime = 1;
    for (int i = 0; i < renderCommands.length; ++i) {
        Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];

        switch (renderCommand->commandType) {
            default:
                std::cerr << "Unhandled Command " << renderCommand->commandType << "\n";
                break;
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
                Rectangle rect;

                rect.position.x = renderCommand->boundingBox.x;
                rect.position.y = renderCommand->boundingBox.y;

                // Scale the size
                rect.size.x = renderCommand->boundingBox.width;
                rect.size.y = renderCommand->boundingBox.height;


                rect.color.r = renderCommand->config.rectangleElementConfig->color.r / 255;
                rect.color.g = renderCommand->config.rectangleElementConfig->color.g / 255;
                rect.color.b = renderCommand->config.rectangleElementConfig->color.b / 255;
                rect.color.a = renderCommand->config.rectangleElementConfig->color.a / 255;

                LowRenderer::AddRectangle(renderCommand->id, rect);
                break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT:

                Text text;
                text.color.r = renderCommand->config.textElementConfig->textColor.r / 255;
                text.color.g = renderCommand->config.textElementConfig->textColor.g / 255;
                text.color.b = renderCommand->config.textElementConfig->textColor.b / 255;
                text.color.a = renderCommand->config.textElementConfig->textColor.a / 255;

//                renderCommand->config.textElementConfig->wrapMode == CLAY_TEXT_WRAP_NONE;
                text.value = renderCommand->text.chars;


                text.position.x = renderCommand->boundingBox.x;
                text.position.y = renderCommand->boundingBox.y;

                text.size.x = renderCommand->boundingBox.width;
                text.size.y = renderCommand->boundingBox.height;

                text.fontId = renderCommand->config.textElementConfig->fontId;

                text.fontSize = renderCommand->config.textElementConfig->fontSize;
                text.lineHeight = renderCommand->config.textElementConfig->lineHeight;
                text.letterSpacing = renderCommand->config.textElementConfig->letterSpacing;

                LowRenderer::AddText(renderCommand->id, text);
                break;
        }

    }

    LowRenderer::DrawRectangleBatched();
    LowRenderer::DrawTextBatched();
}


void UILayer::Update(float deltaTime) {

}



