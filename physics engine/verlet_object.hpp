#pragma once

#include "raylib.h"
#include "vector2_math.hpp"

struct VerletObject
{
    Vector2 position_current;
    float radius;
    Color color;
    bool static_object;
    Vector2 position_old;
    Vector2 acceleration;

    void updatePosition(const float dt)
    {
        const Vector2 velocity = position_current - position_old;

        position_old = position_current;
        position_current += velocity + acceleration * dt * dt;

        acceleration = {};
    }

    void accelerate(const Vector2 acc)
    {
        acceleration += acc;
    }
};