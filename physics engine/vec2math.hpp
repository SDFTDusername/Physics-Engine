#pragma once
#include "vec2.hpp"
#include <math.h>

namespace Vec2Math
{
    float length(Vec2 v)
    {
        return (float)sqrt(v.x * v.x + v.y * v.y);
    }
}