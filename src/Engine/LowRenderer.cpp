//
// Created by Bekir Gulestan on 12/30/24.
//


#include "Engine/HighRenderer.h"
#include "Engine/LowRenderer.h"
#include "Engine/Object/Object.h"
#include "Engine/Object/ObjectParser.h"
#include "Engine/TextureManager.h"
#include "Engine/Window.h"

#include "glad/glad.h"
#include "stb_truetype/stb_truetype.h"
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

    assert(objParsed.isTextured && (objParsed.texCoords.size() == objParsed.vertices.size()));

    LayoutStack stack = {
            VertexLayout(2),
            VertexLayout(2)
    };

    auto *vertices = new float[objParsed.vertices.size() * stack.getDimentionCount()];
    auto *indices = new unsigned int[objParsed.indices.size()];

    // move vertices and indices
    std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);

    int i = 0;
    for (int j = 0; j < objParsed.vertices.size(); ++j) {
        auto vert = objParsed.vertices[j];

        vertices[i++] = vert.x;
        vertices[i++] = vert.y;


        auto nextVert = objParsed.texCoords[j];
        vertices[i++] = nextVert.x;
        vertices[i++] = nextVert.y;

    }

    // define sizeof vertices
    unsigned int sizeofVertices = objParsed.vertices.size() * sizeof(float) * stack.getDimentionCount();
    // define sizeof indices
    unsigned int indicesSize = objParsed.indices.size() * sizeof(unsigned int);


    auto *obj = new Object(vertices, sizeofVertices, indices, indicesSize, stack);

//    std::cout << "R: " << rectangle.color.r << "\t G: " << rectangle.color.g << "\t B:" << rectangle.color.b
//              << std::endl;
//    std::cout << "X Pos   : " << rectangle.position.x << "\t\t Y Pos: " << rectangle.position.y << std::endl;
//    std::cout << "Width   : " << rectangle.size.x << "\t Height: " << rectangle.size.y << std::endl;
//    std::cout << "screen W: " << Window::getWidth() << "\t\t H: " << Window::getHeight() << std::endl;


    // Calculate pixel scaling based on the window's size and aspect ratio
    auto screenWidth = static_cast<float>(Window::getWidth());
    auto screenHeight = static_cast<float>(Window::getHeight());

    // TODO maybe use aspectRatio
    float aspectRatio = screenWidth / screenHeight;

    float camHalfWidth = HighRenderer::getCamera().getSize().x;
    float camWidth = camHalfWidth * 2;
    float camHalfHeight = HighRenderer::getCamera().getSize().y;
    float camHeight = camHalfHeight * 2;


    float sizeX = (rectangle.size.x / screenWidth) * camWidth;// half so use two times
    float sizeY = (rectangle.size.y / screenHeight) * camHeight; // half so use two times

    float posX = (rectangle.position.x / screenWidth) * camWidth - camHalfWidth + (0.5f * sizeX);
    float posY = camHalfHeight - (rectangle.position.y / screenHeight) * camHeight - (0.5f * sizeY);

    obj->position.x = posX;
    obj->position.y = posY;

    obj->scale.x = sizeX;
    obj->scale.y = sizeY;

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


void LowRenderer::DrawText(Text text) {
    auto objParsed = ObjectParser::LoadObject("square.obj");
    LayoutStack stack = {
            VertexLayout(2),
            VertexLayout(2)
    };

    auto *vertices = new float[objParsed.vertices.size() * stack.getDimentionCount()];
    auto *indices = new unsigned int[objParsed.indices.size()];

    // move vertices and indices
    std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);


    int i = 0;
    for (int j = 0; j < objParsed.vertices.size(); ++j) {
        auto vert = objParsed.vertices[j];

        vertices[i++] = vert.x;
        vertices[i++] = vert.y;


        auto nextVert = objParsed.texCoords[j];
        vertices[i++] = nextVert.x;
        vertices[i++] = nextVert.y;

    }

    // define sizeof vertices
    unsigned int sizeofVertices = objParsed.vertices.size() * sizeof(float) * stack.getDimentionCount();
    // define sizeof indices
    unsigned int indicesSize = objParsed.indices.size() * sizeof(unsigned int);


    auto *obj = new Object(vertices, sizeofVertices, indices, indicesSize, stack);

// Calculate pixel scaling based on the window's size and aspect ratio
    auto screenWidth = static_cast<float>(Window::getWidth());
    auto screenHeight = static_cast<float>(Window::getHeight());

    // TODO maybe use aspectRatio
    float aspectRatio = screenWidth / screenHeight;

    float camHalfWidth = HighRenderer::getCamera().getSize().x;
    float camWidth = camHalfWidth * 2;
    float camHalfHeight = HighRenderer::getCamera().getSize().y;
    float camHeight = camHalfHeight * 2;

    int sizeX = 1;
    int sizeY = 1;

    float posX = (text.position.x / screenWidth) * camWidth - camHalfWidth + (0.5f * sizeX);
    float posY = camHalfHeight - (text.position.y / screenHeight) * camHeight - (0.5f * sizeY);

    obj->position.x = posX;
    obj->position.y = posY;

    obj->scale.x = sizeX * 0.5;
    obj->scale.y = sizeY * 0.5;


    obj->color.r = text.color.r / 255;
    obj->color.g = text.color.g / 255;
    obj->color.b = text.color.b / 255;
    obj->color.a = text.color.a / 255;


    ///
    /// Text Rendering
    ///

    //TODO add text rendering

    ///
    ///
    ///


    obj->LoadShader("shaders/text.vert", "shaders/text.frag");
    obj->getShader()->Bind();


    auto texture = TextureManager::LoadTexture("textures/wall.png");
    texture->Bind(1);

    obj->getShader()->SetUniform1i("textureID", texture->slot());
    obj->Draw();

    delete obj;
}
