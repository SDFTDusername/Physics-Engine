#pragma once
#include "raylib.h"
#include "raymath.h"
#include "vector2_math.hpp"
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

        const Vector2 axis = obj_1.position_current - obj_2.position_current;
        const float dist = Vector2Length(axis);
        const Vector2 n = axis / dist;
        const float delta = target_dist - dist;

        if (!obj_1.static_object) obj_1.position_current = obj_1.position_current + (n * delta * 0.5f);
        if (!obj_2.static_object) obj_2.position_current = obj_2.position_current - (n * delta * 0.5f);
    }
};