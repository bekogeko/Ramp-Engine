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

    int pixelScale_X = 61;
    int pixelScale_Y = 81;

    obj->color.r = rectangle.color.r / 255;
    obj->color.g = rectangle.color.g / 255;
    obj->color.b = rectangle.color.b / 255;
    obj->color.a = rectangle.color.a / 255;

    obj->position = rectangle.position;
    // Set the scale based on the rectangle's width and height
    obj->scale = glm::vec2(rectangle.size.x / pixelScale_X, rectangle.size.y / pixelScale_Y);


    std::cout << "R: " << obj->color.r << "\t G: " << obj->color.g << "\t B:" << obj->color.b << std::endl;
    std::cout << "X Pos   : " << obj->scale.x << "\t Y Pos: " << obj->scale.y << std::endl;
    std::cout << "Width   : " << obj->scale.x << "\t Height: " << obj->scale.y << std::endl;
    std::cout << "screen W: " << Window::getWidth() << "\t\t H: " << Window::getHeight() << std::endl;

    obj->LoadShader("shaders/ui.vert", "shaders/ui.frag");

#define GL_ERROR_SILENT
    obj->Draw();
#undef GL_ERROR_SILENT

    delete obj;
}
