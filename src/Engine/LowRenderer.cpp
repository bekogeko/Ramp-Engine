//
// Created by Bekir Gulestan on 12/30/24.
//


#include "Engine/HighRenderer.h"
#include "Engine/LowRenderer.h"
#include "Engine/Object/Object.h"
#include "Engine/Window.h"

#include "glad/glad.h"
#include "Engine/ResourceManager.h"
#include <GLFW/glfw3.h>


double LowRenderer::currentTime = 0.0f; // Initialization of lastTime
double LowRenderer::lastTime = 0.0f; // Initialization of lastTime

float LowRenderer::getDeltaTime() {
    auto deltaTime = (float) (currentTime - lastTime);
    return deltaTime;
}

void LowRenderer::DrawRectangle(Rectangle rectangle) {
    auto objParsed = ResourceManager::LoadObject("square.obj");

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
    obj->position.y = camHalfHeight - (sizeY / 2) - (rectangle.position.y / screenHeight) * camHeight;

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
    auto objParsed = ResourceManager::LoadObject("square.obj");

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


    // warning: Fixme: this should not be the way
    glm::vec2 initialCursor = {0.5, -0.5};
    //activeCursor
    glm::vec2 cursorPos = initialCursor;

    std::vector<std::array<float, 8>> instanceDatas;
    int emptyChars = 0;
    for (int i = 0; i < text.value.size(); ++i) {
        if (text.value[i] == ' ') {
            // fixme: questionable api use/abuse?
            auto glyph = fontTex->getChar(text.value[i]);

            cursorPos.x += glyph.advance / text.fontSize;
            emptyChars++;
            continue;
        }
        if (text.value[i] == '\n') {
            emptyChars++;
            auto glyph = fontTex->getChar(text.value[i]);

            // TODO add optional parameter to configure lineHeight
            float lineSpacing = 0;
            cursorPos.y -= (lineSpacing / text.fontSize) + 1.5f; // default is 1.5 em space
            cursorPos.x = initialCursor.x;
            continue;
        }




        // get min_s, min_t, max_s, max_t
        auto glyph = fontTex->getChar(text.value[i]);
        auto texCoords = fontTex->getTextureCoords(text.value[i]);


        auto temp = cursorPos.y;
//        cursorPos.y -= ((glyph.size.y / (2.0f * text.fontSize)) + (glyph.bearing.y / text.fontSize));
        cursorPos.y -= ((glyph.size.y / (2.0f * text.fontSize)) + (glyph.bearing.y / text.fontSize));
        cursorPos.y--;


        instanceDatas.push_back({
                                        cursorPos.x, cursorPos.y,
                                        texCoords[0], texCoords[1], texCoords[2], texCoords[3],
                                        glyph.size.x / text.fontSize, glyph.size.y / text.fontSize
                                });

        cursorPos.x += (glyph.advance / text.fontSize);
        cursorPos.y = temp;
    }

    GLuint vbo_cursorPos;
    glGenBuffers(1, &vbo_cursorPos);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cursorPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * instanceDatas.size(), instanceDatas.data(), GL_STATIC_DRAW);

    // Set up the vertex attribute pointer for the instance data
    glEnableVertexAttribArray(2); // Assuming location 2 for instance data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an attribute per instance

    glEnableVertexAttribArray(3); // Assuming location 2 for instance data
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (sizeof(float) * 2));
    glVertexAttribDivisor(3, 1); // Tell OpenGL this is an attribute per instance

    glEnableVertexAttribArray(4); // Assuming location 2 for instance data
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (sizeof(float) * 6));
    glVertexAttribDivisor(4, 1); // Tell OpenGL this is an attribute per instance



    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // set uColor

    auto camSize = HighRenderer::getCamera().getSize();
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());

    auto size = glm::vec2((float(text.fontSize) * 2 * camSize.x) / (screen.x),
                          (float(text.fontSize) * 2 * camSize.y) / (screen.y));

    // position.x in [-hw,hw]
    // position.y is [-hh,hh]
    glm::vec2 position = {-camSize.x + (size.x / 2.0f) + (text.position.x / screen.x) * 2 * camSize.x,
                          camSize.y - (size.y / 2.0f) - (text.position.y / screen.y) * 2 * camSize.y};
    glm::vec2 scale = {size.x, size.y};
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

//    std::cout << "Empty Chars :" << emptyChars << std::endl;
//    std::cout << "Length Chars :" << text.value.length() << std::endl;
    vertexArray->DrawElementsInstanced(text.value.length() - emptyChars);

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
