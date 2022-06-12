#pragma once
#include "raylib.h"
#include "raymath.h"

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2Add(v1, v2);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2Subtract(v1, v2);
}

Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2Multiply(v1, v2);
}

Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2Divide(v1, v2);
}


Vector2 operator+(const Vector2& vec, const float& val)
{
    return Vector2AddValue(vec, val);
}

Vector2 operator-(const Vector2& vec, const float& val)
{
    return Vector2SubtractValue(vec, val);
}

Vector2 operator*(const Vector2& vec, const float& val)
{
    return Vector2Scale(vec, val);
}

Vector2 operator/(const Vector2& vec, const float& val)
{
    if (val != 0)
        return Vector2Scale(vec, (1 / val));
    return Vector2One();
}


Vector2 operator+(const Vector2& vec, const int& val)
{
    return Vector2AddValue(vec, (float)val);
}

Vector2 operator-(const Vector2& vec, const int& val)
{
    return Vector2SubtractValue(vec, (float)val);
}

Vector2 operator*(const Vector2& vec, const int& val)
{
    return Vector2Scale(vec, (float)val);
}

Vector2 operator/(const Vector2& vec, const int& val)
{
    if (val != 0)
        return Vector2Scale(vec, (1 / (float)val));
    return Vector2One();
}