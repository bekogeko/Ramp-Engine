//
// Created by Bekir Gulestan on 11/17/25.
//

#ifndef RAY_GAME_ENTRYPOINT_H
#define RAY_GAME_ENTRYPOINT_H
#include <iostream>

#include "Application.h"


// extern Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args);

extern Ramp::Application* Ramp::CreateApplication();

static void error_callback(int error, const char *description) {
    std::cerr << "Error: " << description << "\n";
}

int main(int argc, char** argv)
{

    auto app = Ramp::CreateApplication();

    glfwSetErrorCallback(error_callback);
    app->Run();

    delete app;
}


#endif //RAY_GAME_ENTRYPOINT_H