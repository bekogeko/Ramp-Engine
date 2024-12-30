#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <openglErrorReporting.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Engine/Window.h"
#include "Engine/HighRenderer.h"

#include "Player.h"

static void error_callback(int error, const char *description) {
    std::cout << "Error: " << description << "\n";
}

int main() {
    std::cout << "Starting application...\n";

    glfwSetErrorCallback(error_callback);

    Window window(640, 480, "Simple example");

    // Warning: something is wrong with this function
    enableReportGlErrors();


#if REMOVE_IMGUI == 0
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window.getRawWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "ImGui initialized successfully\n";
#endif


    // Rectangle vertices
    float vertices[] = {
            0.5f, 0.5f, // top right
            0.5f, -0.5f, // bottom right
            -0.5f, -0.5f, // bottom left
            -0.5f, 0.5f // top left
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
    };

    // register object transfer to high renderer
    unsigned int id = HighRenderer::RegisterObject(vertices, sizeof(vertices), indices, sizeof(indices));
    // get objects by id
    auto objId = HighRenderer::getById(id);

    objId->position.x -= 0.5;
    objId->color.r = 1;

    objId->attachComponent<Player>();


    std::shared_ptr<Object> obj = std::make_shared<Object>(vertices, sizeof(vertices), indices, sizeof(indices));
    // create obj2 with different color and position
    std::shared_ptr<Object> obj2 = std::make_shared<Object>(vertices, sizeof(vertices), indices, sizeof(indices));

    obj2->position = {1, 1};
    obj2->color = {1.0, 0.55, 0.2};

    HighRenderer::RegisterObject(obj);
    HighRenderer::RegisterObject(obj2);

    while (!window.shouldClose()) {


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


#if REMOVE_IMGUI == 0
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        HighRenderer::Draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
#endif
        window.swapBuffers();
        window.pollInputs();
    }

#if REMOVE_IMGUI == 0
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif


    //clean up
    std::cout << "Cleaning up...\n";
    HighRenderer::FreeAll();

    std::cout << "Application terminated successfully\n";

    return 0;
}