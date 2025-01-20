//
// Created by Bekir Gulestan on 1/15/25.
//
#include "Engine/Font.h"
#include "Engine/ResourceManager.h"
#include <stb_truetype/stb_truetype.h>
#include <iostream>
#include <glad/glad.h>
#include <vector>

#if PRODUCTION_BUILD == 1
#include <filesystem>
#endif


Font::Font(const std::string &pathName, int fontSize, unsigned char hashId) : m_hashId(hashId) {
    unsigned char ttf_buffer[1 << 20];
    unsigned char temp_bitmap[512 * 512];

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string fullPath = std::filesystem::current_path().c_str();
    fullPath += &RESOURCES_PATH[1];
    fullPath += pathName;
#elif PRODUCTION_BUILD == 0
    std::string fullPath = RESOURCES_PATH + pathName;
#endif

    fread(ttf_buffer, 1, 1 << 20, fopen(fullPath.c_str(), "rb"));

    stbtt_InitFont(&fontInfo, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

    // FIXME fontsize was assume to be fontSize*2
    stbtt_BakeFontBitmap(ttf_buffer, 0, fontSize * 2, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!


    // TODO Use Texture class
    auto texture = ResourceManager::LoadTextureFromBytes(
            pathName + "-" + std::to_string(fontSize),
            temp_bitmap,
            512, 512
    );

    m_ftex = texture;


}

glm::vec4 Font::getTextureCoords(char c) {
    std::vector<glm::vec2> texCoords;

    // calculates texture coords using stbtt
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &q.x0, &q.y0, &q, 1);


//    std::cout << "\n\nmin_s: " << q.s0 << " min_t: " << q.t0 << std::endl;
//    std::cout << "max_s: " << q.s1 << " max_t: " << q.t1 << std::endl;


    return {q.s0, q.t0, q.s1, q.t1};
}


Glyph Font::getChar(char c) {

    Glyph glyph;

    glyph.size = glm::vec2(cdata[c - 32].x1 - cdata[c - 32].x0, cdata[c - 32].y1 - cdata[c - 32].y0);
    glyph.bearing = glm::vec2(cdata[c - 32].xoff, cdata[c - 32].yoff);
    glyph.advance = cdata[c - 32].xadvance;

    return glyph;
}

void Font::Bind(int slot) {
    m_ftex->Bind(slot);
}
