//
// Created by Bekir Gulestan on 12/30/24.
//


#include "Engine/HighRenderer.h"
#include "Engine/LowRenderer.h"
#include "Engine/Window.h"

#include "glad/glad.h"
#include "Engine/ResourceManager.h"
#include <GLFW/glfw3.h>

#include "Engine/Renderer/CircleGenerator.h"
#include "Engine/Renderer/RoundedRectangleGenerator.h"


double LowRenderer::currentTime = 0.0f; // Initialization of lastTime
double LowRenderer::lastTime = 0.0f; // Initialization of lastTime
std::map<uint32_t, Rectangle> LowRenderer::m_rectBatch;
std::map<uint32_t, Text> LowRenderer::m_textBatch;
std::map<uint32_t, Text> LowRenderer::m_previousTextBatch;
std::map<uint32_t, std::unique_ptr<VertexArray>> LowRenderer::m_vertexArrayBatch;


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


    int vertSize = 8;
    int indicesSize = 6;
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    VertexArray vertexArray(indices, indicesSize * sizeof(unsigned int));
    vertexArray.AddBuffer(vertices, vertSize * sizeof(float), stack);

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
}


void LowRenderer::DrawText(uint32_t id, Text text) {


    //fixme
    auto fontTex = ResourceManager::LoadFontById(text.fontId).lock();
    assert(fontTex->getHashId() != 0);


    // if there is no vertexArray for this id
    if (m_vertexArrayBatch.find(id) == m_vertexArrayBatch.end()) {

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

        auto *vertexArray = new VertexArray(
                indices,
                indicesSize * sizeof(unsigned int));

        vertexArray->AddBuffer(vertices, vertSize * sizeof(float), stack);
        vertexArray->Bind();


        glm::vec2 initialCursor = {0.0f, .5f};
        //activeCursor
        glm::vec2 cursorPos = initialCursor;

        // cursorPos,   vec2
        // texCoord,    vec4
        // size,        vec2
        std::vector<float> instanceDatas;
        instanceDatas.reserve(text.value.size() * 8);

        for (int i = 0; i < text.value.size(); ++i) {
            // get min_s, min_t, max_s, max_t
            auto glyph = fontTex->getChar(text.value[i]);
            auto texCoords = fontTex->getTextureCoords(text.value[i]);


            auto temp = cursorPos.y;
            cursorPos.y -= ((glyph.size.y / (2.0f * text.fontSize)) + (glyph.bearing.y / text.fontSize));
            cursorPos.y--;

            instanceDatas.push_back(cursorPos.x);
            instanceDatas.push_back(cursorPos.y);
            instanceDatas.push_back(texCoords[0]);
            instanceDatas.push_back(texCoords[1]);
            instanceDatas.push_back(texCoords[2]);
            instanceDatas.push_back(texCoords[3]);
            instanceDatas.push_back(glyph.size.x / text.fontSize);
            instanceDatas.push_back(glyph.size.y / text.fontSize);

            cursorPos.x += (glyph.advance / text.fontSize);
            cursorPos.y = temp;
        }

        LayoutStack vboCursorStack = {
                VertexLayout(2, true), // Position
                VertexLayout(4, true), // TexCoords
                VertexLayout(2, true) // size
        };


        vertexArray->AddBuffer(instanceDatas.data(), instanceDatas.size() * sizeof(float), vboCursorStack);
        m_vertexArrayBatch[id] = std::unique_ptr<VertexArray>(vertexArray);
    }


    m_vertexArrayBatch[id]->Bind();

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


    m_vertexArrayBatch[id]->Bind();

//    std::cout << "Empty Chars :" << emptyChars << std::endl;
//    std::cout << "Length Chars :" << text.value.length() << std::endl;
    m_vertexArrayBatch[id]->DrawElementsInstanced(text.value.length());

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
            m_rectBatch[id].color == rectangle.color &&
            m_rectBatch[id].cornerRadius == rectangle.cornerRadius) {
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


    int vertSize = 8;
    int indicesSize = 6;
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    VertexArray vertexArray(
            indices,
            indicesSize * sizeof(unsigned int)
    );
    vertexArray.AddBuffer(vertices, vertSize * sizeof(float), stack);
    vertexArray.Bind();

    // position vec2
    // size     vec2
    // color    vec4
    std::vector<float> instanceData;

    instanceData.reserve(m_rectBatch.size() * 8);

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

        instanceData.push_back(position.x);
        instanceData.push_back(position.y);
        instanceData.push_back(size.x);
        instanceData.push_back(size.y);
        instanceData.push_back(color.r);
        instanceData.push_back(color.g);
        instanceData.push_back(color.b);
        instanceData.push_back(color.a);
    }

    LayoutStack instanceStack = {
            VertexLayout(2, true),
            VertexLayout(2, true),
            VertexLayout(4, true),
    };
    vertexArray.AddBuffer(instanceData.data(), instanceData.size() * sizeof(float),
                          instanceStack);


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
}

void LowRenderer::DrawRoundedRectangle(uint32_t id, Rectangle rectangle) {



    // Build rounded-rect boundary in local space [-0.5, 0.5] and render via VertexArray
    float rx = std::min(0.5f, rectangle.cornerRadius / std::max(1.0f, rectangle.size.x));
    float ry = std::min(0.5f, rectangle.cornerRadius / std::max(1.0f, rectangle.size.y));

    // Use the rounded-rectangle generator to derive corner centers (in [0,1]) then shift to [-0.5,0.5]
    uint32_t segmentsPerCorner = 16; // tesselation per corner
    RoundedRectangleGenerator rr({1.0f, 1.0f}, rectangle.cornerRadius/rectangle.size.x, segmentsPerCorner);

    std::vector<glm::vec2> verts;
    verts.reserve(segmentsPerCorner * 4);
    for (uint32_t i = 0; i < segmentsPerCorner; ++i) {
        glm::vec2 corner = rr.getPoint(i) + glm::vec2(-0.5f, -0.5f);
        verts.push_back(corner);
    }

    // Indices for triangle fan
    std::vector<unsigned int> indices;
    unsigned int boundaryCount = static_cast<unsigned int>(verts.size()-1);
    indices.reserve(boundaryCount * 3);
    indices.push_back(0);
    for (unsigned int i = 1; i < boundaryCount; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Upload to VAO/VBO and draw
    LayoutStack stack = { VertexLayout(2, false) }; // Position only

    std::vector<float> vertexFloats;
    vertexFloats.reserve(verts.size() * 2);
    for (const auto& v : verts) {
        vertexFloats.push_back(v.x);
        vertexFloats.push_back(v.y);
    }

    VertexArray vertexArray(indices.data(), static_cast<unsigned int>(indices.size() * sizeof(unsigned int)));
    vertexArray.AddBuffer(vertexFloats.data(), static_cast<unsigned int>(vertexFloats.size() * sizeof(float)), stack);

    // Build transform (same scheme as other UI rects)
    auto camSize = HighRenderer::getCamera().getSize();
    auto screen  = glm::vec2(Window::getWidth(), Window::getHeight());

    glm::vec2 size = {
        (rectangle.size.x * 2.0f * camSize.x) / screen.x,
        (rectangle.size.y * 2.0f * camSize.y) / screen.y
    };

    glm::vec2 position = {
        -camSize.x + (size.x * 0.5f) + ((rectangle.position.x * 2.0f * camSize.x) / screen.x),
         camSize.y - (size.y * 0.5f) - ((rectangle.position.y * 2.0f * camSize.y) / screen.y)
    };

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    auto viewMat = HighRenderer::getCamera().getViewMatrix();
    auto projMat = HighRenderer::getCamera().getProjectionMatrix();

    auto shader_ptr = ResourceManager::LoadShader("shaders/ui.vert", "shaders/ui.frag");
    {
        auto shader = shader_ptr.lock();
        shader->Bind();
        shader->SetUniformMat4("uProjection", &projMat[0][0]);
        shader->SetUniformMat4("uView", &viewMat[0][0]);
        shader->SetUniformMat4("uModel", &model[0][0]);
        shader->SetUniform4f("uColor", rectangle.color.r, rectangle.color.g, rectangle.color.b, rectangle.color.a);
    }

    vertexArray.Bind();
    vertexArray.DrawElements(VertexArray::DrawMode::TRIANGLE_FAN);
    ShaderProgram::Unbind();
    VertexArray::Unbind();

}

void LowRenderer::DrawTextBatched() {

    // go through all the previous text
    for (const auto &[id, text]: m_previousTextBatch) {
        // if the text is not in the current batch
        if (m_textBatch.find(id) == m_textBatch.end()) {
            // remove the text from the batch
            m_vertexArrayBatch.erase(id);
        } else {
            // if the text is in the current batch
            // check if the text is the same
            if (m_textBatch[id].value != text.value) {
                m_vertexArrayBatch.erase(id);
            }
        }
    }

    for (const auto &[id, text]: m_textBatch) {
        DrawText(id, text);
    }

    // copy the current text batch to the previous text batch
    m_previousTextBatch.clear();
    m_previousTextBatch = m_textBatch;
    m_textBatch.clear();
    //
    std::cout << "Text batch cleared\n";
}

void LowRenderer::DrawTextWorld(Text text) {
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
    std::vector<float> instanceDatas;
//    instanceDatas.reserve(text.value.size() * 8);
    for (int i = 0; i < text.value.size(); ++i) {
        // get min_s, min_t, max_s, max_t
        auto glyph = fontTex->getChar(text.value[i]);
        auto texCoords = fontTex->getTextureCoords(text.value[i]);


        auto temp = cursorPos.y;
        cursorPos.y -= ((glyph.size.y / (2.0f * text.fontSize)) + (glyph.bearing.y / text.fontSize));
        cursorPos.y--;

        instanceDatas.push_back(cursorPos.x);
        instanceDatas.push_back(cursorPos.y);
        instanceDatas.push_back(texCoords[0]);
        instanceDatas.push_back(texCoords[1]);
        instanceDatas.push_back(texCoords[2]);
        instanceDatas.push_back(texCoords[3]);
        instanceDatas.push_back(glyph.size.x / text.fontSize);
        instanceDatas.push_back(glyph.size.y / text.fontSize);

        cursorPos.x += (glyph.advance / text.fontSize);
        cursorPos.y = temp;
    }

    // warning VBO api should be used
    LayoutStack vboCursorStack = {
            VertexLayout(2, true), // Position
            VertexLayout(4, true), // TexCoords
            VertexLayout(2, true) // size
    };


    vertexArray.AddBuffer(instanceDatas.data(), instanceDatas.size() * sizeof(float), vboCursorStack);

    auto camSize = HighRenderer::getCamera().getSize();
    auto camPos = HighRenderer::getCamera().position;
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());

    auto size = glm::vec2((float(text.fontSize) * 2 * camSize.x) / (screen.x),
                          (float(text.fontSize) * 2 * camSize.y) / (screen.y));

    // position.x is in world space
    // position.y is in world space
    glm::vec2 position = {
            text.position.x,
            text.position.y
    };
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
    auto shader_ptr = ResourceManager::LoadShader("shaders/textWorld.vert", "shaders/text.frag");
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
    vertexArray.DrawElementsInstanced(text.value.length());

    ShaderProgram::Unbind();
    VertexArray::Unbind();

}

void LowRenderer::DrawRectangleWorld(Rectangle rectangle) {

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

    int vertSize = 8;
    int indicesSize = 6;
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    VertexArray vertexArray(indices, indicesSize * sizeof(unsigned int));
    vertexArray.AddBuffer(vertices, vertSize * sizeof(float), stack);

    auto camSize = HighRenderer::getCamera().getSize();
    auto screen = glm::vec2(Window::getWidth(), Window::getHeight());


    // position.x in [-hw,hw]
    // position.y is [-hh,hh]
    glm::vec2 size = {rectangle.size.x,
                      rectangle.size.y};
    glm::vec2 position = {rectangle.position.x, rectangle.position.y};

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
    auto shader_ptr = ResourceManager::LoadShader("shaders/default.vert", "shaders/ui.frag");
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
}
