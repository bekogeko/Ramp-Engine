//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_BOXCOLLIDER2D_H
#define RAY_GAME_BOXCOLLIDER2D_H

#include <box2d/box2d.h>

struct BoxCollider2D {
    // Half extents in your world units (meters). If your renderer is in pixels, decide a pixels-per-meter scale.
    float hx{0.5f};
    float hy{0.5f};

    float density{1.0f};
    float friction{0.2f};
    float restitution{0.0f};
    bool isSensor{false};
};


#endif //RAY_GAME_BOXCOLLIDER2D_H