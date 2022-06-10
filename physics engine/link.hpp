#pragma once
#include "vec2.hpp"
#include "vec2math.hpp"
#include "verletObject.hpp"
#include <vector>

struct Link
{
    int object_1;
    int object_2;
    float target_dist;

    void apply(std::vector<VerletObject>& verletObjects)
    {
        VerletObject& obj_1 = verletObjects[object_1];
        VerletObject& obj_2 = verletObjects[object_2];

        const Vec2 axis = obj_1.position_current - obj_2.position_current;
        const float dist = Vec2Math::length(axis);
        const Vec2 n = axis / dist;
        const float delta = target_dist - dist;

        if (obj_1.physics) obj_1.position_current += n * delta * 0.5f;
        if (obj_2.physics) obj_2.position_current -= n * delta * 0.5f;
    }
};