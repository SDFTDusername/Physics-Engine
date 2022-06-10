#pragma once
#include "raylib.h"
#include "vec2.hpp"
#include "vec2math.hpp"

struct VerletObject
{
    Vec2 position_current;
    int radius;
    Color color;
    bool physics;
    Vec2 position_old;
    Vec2 acceleration;

    void updatePosition(float dt, Sound hit)
    {
        const Vec2 velocity = position_current - position_old;

        /*
        if (Vec2Math::length(velocity) < -0.5f)
        {
            PlaySound(hit);
        }
        */

        position_old = position_current;
        position_current = position_current + velocity + acceleration * dt * dt;

        acceleration = {};
    }

    void accelerate(Vec2 acc)
    {
        acceleration += acc;
    }
};