//
// Created by Bekir Gulestan on 12/30/24.
//


#include "Engine/HighRenderer.h"
#include "Engine/LowRenderer.h"
#include "Engine/Object/Object.h"
#include "Engine/Object/ObjectParser.h"
#include "Engine/Window.h"

#include "glad/glad.h"
#include "stb_truetype/stb_truetype.h"
#include "Engine/ResourceManager.h"
#include <GLFW/glfw3.h>


double LowRenderer::currentTime = 0.0f; // Initialization of lastTime
double LowRenderer::lastTime = 0.0f; // Initialization of lastTime

float LowRenderer::getDeltaTime() {
    auto deltaTime = (float) (currentTime - lastTime);
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


    std::cout << ":rect Position: " << rectangle.position.x << "\t" << rectangle.position.y << std::endl;
    std::cout << ":rect Size: " << rectangle.size.x << "\t" << rectangle.size.y << std::endl;


    obj->position.x = -camHalfWidth + (sizeX / 2) + (rectangle.position.x / screenWidth) * camWidth;
    obj->position.y = camHalfHeight - (sizeY / 2) - (rectangle.position.y / screenWidth) * camHeight;

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
            VertexLayout(2), // Position
            VertexLayout(2) // TexCoords
    };

    auto *vertices = new float[objParsed.vertices.size() * stack.getDimentionCount()];
    auto *indices = new unsigned int[objParsed.indices.size()];

    // move vertices and indices
    std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);


//    auto fontTex = ResourceManager::LoadFont("fonts/DefaultSansRegular.ttf", text.fontSize);
    auto fontTex = ResourceManager::LoadFont("fonts/JetBrainsMono-Regular.ttf", text.fontSize);

    glm::vec2 cursorPosition = {0, text.fontSize};
    for (int i = 0; i < text.value.length(); ++i) {
        auto texCoords = fontTex->getTextureCoords(text.value[i]);
        auto glyph = fontTex->getChar(text.value[i]);

        int vIndex = 0;
        for (int j = 0; j < objParsed.vertices.size(); ++j) {
            auto vert = objParsed.vertices[j];

            // 32 px is 1 unit
            vertices[vIndex++] = vert.x * ((float) text.fontSize / 32.0f) * (glyph.size.x / glyph.size.y);
            vertices[vIndex++] = vert.y * ((float) text.fontSize / 32.0f);

            auto texCoord = texCoords[j];
            vertices[vIndex++] = texCoord.x;
            vertices[vIndex++] = texCoord.y;

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
        float camHalfHeight = HighRenderer::getCamera().getSize().y;

        float sizeX = (glyph.size.x / text.fontSize);
        float sizeY = (glyph.size.y / text.fontSize);


        auto pixelPosition =
                text.position + cursorPosition + glm::vec2(glyph.bearing.x, (glyph.size.y / 2) + glyph.bearing.y);

        auto transformedPos =
                glm::vec2(camHalfWidth, camHalfHeight) *
                ((glm::vec2(2, 2) * (pixelPosition / glm::vec2(screenWidth, screenHeight))) - glm::vec2(1, 1));

        // pixelPosition = [0,640]
        // pixelPosition / screenSize   = [0,1]
        // *2 = [0,2]
        // -1 = [-1,1]
        // * axisSize = [-4,4]


        obj->position.x = transformedPos.x;
        obj->position.y -= transformedPos.y;


        obj->scale.x = sizeX * 0.5;
        obj->scale.y = sizeY * 0.5;


        obj->color.r = text.color.r / 255;
        obj->color.g = text.color.g / 255;
        obj->color.b = text.color.b / 255;
        obj->color.a = text.color.a / 255;

        // TODO: maybe manage slotNumber
        fontTex->Bind(1);
        obj->LoadShader("shaders/text.vert", "shaders/text.frag");
        obj->getShader()->Bind();

        obj->getShader()->SetUniform1i("textureID", fontTex->slot());
        obj->Draw();

        delete obj;

        cursorPosition.x += (float) (glyph.advance) - (glyph.size.x / 2) + 2 * (glyph.bearing.x);
    }


}

float LowRenderer::getFPS() {
    double deltaTime = (currentTime - lastTime);
    return (float) (1.0 / deltaTime);
}

void LowRenderer::swapTime() {
    lastTime = currentTime;
    currentTime = glfwGetTime();
}

void LowRenderer::updateTime() {

}
