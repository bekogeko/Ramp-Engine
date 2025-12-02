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
#include "Engine/ResourceManager.h"
#include "Engine/LowRenderer.h"

const Clay_Color COLOR_ORANGE = (Clay_Color){225, 138, 50, 255};

void OnUserInterfaceError(Clay_ErrorData e) {
    // log error
    std::cerr << "Error: An error message here " << e.errorText.chars << std::endl;
}

// Example measure text function
static Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated
    auto font = ResourceManager::LoadFontById(config->fontId).lock();
    if (!font) {
        return Clay_Dimensions{0.0f, 0.0f};
    }

    // Measure string size for Font
    Clay_Dimensions textSize = {0};
    float maxLineWidthPx = 0.0f;      // track max width in final pixels to avoid unit mismatches
    float currentLineAdvance = 0.0f;  // sum of advances in font units
    int currentLineCharCount = 0;
    int linesCount = 1; // at least one line exists even if no '\n'

    float bakedSize = static_cast<float>(font->getFontSize());
    if (bakedSize <= 0.0f) bakedSize = 1.0f;
    const float scaleFactor = static_cast<float>(config->fontSize) / bakedSize;

    auto finalizeLine = [&](void) {
        // Convert accumulated advance to pixels and add letter spacing gaps for this line
        const int gaps = currentLineCharCount > 1 ? (currentLineCharCount - 1) : 0;
        const float widthPx = currentLineAdvance * scaleFactor + gaps * static_cast<float>(config->letterSpacing);
        maxLineWidthPx = std::max(maxLineWidthPx, widthPx);
        currentLineAdvance = 0.0f;
        currentLineCharCount = 0;
    };

    for (size_t i = 0; i < static_cast<size_t>(text.length); ++i) {
        unsigned char c = static_cast<unsigned char>(text.chars[i]);

        // Handle CRLF and lone CR gracefully
        if (c == '\r') {
            continue;
        }
        if (c == '\n') {
            finalizeLine();
            ++linesCount;
            continue;
        }

        // TODO: If you need UTF-8, decode to code points and use that instead of raw bytes.
        const auto& glyph = font->getChar(static_cast<char>(c));

        // Prefer glyph.advance; fall back conservatively when unavailable
        float glyphAdvance = glyph.advance;
        if (glyphAdvance == 0.0f) {
            // Fallback heuristic: avoid negative advances; many spaces have zero size and rely on advance
            glyphAdvance = std::max(glyph.size.x + glyph.bearing.x, 0.0f);
        }

        // Optional: kerning if available
        // if (currentLineCharCount > 0) {
        //     glyphAdvance += font->getKerning(prevChar, c);
        // }

        currentLineAdvance += glyphAdvance;
        ++currentLineCharCount;
        // Note: we don't add letterSpacing per glyph here; it is handled in finalizeLine() by gaps count.
    }

    // Final line
    finalizeLine();

    // Height: use explicit lineHeight if provided; otherwise fontSize
    const float lineAdvancePx = (config->lineHeight > 0) ? static_cast<float>(config->lineHeight)
                                                         : static_cast<float>(config->fontSize);

    textSize.width  = maxLineWidthPx;
    textSize.height = linesCount * lineAdvancePx;
    return textSize;
}


UILayer::UILayer()  {
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

    // should be implemented by child classes
    BuildUI();

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    for (int i = 0; i < renderCommands.length; ++i) {
        Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];
        Rectangle rect{};
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
                rect.cornerRadius = renderCommand->renderData.rectangle.cornerRadius.topLeft;
                if (rect.cornerRadius > 0) {
                    LowRenderer::DrawRoundedRectangle(renderCommand->id, rect);
                }else {
                    LowRenderer::AddRectangle(renderCommand->id, rect);
                }

                // LowRenderer::DrawRoundedRectangle(renderCommand->id, rect);
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



