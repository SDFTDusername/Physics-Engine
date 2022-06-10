#pragma once

struct Vec2
{
    float x = 0;
    float y = 0;

    Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2& operator*=(const Vec2& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vec2& operator/=(const Vec2& rhs)
    {
        if (x == 0 || rhs.x == 0)
            x = 0;
        else
            x /= rhs.x;
        if (y == 0 || rhs.y == 0)
            y = 0;
        else
            y /= rhs.y;
        return *this;
    }

    Vec2& operator+=(const float& rhs)
    {
        x += rhs;
        y += rhs;
        return *this;
    }

    Vec2& operator-=(const float& rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    Vec2& operator*=(const float& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vec2& operator/=(const float& rhs)
    {
        if (x == 0 || rhs == 0)
            x = 0;
        else
            x /= rhs;
        if (y == 0 || rhs == 0)
            y = 0;
        else
            y /= rhs;
        return *this;
    }

    Vec2& operator+=(const int& rhs)
    {
        return operator+=(rhs);
    }

    Vec2& operator-=(const int& rhs)
    {
        return operator-=(rhs);
    }

    Vec2& operator*=(const int& rhs)
    {
        return operator*=(rhs);
    }

    Vec2& operator/=(const int& rhs)
    {
        return operator/=(rhs);
    }

    friend Vec2 operator+(Vec2 lhs, const Vec2& rhs)
    {
        return lhs += rhs;
    }

    friend Vec2 operator-(Vec2 lhs, const Vec2& rhs)
    {
        return lhs -= rhs;
    }

    friend Vec2 operator*(Vec2 lhs, const Vec2& rhs)
    {
        return lhs *= rhs;
    }

    friend Vec2 operator/(Vec2 lhs, const Vec2& rhs)
    {
        return lhs /= rhs;
    }

    friend Vec2 operator+(Vec2 lhs, const float& rhs)
    {
        return lhs += rhs;
    }

    friend Vec2 operator-(Vec2 lhs, const float& rhs)
    {
        return lhs -= rhs;
    }

    friend Vec2 operator*(Vec2 lhs, const float& rhs)
    {
        return lhs *= rhs;
    }

    friend Vec2 operator/(Vec2 lhs, const float& rhs)
    {
        return lhs /= rhs;
    }

    friend Vec2 operator+(Vec2 lhs, const int& rhs)
    {
        return operator+(lhs, rhs);
    }

    friend Vec2 operator-(Vec2 lhs, const int& rhs)
    {
        return operator-(lhs, rhs);
    }

    friend Vec2 operator*(Vec2 lhs, const int& rhs)
    {
        return operator*(lhs, rhs);
    }

    friend Vec2 operator/(Vec2 lhs, const int& rhs)
    {
        return operator/(lhs, rhs);
    }
};