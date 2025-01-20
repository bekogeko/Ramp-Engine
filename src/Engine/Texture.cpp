//
// Created by Bekir Gulestan on 1/11/25.
//
#include "Engine/Texture.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>
#if PRODUCTION_BUILD == 1
#include <filesystem>
#endif
Texture::Texture(const std::string &path) {

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string filePath = std::filesystem::current_path().c_str();
    filePath += &RESOURCES_PATH[1];
    filePath += path;
#elif PRODUCTION_BUILD == 0
    std::string filePath = RESOURCES_PATH + path;
#endif

    // Load image data
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        // warning suspicious conversion
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }
    stbi_image_free(data);
}


Texture::Texture(const unsigned char *data, int w, int h, int channelCount) {

    // TODO: add support for channelCount GL_RED etc // format

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // can free temp_bitmap at this point


    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void Texture::Bind(int slot) {
    // warning unbinding a texture might be a bad idea
    Unbind();
    m_slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}