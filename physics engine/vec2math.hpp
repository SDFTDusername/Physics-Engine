#pragma once
#include "vec2.hpp"
#include <math.h>

namespace Vec2Math
{
    float length(Vec2 v)
    {
        return (float)sqrt(((double)v.x * (double)v.x) + ((double)v.y * (double)v.y));
    }
}