//
// Created by Bekir Gulestan on 1/15/25.
//

#ifndef RAY_GAME_FONT_H
#define RAY_GAME_FONT_H

#include <string>
#include <glm/glm.hpp>
#include <stb_truetype/stb_truetype.h>
#include <unordered_map>

struct Glyph {
    glm::vec2 size;        // Size of the glyph in pixels
    glm::vec2 bearing;     // Offset from baseline to top-left of the glyph
    float advance;         // Offset to advance to the next character
//    glm::vec2 texCoords[2]; // Texture coordinates (bottom-left, top-right)
};

class Font {
public:
    Font(const std::string &fontPath, int fontSize);


    void Bind(int slot = 0);

    // returns
    // min_s, min_t, max_s, max_t
    glm::vec4 getTextureCoords(char c);

    Glyph getChar(char c);


    [[nodiscard]] int slot() const {
        return m_slot;
    }

private:
    std::unordered_map<char, Glyph> glyphs;

    unsigned char ttf_buffer[1 << 20];
    unsigned char temp_bitmap[512 * 512];


    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    unsigned int ftex;
    unsigned int m_slot;
    stbtt_fontinfo fontInfo;
};


#endif //RAY_GAME_FONT_H
