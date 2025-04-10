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
std::map<uint32_t, Rectangle> LowRenderer::m_rectBatch;
std::map<uint32_t, Text> LowRenderer::m_textBatch;
std::map<uint32_t, Text> LowRenderer::m_prevTextBatch;
std::map<uint32_t, unsigned int> LowRenderer::m_textVBOs;


float LowRenderer::getDeltaTime() {
    auto deltaTime = (float) (currentTime - lastTime);
    return deltaTime;
}

void LowRenderer::DrawRectangle(Rectangle rectangle) {

    LayoutStack stack = {
            VertexLayout(2, false), // Position
//            VertexLayout(2, false), // TexCoords
    };


    float vertices[] = {
            // position
            0.5, 0.5,
            0.5, -0.5,
            -0.5, -0.5,
            -0.5, 0.5
    };

    int vertSize = 0;
    int indicesSize = 0;
    unsigned int *indices = nullptr;
    {
        auto objParsed = *ResourceManager::LoadObject("square.obj").lock();
        assert(objParsed.isTextured && (objParsed.texCoords.size() == objParsed.vertices.size()));

        indices = new unsigned int[objParsed.indices.size()];

        // move vertices and indices
        std::copy(objParsed.indices.begin(), objParsed.indices.end(), indices);
        vertSize = objParsed.vertices.size();
        indicesSize = objParsed.indices.size();
    }

    VertexArray vertexArray(indices, indicesSize * sizeof(unsigned int));
    vertexArray.AddBuffer(vertices, (vertSize * 2) * sizeof(float), stack);

    auto camSize = HighRenderer::getCamera().getSize();
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());


    // position.x in [-hw,hw]
    // position.y is [-hh,hh]
    glm::vec2 size = {(rectangle.size.x * 2 * camSize.x) / screen.x,
                      (rectangle.size.y * 2 * camSize.y) / screen.y};
    glm::vec2 position = {-camSize.x + (size.x / 2) + ((rectangle.position.x * 2 * camSize.x) / screen.x),
                          camSize.y - (size.y / 2) - ((rectangle.position.y * 2 * camSize.y) / screen.y)};

    float rotation = 0;


    // create model matrix from
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // TODO: scaling factor
    model = glm::scale(model, glm::vec3(size, 1.0f));



    // get camera
    auto viewMat = HighRenderer::getCamera().getViewMatrix();
    auto projMat = HighRenderer::getCamera().getProjectionMatrix();


    // get shader
    auto shader_ptr = ResourceManager::LoadShader("shaders/ui.vert", "shaders/ui.frag");
    {
        auto shader = shader_ptr.lock();
        shader->Bind();

        // set Camera Matrix
        shader->SetUniformMat4("uProjection", &projMat[0][0]);
        shader->SetUniformMat4("uView", &viewMat[0][0]);
        shader->SetUniformMat4("uModel", &model[0][0]);
        shader->SetUniform4f("uColor", rectangle.color.r, rectangle.color.g, rectangle.color.b,
                             rectangle.color.a);
    }

    vertexArray.Bind();
    vertexArray.DrawElements();
    ShaderProgram::Unbind();
    VertexArray::Unbind();

    delete[] indices;
}


void LowRenderer::DrawText(uint32_t id, Text text) {

    // warning maybe focus more on objParsed's lifetime
//    auto objParsed = *ResourceManager::LoadObject("square.obj").lock();
//    assert(objParsed.isTextured && (objParsed.texCoords.size() == objParsed.vertices.size()));

    LayoutStack stack = {
            VertexLayout(2, false), // Position
            VertexLayout(2, false), // TexCoords
    };

    float vertices[] = {
            // position   texcoord
            0.5, 0.5, 1.0, 0,
            0.5, -0.5, 1.0, 1.0,
            -0.5, -0.5, 0, 1.0,
            -0.5, 0.5, 0, 0
    };

    int vertSize = 16;
    int indicesSize = 6;
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    VertexArray vertexArray(
            indices,
            indicesSize * sizeof(unsigned int));

    vertexArray.AddBuffer(vertices, vertSize * sizeof(float), stack);
    vertexArray.Bind();

    //fixme
    auto fontTex = ResourceManager::LoadFontById(text.fontId).lock();
    assert(fontTex->getHashId() != 0);


    glm::vec2 initialCursor = {0.0f, .5f};
    //activeCursor
    glm::vec2 cursorPos = initialCursor;

    // cursorPos,   vec2
    // texCoord,    vec4
    // size,        vec2
    std::vector<std::array<float, 8>> instanceDatas;
    int emptyChars = 0;
    for (int i = 0; i < text.value.size(); ++i) {
        // get min_s, min_t, max_s, max_t
        auto glyph = fontTex->getChar(text.value[i]);
        auto texCoords = fontTex->getTextureCoords(text.value[i]);


        auto temp = cursorPos.y;
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

    // warning VBO api should be used
//
//    LayoutStack vboCursorStack = {
//            VertexLayout(2, true), // Position
//            VertexLayout(4, true), // TexCoords
//            VertexLayout(2, true) // size
//    };
//
//    std::vector<float> flattenedInstanceData;
//    for (const auto &instance: instanceDatas) {
//        flattenedInstanceData.insert(flattenedInstanceData.end(), instance.begin(), instance.end());
//    }
//
//    vertexArray.AddBuffer(flattenedInstanceData.data(), flattenedInstanceData.size() * sizeof(float), stack);

//    VertexBuffer vboCursorPos;

    GLuint vbo_cursorPos;
    // if vbo is already created and upload
    if (m_textVBOs.find(id) == m_textVBOs.end())
        glGenBuffers(1, &vbo_cursorPos);
    else
        vbo_cursorPos = m_textVBOs[id];

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cursorPos);

    if (m_textVBOs.find(id) == m_textVBOs.end())
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

    VertexArray::Unbind();

    // set
    m_textVBOs[id] = vbo_cursorPos;


    auto camSize = HighRenderer::getCamera().getSize();
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());

    auto size = glm::vec2((float(text.fontSize) * 2 * camSize.x) / (screen.x),
                          (float(text.fontSize) * 2 * camSize.y) / (screen.y));

    // position.x in [-hw,hw]
    // position.y is [-hh,hh]
    glm::vec2 position = {-camSize.x + (size.x / 2.0f) + (text.position.x * 2 * camSize.x) / screen.x,
                          camSize.y - (size.y / 2.0f) - (text.position.y * 2 * camSize.y) / screen.y};
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

    // get shaders
    auto shader_ptr = ResourceManager::LoadShader("shaders/text.vert", "shaders/text.frag");
    {
        auto shader = shader_ptr.lock();
        shader->Bind();
        // set Camera Matrix
        shader->SetUniformMat4("uProjection", &projMat[0][0]);
        shader->SetUniformMat4("uView", &viewMat[0][0]);
        shader->SetUniformMat4("uModel", &model[0][0]);
        shader->SetUniform4f("uColor", text.color.r, text.color.g, text.color.b, text.color.a);



        // setup font textureId
        fontTex->Bind(0);
        shader->SetUniform1i("textureID", fontTex->slot());
    }


    vertexArray.Bind();

//    std::cout << "Empty Chars :" << emptyChars << std::endl;
//    std::cout << "Length Chars :" << text.value.length() << std::endl;
    vertexArray.DrawElementsInstanced(text.value.length() - emptyChars);

    ShaderProgram::Unbind();
    VertexArray::Unbind();
}

void LowRenderer::AddText(uint32_t id, const Text &text) {
    if (m_textBatch.find(id) != m_textBatch.end()) {
        return;
    }
    m_textBatch[id] = text;
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

void LowRenderer::AddRectangle(uint32_t id, const Rectangle &rectangle) {
    // check if the rectangle is already in the batch
    if (m_rectBatch.find(id) != m_rectBatch.end()) {
        // if it is, then check if the rectangle is the same
        if (m_rectBatch[id].position == rectangle.position &&
            m_rectBatch[id].size == rectangle.size &&
            m_rectBatch[id].color == rectangle.color) {
            // if it is the same, then do nothing
            return;
        } else {
            // if it is not the same, then update the rectangle
            m_rectBatch[id] = rectangle;
            return;
        }
    }

    std::cout << "rectangle added to a batch with id: " << id << std::endl;
    m_rectBatch[id] = rectangle;
}

void LowRenderer::DrawRectangleBatched() {

    LayoutStack stack = {
            VertexLayout(2, false), // Position
    };


    float vertices[] = {
            // position
            0.5, 0.5,
            0.5, -0.5,
            -0.5, -0.5,
            -0.5, 0.5
    };


    int vertSize = 4;
    int indicesSize = 6;
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    VertexArray vertexArray(
            indices,
            indicesSize * sizeof(unsigned int)
    );
    vertexArray.AddBuffer(vertices, vertSize * 2 * sizeof(float), stack);
    vertexArray.Bind();

    // position vec2
    // size     vec2
    // color    vec4
    std::vector<std::array<float, 8>> instanceData;

    for (auto &[id, rectangle]: m_rectBatch) {
        auto camSize = HighRenderer::getCamera().getSize();
        auto screen = glm::vec2(Window::getWidth(), Window::getHeight());

        // position.x in [-hw,hw]
        // position.y is [-hh,hh]
        glm::vec2 size = {(rectangle.size.x * 2 * camSize.x) / screen.x,
                          (rectangle.size.y * 2 * camSize.y) / screen.y};
        glm::vec2 position = {-camSize.x + (size.x / 2) + ((rectangle.position.x * 2 * camSize.x) / screen.x),
                              camSize.y - (size.y / 2) - ((rectangle.position.y * 2 * camSize.y) / screen.y)};

        glm::vec4 color = {
                rectangle.color.r,
                rectangle.color.g,
                rectangle.color.b,
                rectangle.color.a,
        };

        instanceData.push_back({
                                       position.x, position.y,
                                       size.x, size.y,
                                       color.r, color.g, color.b, color.a
                               });


    }


    GLuint vbo_cursorPos;
    glGenBuffers(1, &vbo_cursorPos);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cursorPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * instanceData.size(), instanceData.data(), GL_STATIC_DRAW);

    // Set up the vertex attribute pointer for the instance data
    glEnableVertexAttribArray(1); // Assuming location 2 for instance data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glVertexAttribDivisor(1, 1); // Tell OpenGL this is an attribute per instance

    glEnableVertexAttribArray(2); // Assuming location 2 for instance data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (sizeof(float) * 2));
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an attribute per instance

    glEnableVertexAttribArray(3); // Assuming location 2 for instance data
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (sizeof(float) * 2));
    glVertexAttribDivisor(3, 1); // Tell OpenGL this is an attribute per instance

    glBindBuffer(GL_ARRAY_BUFFER, 0);

// Delete the buffer object to prevent memory leak

//
//    LayoutStack instanceStack = {
//            VertexLayout(2, true),
//            VertexLayout(2, true),
//            VertexLayout(4, true),
//    };
//
//    std::vector<float> flattenedInstanceData;
//    for (const auto &instance: instanceData) {
//        flattenedInstanceData.insert(flattenedInstanceData.end(), instance.begin(), instance.end());
//    }
//    vertexArray.AddBuffer(flattenedInstanceData.data(), flattenedInstanceData.size() * sizeof(float),
//                          instanceStack);


    auto camSize = HighRenderer::getCamera().getSize();
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());

    float rotation = 0;
    glm::vec2 size = {1, 1};
    glm::vec2 position = {-camSize.x + (size.x / 2),
                          camSize.y - (size.y / 2)};

    // create model matrix from
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // TODO: scaling factor
    model = glm::scale(model, glm::vec3(size, 1.0f));



    // get camera
    auto viewMat = HighRenderer::getCamera().getViewMatrix();
    auto projMat = HighRenderer::getCamera().getProjectionMatrix();


    // get shader
    auto shader_ptr = ResourceManager::LoadShader("shaders/ui_batched.vert", "shaders/ui_batched.frag");
    {
        auto shader = shader_ptr.lock();
        shader->Bind();

        // set Camera Matrix
        shader->SetUniformMat4("uProjection", &projMat[0][0]);
        shader->SetUniformMat4("uView", &viewMat[0][0]);
        shader->SetUniformMat4("uModel", &model[0][0]);
    }

    vertexArray.Bind();
    vertexArray.DrawElementsInstanced(m_rectBatch.size());

    ShaderProgram::Unbind();
    VertexArray::Unbind();

    // delete the not refreshed batch elements
    // which means they are not in the current batch
    // but they are in the previous batch

    glDeleteBuffers(1, &vbo_cursorPos);
}

void LowRenderer::DrawTextBatched() {


    // go through prevText batch
    for (const auto &[prevId, prevText]: m_prevTextBatch) {
        // if this prev_id is NOT on the active batch
        if (m_textBatch.find(prevId) == m_textBatch.end()) {
            // then delete the vbo'
            glDeleteBuffers(1, &m_textVBOs[prevId]);
            // delete from the map
            // multiple ids can share the same vbo
            // if one vbo is deleted then all of them should be deleted
            m_textVBOs.erase(prevId);
        } else {
            // if this prev_id is on the active batch
            // but the new batch contains different text
            // then delete the vbo
            if (m_textBatch[prevId].value != prevText.value) {
                glDeleteBuffers(1, &m_textVBOs[prevId]);
                // delete from the map
                // multiple ids can share the same vbo
                // if one vbo is deleted then all of them should be deleted
                m_textVBOs.erase(prevId);
            }


        }
    }


    for (const auto &[id, text]: m_textBatch) {
        DrawText(id, text);
    }

    // now batch is drawn swap the batch
    m_prevTextBatch = m_textBatch;
    m_textBatch.clear();
    //
    std::cout << "Text batch cleared\n";
}
