//
// Created by Bekir Gulestan on 11/17/25.
//

#ifndef RAY_GAME_SANDBOXUI_H
#define RAY_GAME_SANDBOXUI_H
#include "Engine/UILayer.h"

class SandboxUI: public UILayer {
public:
    SandboxUI();
private:
    void BuildUI() override;
};
#endif //RAY_GAME_SANDBOXUI_H