#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <openglErrorReporting.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Engine/Window.h"
#include "Engine/HighRenderer.h"

#include "Engine/Scene.h"
#include "Engine/LowRenderer.h"
#include "Engine/ResourceManager.h"
#include "Engine/Entity.h"
#include "Engine/Components/MeshComponent.h"

static void error_callback(int error, const char *description) {
    std::cerr << "Error: " << description << "\n";
}

int main() {
//    system("export MallocStackLogging=1;");
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
//    Physics::Init();

    HighRenderer::Init();
    Scene scene;

    {
        Entity square = scene.CreateEntity();
        square.AddComponent<MeshComponent>(MeshComponent::LoadFromFile("square.obj"));
    }

    // enable alpha blending
    // 0 means opaque
    // 1 means transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);
    // disable vSync
    glfwSwapInterval(0);

    while (!window.shouldClose()) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


#if REMOVE_IMGUI == 0
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

//        Physics::Update();

        HighRenderer::Update(LowRenderer::getDeltaTime());
        HighRenderer::Draw();

        scene.Draw();

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
    HighRenderer::Destroy();
    ResourceManager::Destroy();


    std::cout << "Application terminated successfully\n";
//    system("leaks Ray-Game --list");

    return 0;
}