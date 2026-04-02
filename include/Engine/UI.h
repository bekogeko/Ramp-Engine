//
// Created by Bekir Gulestan on 12/7/25.
//

#ifndef RAY_GAME_UI_H
#define RAY_GAME_UI_H
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>

#include "glm/fwd.hpp"
#include "clay.h"

struct Color {
    glm::uint8_t r{255}, g{255}, b{255}, a{255};
};

enum class WrapMode : uint8_t {
    None = 0,
    Words,
    Anywhere
};

enum class AlignX : uint8_t { Left, Center, Right, Stretch };
enum class AlignY : uint8_t { Top, Middle, Bottom, Stretch };
enum class LayoutDirection : uint8_t { LeftToRight, TopToBottom, RightToLeft, BottomToTop };


struct Sizing {
    enum class Type : uint8_t { Grow, Fixed, Fit } type{Type::Grow};
    float value{0.0f}; // only used for Fixed

    static constexpr Sizing Grow() { return {Type::Grow, 0.0f}; }
    static constexpr Sizing Fit()  { return {Type::Fit, 0.0f}; }
    static constexpr Sizing Fixed(float v) { return {Type::Fixed, v}; }
};

struct Padding {
    float left{0}, top{0}, right{0}, bottom{0};
    static constexpr Padding All(float v) { return {v, v, v, v}; }
    static constexpr Padding LH(float l, float h) { return {l, h, l, h}; } // helper if needed
};

struct CornerRadius {
    float topLeft{0}, topRight{0}, bottomRight{0}, bottomLeft{0};
    static constexpr CornerRadius All(float r) { return {r, r, r, r}; }
};

struct Layout {
    Sizing width{Sizing::Grow()};
    Sizing height{Sizing::Grow()};
    Padding padding{Padding::All(0)};
    float childGap{0.0f};
    AlignX alignX{AlignX::Left};
    AlignY alignY{AlignY::Top};
    LayoutDirection direction{LayoutDirection::LeftToRight};
    bool clipChildren{false}; // enables scissor if true (for scrollable/clip regions)
};

struct BoxConfig {
    Layout layout{};
    Color backgroundColor{255,255,255,0}; // transparent by default
    CornerRadius cornerRadius{};          // all zeros = sharp corners
    bool hoverable{false};                // if true, backend tracks hover state for this box
};

struct TextConfig {
    Color textColor{255,255,255,255};
    uint32_t fontId{0};       // 0 means "backend default"
    uint16_t fontSize{16};
    uint16_t lineHeight{0};   // 0 = auto = fontSize
    uint16_t letterSpacing{0};
    WrapMode wrap{WrapMode::None};
};

namespace UI {

    inline Clay_Color ToClayColor(const Color color) {
        return Clay_Color{
            static_cast<float>(color.r),
            static_cast<float>(color.g),
            static_cast<float>(color.b),
            static_cast<float>(color.a)
        };
    }

    inline Clay_CornerRadius ToClayCornerRadius(const CornerRadius radius) {
        return Clay_CornerRadius{
            radius.topLeft,
            radius.topRight,
            radius.bottomLeft,
            radius.bottomRight
        };
    }

    inline Clay_Padding ToClayPadding(const Padding padding) {
        auto clampToUint16 = [](const float value) -> uint16_t {
            if (value <= 0.0f) {
                return 0;
            }
            return static_cast<uint16_t>(value);
        };

        return Clay_Padding{
            clampToUint16(padding.left),
            clampToUint16(padding.right),
            clampToUint16(padding.top),
            clampToUint16(padding.bottom)
        };
    }

    inline Clay_SizingAxis ToClaySizingAxis(const Sizing sizing) {
        Clay_SizingAxis axis{};

        switch (sizing.type) {
            case Sizing::Type::Fit:
                axis.type = CLAY__SIZING_TYPE_FIT;
                axis.size.minMax = Clay_SizingMinMax{0.0f, 0.0f};
                break;
            case Sizing::Type::Fixed:
                axis.type = CLAY__SIZING_TYPE_FIXED;
                axis.size.minMax = Clay_SizingMinMax{sizing.value, sizing.value};
                break;
            case Sizing::Type::Grow:
            default:
                axis.type = CLAY__SIZING_TYPE_GROW;
                axis.size.minMax = Clay_SizingMinMax{0.0f, 0.0f};
                break;
        }

        return axis;
    }

    inline Clay_LayoutAlignmentX ToClayAlignX(const AlignX alignX) {
        switch (alignX) {
            case AlignX::Center:
                return CLAY_ALIGN_X_CENTER;
            case AlignX::Right:
                return CLAY_ALIGN_X_RIGHT;
            case AlignX::Stretch:
            case AlignX::Left:
            default:
                return CLAY_ALIGN_X_LEFT;
        }
    }

    inline Clay_LayoutAlignmentY ToClayAlignY(const AlignY alignY) {
        switch (alignY) {
            case AlignY::Middle:
                return CLAY_ALIGN_Y_CENTER;
            case AlignY::Bottom:
                return CLAY_ALIGN_Y_BOTTOM;
            case AlignY::Stretch:
            case AlignY::Top:
            default:
                return CLAY_ALIGN_Y_TOP;
        }
    }

    inline Clay_LayoutDirection ToClayLayoutDirection(const LayoutDirection direction) {
        switch (direction) {
            case LayoutDirection::TopToBottom:
            case LayoutDirection::BottomToTop:
                return CLAY_TOP_TO_BOTTOM;
            case LayoutDirection::RightToLeft:
            case LayoutDirection::LeftToRight:
            default:
                return CLAY_LEFT_TO_RIGHT;
        }
    }

    inline Clay_TextElementConfigWrapMode ToClayWrapMode(const WrapMode wrapMode) {
        switch (wrapMode) {
            case WrapMode::Words:
                return CLAY_TEXT_WRAP_WORDS;
            case WrapMode::Anywhere:
                return CLAY_TEXT_WRAP_NEWLINES;
            case WrapMode::None:
            default:
                return CLAY_TEXT_WRAP_NONE;
        }
    }

    inline Clay_LayoutConfig ToClayLayout(const Layout& layout) {
        Clay_LayoutConfig config{};
        config.sizing.width = ToClaySizingAxis(layout.width);
        config.sizing.height = ToClaySizingAxis(layout.height);
        config.padding = ToClayPadding(layout.padding);
        config.childGap = static_cast<uint16_t>(layout.childGap <= 0.0f ? 0.0f : layout.childGap);
        config.childAlignment = Clay_ChildAlignment{
            ToClayAlignX(layout.alignX),
            ToClayAlignY(layout.alignY)
        };
        config.layoutDirection = ToClayLayoutDirection(layout.direction);
        return config;
    }

    inline Clay_ElementDeclaration ToClayDeclaration(const BoxConfig& config) {
        Clay_ElementDeclaration declaration{};
        declaration.layout = ToClayLayout(config.layout);
        declaration.backgroundColor = ToClayColor(config.backgroundColor);
        declaration.cornerRadius = ToClayCornerRadius(config.cornerRadius);

        if (config.layout.clipChildren) {
            declaration.clip = Clay_ClipElementConfig{
                true,
                true,
                Clay_Vector2{0.0f, 0.0f}
            };
        }

        return declaration;
    }

    inline Clay_TextElementConfig ToClayTextConfig(const TextConfig& config) {
        Clay_TextElementConfig textConfig{};
        textConfig.textColor = ToClayColor(config.textColor);
        textConfig.fontId = static_cast<uint16_t>(config.fontId);
        textConfig.fontSize = config.fontSize;
        textConfig.lineHeight = config.lineHeight;
        textConfig.letterSpacing = config.letterSpacing;
        textConfig.wrapMode = ToClayWrapMode(config.wrap);
        textConfig.textAlignment = CLAY_TEXT_ALIGN_LEFT;
        return textConfig;
    }

    inline Clay_ElementId ToClayId(const std::string_view id) {
        return Clay_GetElementId(Clay_String{
            false,
            static_cast<int32_t>(id.size()),
            id.data()
        });
    }

    class Element {
    public:
        Element(std::string_view id, const BoxConfig& config = {}) {
            Clay__OpenElementWithId(ToClayId(id));
            const Clay_ElementDeclaration declaration = ToClayDeclaration(config);
            Clay__ConfigureOpenElement(declaration);
        }

        ~Element() {
            Clay__CloseElement();
        }

        Element(const Element&) = delete;
        Element& operator=(const Element&) = delete;
        Element(Element&&) = delete;
        Element& operator=(Element&&) = delete;
    };

    template <typename Fn>
    void Box(std::string_view id, const BoxConfig& config, Fn&& children) {
        Element element(id, config);
        std::forward<Fn>(children)();
    }

    inline void Text(std::string_view value, const TextConfig& config = {}) {
        Clay_String text{
            false,
            static_cast<int32_t>(value.size()),
            value.data()
        };
        Clay_TextElementConfig* textConfig = Clay__StoreTextElementConfig(ToClayTextConfig(config));
        Clay__OpenTextElement(text, textConfig);
    }

    inline bool Hovered() {
        return Clay_Hovered();
    }
}

#endif //RAY_GAME_UI_H
