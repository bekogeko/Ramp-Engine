//
// Created by Bekir Gulestan on 12/30/24.
//


#include "Engine/HighRenderer.h"
#include "Engine/LowRenderer.h"
#include "Engine/Object.h"
#include "Engine/ObjectParser.h"
#include "Engine/Window.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

float LowRenderer::lastTime = 0.0f; // Initialization of lastTime
float LowRenderer::getDeltaTime() {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}

void LowRenderer::DrawRectangle(Rectangle rectangle) {
    auto objParsed = ObjectParser::LoadObject("square.obj");

    auto *vertices = new float[objParsed.vertices.size() * 2];
    auto *indices = new unsigned int[objParsed.indices.size()];

    // move vertices and indices
    std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);

    int i = 0;
    for (auto vert: objParsed.vertices) {
        vertices[i++] = vert.x;
        vertices[i++] = vert.y;
    }

    // define sizeof vertices
    unsigned int sizeofVertices = objParsed.vertices.size() * sizeof(float) * 2;
    // define sizeof indices
    unsigned int indicesSize = objParsed.indices.size() * sizeof(unsigned int);


    auto *obj = new Object(vertices, sizeofVertices, indices, indicesSize);

    std::cout << "R: " << rectangle.color.r << "\t G: " << rectangle.color.g << "\t B:" << rectangle.color.b
              << std::endl;
    std::cout << "X Pos   : " << rectangle.position.x << "\t\t Y Pos: " << rectangle.position.y << std::endl;
    std::cout << "Width   : " << rectangle.size.x << "\t Height: " << rectangle.size.y << std::endl;
    std::cout << "screen W: " << Window::getWidth() << "\t\t H: " << Window::getHeight() << std::endl;


    // Calculate pixel scaling based on the window's size and aspect ratio
    float screenWidth = static_cast<float>(Window::getWidth());
    float screenHeight = static_cast<float>(Window::getHeight());
    float aspectRatio = screenWidth / screenHeight;

    // Apply rectangle position and size scaling
    // position.x is in between [-4,4]
    // rect.pos.x is in between [0,640]

    // position.y is in between [0,480]
    // rect.pos.y is in between [-3,3]

    // if rect.size.x is 640
    // then rect size should be full
    // full is
    float sizeX = (rectangle.size.x / screenWidth) * 8.0f;
    float sizeY = (rectangle.size.y / screenHeight) * 6.0f;

    float posX = (rectangle.position.x / screenWidth) * 8.0f - 4.0f + (0.5f * sizeX);
    float posY = 3.0f - (rectangle.position.y / screenHeight) * 6.0f - (0.5f * sizeY);



    // posX -80

    // move origin from x[-4,4] to [0,640]
    // move origin from y[-3,3] to [0,480]
    obj->position.x = posX;
    obj->position.y = posY;

    obj->scale.x = sizeX;
    obj->scale.y = sizeY;


//    obj->scale.x = (rectangle.size.x / screenWidth) * 8.0f - 4.0f; // Normalize width
//    obj->scale.y = 3.0f - (rectangle.size.y / screenHeight) * 6.0f; // Normalize height ); // Normalize height




    obj->color.r = rectangle.color.r / 255;
    obj->color.g = rectangle.color.g / 255;
    obj->color.b = rectangle.color.b / 255;
    obj->color.a = rectangle.color.a / 255;


    obj->LoadShader("shaders/ui.vert", "shaders/ui.frag");

#define GL_ERROR_SILENT
    obj->Draw();
#undef GL_ERROR_SILENT

    delete obj;
}
