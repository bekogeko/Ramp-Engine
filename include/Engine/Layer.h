//
// Created by Bekir Gulestan on 1/8/25.
//

#ifndef RAY_GAME_LAYER_H
#define RAY_GAME_LAYER_H

struct EngineContext {
    class Window* window;

};

class Layer {


public:

    virtual void OnAttach(EngineContext ctx) {}
    virtual void Update(float deltaTime) = 0;

    virtual void Draw() = 0;

    virtual ~Layer() = default;


};

#endif //RAY_GAME_LAYER_H
