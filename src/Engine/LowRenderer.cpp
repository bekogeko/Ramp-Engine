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


    obj->position.x = -camHalfWidth + (sizeX / 2) + (rectangle.position.x / screenWidth) * camWidth;
    obj->position.y = camHalfHeight - (sizeY / 2) - (rectangle.position.y / screenWidth) * camHeight;

    obj->scale.x = sizeX;
    obj->scale.y = sizeY;

    obj->color.r = rectangle.color.r / 255;
    obj->color.g = rectangle.color.g / 255;
    obj->color.b = rectangle.color.b / 255;
    obj->color.a = rectangle.color.a / 255;


    obj->useShader("shaders/ui.vert", "shaders/ui.frag");

#define GL_ERROR_SILENT
    obj->Draw();
#undef GL_ERROR_SILENT

    delete obj;
}


void LowRenderer::DrawText(Text text) {
    auto objParsed = ObjectParser::LoadObject("square.obj");

    LayoutStack stack = {
            VertexLayout(2, false), // Position
            VertexLayout(2, false), // TexCoords
    };


    float vertices[] = {
            // position   texcoord
            // TODO change texCoords with glyph texCoords
            0.5, 0.5, objParsed.texCoords[0].x, objParsed.texCoords[0].y,
            0.5, -0.5, objParsed.texCoords[1].x, objParsed.texCoords[1].y,
            -0.5, -0.5, objParsed.texCoords[2].x, objParsed.texCoords[2].y,
            -0.5, 0.5, objParsed.texCoords[3].x, objParsed.texCoords[3].y
    };

    auto *indices = new unsigned int[objParsed.indices.size()];

    // move vertices and indices
    std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);

    auto vertexArray = std::make_unique<VertexArray>(vertices, 16 * sizeof(float), indices,
                                                     objParsed.indices.size() * sizeof(unsigned int), stack);

    vertexArray->Bind();
    auto fontTex = ResourceManager::LoadFont("fonts/JetBrainsMono-Regular.ttf", text.fontSize);

    glm::vec2 cursorPos = {0, 0};

    std::vector<std::array<float, 6>> instanceDatas;
    for (int i = 0; i < text.value.size(); ++i) {

        // get min_s, min_t, max_s, max_t
        auto texCoords = fontTex->getTextureCoords(text.value[i]);

        cursorPos.x += 1;
        instanceDatas.push_back({cursorPos.x, cursorPos.y, texCoords[0], texCoords[1], texCoords[2], texCoords[3]});

    }

    GLuint vbo_cursorPos;
    glGenBuffers(1, &vbo_cursorPos);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cursorPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * instanceDatas.size(), instanceDatas.data(), GL_STATIC_DRAW);

    // Set up the vertex attribute pointer for the instance data
    glEnableVertexAttribArray(2); // Assuming location 2 for instance data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an attribute per instance

    glEnableVertexAttribArray(3); // Assuming location 2 for instance data
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (sizeof(float) * 2));
    glVertexAttribDivisor(3, 1); // Tell OpenGL this is an attribute per instance


// Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // set uColor


    // position.x in [-hw,hw]
    // position.y is [-hh,hh]
    glm::vec2 position = {1, 1};
    glm::vec2 scale = {1, 1};
    float rotation = 0;


    // create model matrix from
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // TODO: scaling factor
    model = glm::scale(model, glm::vec3(scale, 1.0f));



    // get camera
    auto viewMat = HighRenderer::getCamera().getViewMatrix();
    auto projMat = HighRenderer::getCamera().getProjectionMatrix();

    // get shader
    auto shader = ResourceManager::LoadShader("shaders/text.vert", "shaders/text.frag");

    shader->Bind();

    // set Camera Matrix
    shader->SetUniformMat4("uProjection", &projMat[0][0]);
    shader->SetUniformMat4("uView", &viewMat[0][0]);
    shader->SetUniformMat4("uModel", &model[0][0]);
    shader->SetUniform4f("uColor", text.color.r, text.color.g, text.color.b, text.color.a);



    // setup font textureId
    fontTex->Bind(0);
    shader->SetUniform1i("textureID", fontTex->slot());


    vertexArray->Bind();

    vertexArray->DrawElementsInstanced(text.value.length());

    shader->Unbind();
    vertexArray->Unbind();


    delete[] indices;
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
