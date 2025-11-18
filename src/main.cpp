// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <iostream>
// #include <openglErrorReporting.h>
//
// #include "imgui.h"
// #include "backends/imgui_impl_glfw.h"
// #include "backends/imgui_impl_opengl3.h"
//
// #include "Engine/Window.h"
// #include "Engine/HighRenderer.h"
//
// #include "Engine/Scene.h"
// #include "Engine/LowRenderer.h"
// #include "Engine/ResourceManager.h"
// #include "Engine/Entity.h"
// #include "Engine/Components/BoxCollider2D.h"
// #include "Engine/Components/MeshComponent.h"
// #include "Engine/Components/RigidBody2D.h"
// #include "Engine/Physics/PhysicsSystem.h"
//
// //
// // int main() {
// // //    system("export MallocStackLogging=1;");
// //     std::cout << "Starting application...\n";
// //
// //     glfwSetErrorCallback(error_callback);
// //
// //     Window window(640, 480, "Simple example");
// //
// //     // Warning: something is wrong with this function
// //     enableReportGlErrors();
// //
// //
// // #if REMOVE_IMGUI == 0
// //     IMGUI_CHECKVERSION();
// //     ImGui::CreateContext();
// //     ImGuiIO &io = ImGui::GetIO();
// //     (void) io;
// //     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
// //     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
// //     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
// //
// //     ImGui::StyleColorsDark();
// //     ImGuiStyle &style = ImGui::GetStyle();
// //     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
// //         style.WindowRounding = 0.0f;
// //         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
// //     }
// //
// //     ImGui_ImplGlfw_InitForOpenGL(window.getRawWindow(), true);
// //     ImGui_ImplOpenGL3_Init("#version 330");
// //     std::cout << "ImGui initialized successfully\n";
// // #endif
// // //    Physics::Init();
// //
// //     HighRenderer::Init();
// //     Scene scene;
// //     PhysicsSystem physics(scene.Registry());
// //
// //
// //     for (int i = 0; i < 7; i++){
// //         Entity square = scene.CreateEntity();
// //         square.GetComponent<Transform>().position.x = 1.1f * i -3.35f;
// //         square.AddComponent<MeshComponent>(MeshComponent::LoadFromFile("square.obj"));
// //         square.AddComponent<RigidBody2D>(RigidBody2D{
// //             .body = b2_nullBodyId,
// //             .type = b2_dynamicBody,
// //             .fixedRotation = false,
// //         });
// //         square.AddComponent<BoxCollider2D>(BoxCollider2D{
// //             .hx = .5f,
// //             .hy = .5f,
// //             .density = 1.0f,
// //             .friction = 0.3f,
// //             .restitution = 0.0f,
// //             .isSensor = false,
// //         });
// //
// //     }
// //     // Static ground
// //     {
// //         Entity ground = scene.CreateEntity();
// //         // ground.GetTransform().scale.x = 7.0f;
// //         // ground.GetTransform().scale.y = 1.0f;
// //         ground.GetTransform().position.y -= 3.0f;
// //
// //         ground.AddComponent<MeshComponent>(MeshComponent::LoadFromRectangle(7.0f, 1.0f));
// //
// //         ground.AddComponent<RigidBody2D>(RigidBody2D{.type=b2_staticBody });
// //         ground.AddComponent<BoxCollider2D>(BoxCollider2D{ .hx=3.5f, .hy=0.5f, .density=0.0f, .friction=0.8f });
// //     }
// //
// //
// //     // enable alpha blending
// //     // 0 means opaque
// //     // 1 means transparent
// //     glEnable(GL_BLEND);
// //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// //     glDepthMask(GL_TRUE);
// //     // disable vSync
// //     glfwSwapInterval(0);
// //
// //     while (!window.shouldClose()) {
// //
// //         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
// //         glClear(GL_COLOR_BUFFER_BIT);
// //
// //
// // #if REMOVE_IMGUI == 0
// //         ImGui_ImplOpenGL3_NewFrame();
// //         ImGui_ImplGlfw_NewFrame();
// //         ImGui::NewFrame();
// //
// // //        Physics::Update();
// //         physics.Step(LowRenderer::getDeltaTime());
// //
// //         HighRenderer::Update(LowRenderer::getDeltaTime());
// //         HighRenderer::Draw();
// //         ImGui::ShowDemoWindow();
// //
// //         scene.Draw();
// //
// //         ImGui::Render();
// //         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// //
// //         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
// //             GLFWwindow *backup_current_context = glfwGetCurrentContext();
// //
// //             ImGui::UpdatePlatformWindows();
// //             ImGui::RenderPlatformWindowsDefault();
// //             glfwMakeContextCurrent(backup_current_context);
// //         }
// // #endif
// //         window.swapBuffers();
// //         window.pollInputs();
// //     }
// //
// // #if REMOVE_IMGUI == 0
// //     ImGui_ImplOpenGL3_Shutdown();
// //     ImGui_ImplGlfw_Shutdown();
// //     ImGui::DestroyContext();
// // #endif
// //
// //
// //     //clean up
// //     std::cout << "Cleaning up...\n";
// //     HighRenderer::Destroy();
// //     ResourceManager::Destroy();
// //
// //
// //     std::cout << "Application terminated successfully\n";
// // //    system("leaks Ray-Game --list");
// //
// //     return 0;
// // }


#include "SandboxUI.h"
#include "SandboxWorld.h"
#include "Engine/EntryPoint.h"
#include "Engine/ImguiLayer.h"


class SandboxApp : public Ramp::Application {
    public:
        SandboxApp() {
            PushLayer(new SandboxUI());
            PushLayer(new SandboxWorld());
            PushLayer(new ImguiLayer());
        }

        ~SandboxApp() override = default;
};



Ramp::Application* Ramp::CreateApplication() {
    return new SandboxApp();
}