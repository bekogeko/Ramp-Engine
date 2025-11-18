//
// Created by Bekir Gulestan on 11/17/25.
//

#ifndef RAY_GAME_APPLICATION_H
#define RAY_GAME_APPLICATION_H

#include "Layer.h"
#include "Window.h"

namespace Ramp {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        // onWindowClose
        // onWindowResize

    protected:
        void PushLayer(Layer* layer);

    private:

        // window
        std::unique_ptr<Window> m_window;

        // Layers
        static std::vector<std::unique_ptr<Layer>> layers;

    };

    Application* CreateApplication();
}

#endif //RAY_GAME_APPLICATION_H