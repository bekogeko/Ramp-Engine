//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_RIGIDBODY2D_H
#define RAY_GAME_RIGIDBODY2D_H

#include <box2d/box2d.h>

struct RigidBody2D {
    b2BodyId body{b2_nullBodyId};
    b2BodyType type{b2_dynamicBody};
    bool fixedRotation{false};
    float linearDamping{0.0f};
    float angularDamping{0.0f};

    // Optional: initial velocities
    float vx{0.0f};
    float vy{0.0f};
    float av{0.0f};
};


#endif //RAY_GAME_RIGIDBODY2D_H