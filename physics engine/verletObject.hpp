#pragma once
#include "raylib.h"
#include "vec2.hpp"

struct VerletObject
{
    Vec2 position_current;
    // v Optional v
    int radius;
    Color color;
    bool physics;
    // ^ Optional ^
    Vec2 position_old;
    Vec2 acceleration;

    void updatePosition(float dt)
    {
        const Vec2 velocity = position_current - position_old;
        position_old = position_current;
        position_current = position_current + velocity + acceleration * dt * dt;
        acceleration = {};
    }

    void accelerate(Vec2 acc)
    {
        acceleration += acc;
    }
};