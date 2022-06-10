#pragma once
#include "vec2.hpp"
#include "vec2math.hpp"
#include "verletObject.hpp"
#include <vector>

struct Link
{
    int obj_1;
    int obj_2;
    float target_dist;

    void apply(std::vector<VerletObject>& verletObjects)
    {
        const Vec2 axis = verletObjects[obj_1].position_current - verletObjects[obj_2].position_current;
        const float dist = Vec2Math::length(axis);
        const Vec2 n = axis / dist;
        const float delta = target_dist - dist;
        if (verletObjects[obj_1].physics) verletObjects[obj_1].position_current += n * delta * 0.5f;
        if (verletObjects[obj_2].physics) verletObjects[obj_2].position_current -= n * delta * 0.5f;
    }
};