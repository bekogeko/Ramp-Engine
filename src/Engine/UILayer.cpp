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

const Clay_Color COLOR_ORANGE = (Clay_Color){225, 138, 50, 255};

void OnUserInterfaceError(Clay_ErrorData e) {
    // log error
    std::cerr << "Error: An error message here " << e.errorText.chars << std::endl;
}

// Example measure text function
static Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated

    // get only first length characters
    auto sliced = std::string(text.chars, text.length);

    return {text.length * config->fontSize * 1.0f, config->fontSize * 1.0f};
}


UILayer::UILayer(int index) : Layer(index) {
    // Note: malloc is only used here as an example, any allocator that provides
    // a pointer to addressable memory of at least totalMemorySize will work
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));


    Clay_Initialize(arena, (Clay_Dimensions)
                    {
                        static_cast<float>(Window::getWidth()),
                        static_cast<float>(Window::getHeight())
                    }
                    ,
                    {
                        .
                        errorHandlerFunction = OnUserInterfaceError
                    }
    );

    // Tell clay how to measure text
    Clay_SetMeasureTextFunction(MeasureText, nullptr);
}

void UILayer::Draw() {
    Clay_BeginLayout();

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
             CLAY_SIZING_FIXED(82), CLAY_SIZING_FIXED(82)
             }
             }
             }) {
            CLAY(CLAY_ID("Content"), {
                 .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = {16, 16, 16, 16}},
                 .backgroundColor = {110, 85, 38, 255}

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

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    for (int i = 0; i < renderCommands.length; ++i) {
        Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];
        Rectangle rect;
        Text text;

        switch (renderCommand->commandType) {
            default:
                std::cerr << "Unhandled Command " << renderCommand->commandType << "\n";
                break;
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:


                rect.position.x = renderCommand->boundingBox.x;
                rect.position.y = renderCommand->boundingBox.y;

                // Scale the size
                rect.size.x = renderCommand->boundingBox.width;
                rect.size.y = renderCommand->boundingBox.height;


                rect.color.r = renderCommand->renderData.rectangle.backgroundColor.r / 255;
                rect.color.g = renderCommand->renderData.rectangle.backgroundColor.g / 255;
                rect.color.b = renderCommand->renderData.rectangle.backgroundColor.b / 255;
                rect.color.a = renderCommand->renderData.rectangle.backgroundColor.a / 255;

                // FIXME: corner radius is not working for all 4
                //                rect.cornerRadius = renderCommand->renderData.rectangle.cornerRadius.topLeft;

                LowRenderer::AddRectangle(renderCommand->id, rect);
                break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT:

                text.color.r = (renderCommand->renderData.text.textColor.r) / 255;
                text.color.g = renderCommand->renderData.text.textColor.g / 255;
                text.color.b = renderCommand->renderData.text.textColor.b / 255;
                text.color.a = renderCommand->renderData.text.textColor.a / 255;

                text.value = std::string(renderCommand->renderData.text.stringContents.chars,
                                         renderCommand->renderData.text.stringContents.length);

                text.position.x = renderCommand->boundingBox.x;
                text.position.y = renderCommand->boundingBox.y;

                text.size.x = renderCommand->boundingBox.width;
                text.size.y = renderCommand->boundingBox.height;

                text.fontId = renderCommand->renderData.text.fontId;

                text.fontSize = renderCommand->renderData.text.fontSize;
                text.lineHeight = renderCommand->renderData.text.lineHeight;
                text.letterSpacing = renderCommand->renderData.text.letterSpacing;

                LowRenderer::AddText(renderCommand->id, text);
                break;
        }
    }

    LowRenderer::DrawRectangleBatched();
    LowRenderer::DrawTextBatched();
}


void UILayer::Update(float deltaTime) {
    auto mousePos = Input::getMousePosition();
    auto isMouseDown = Input::getMouseButton(0);

    // Update internal pointer position for handling mouseover / click / touch events
    Clay_SetPointerState((Clay_Vector2)
                         {
                             mousePos.x, mousePos.y
                         }
                         ,
                         isMouseDown
    );
}

void UILayer::UpdateWindowSize() {
    Clay_SetLayoutDimensions(
        (Clay_Dimensions)
        {
            static_cast<float>(Window::getWidth()),
            static_cast<float>(Window::getHeight())
        }
    );
}



