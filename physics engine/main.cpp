#include "raylib.h"
#include <iostream>
#include <random>
#include <math.h>
#include <vector>

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
        x += rhs;
        y += rhs;
        return *this;
    }

    Vec2& operator-=(const int& rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    Vec2& operator*=(const int& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vec2& operator/=(const int& rhs)
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
        return lhs += rhs;
    }

    friend Vec2 operator-(Vec2 lhs, const int& rhs)
    {
        return lhs -= rhs;
    }

    friend Vec2 operator*(Vec2 lhs, const int& rhs)
    {
        return lhs *= rhs;
    }

    friend Vec2 operator/(Vec2 lhs, const int& rhs)
    {
        return lhs /= rhs;
    }
};
namespace Vec2Math
{
    float length(Vec2 v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }
}

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

struct Solver
{
    Vec2 gravity = { 0, 1000 };

    void update(std::vector<VerletObject> &verletObjects, std::vector<Link> &links, float dt)
    {
        const uint32_t sub_steps = 8;
        const float sub_dt = dt / sub_steps;
        for (uint32_t i(sub_steps); i--;) {
            applyGravity(verletObjects);
            applyConstraint(verletObjects);

            for (int link_i = 0; link_i < links.size(); link_i++)
            {
                links[link_i].apply(verletObjects);
            }

            solveCollisions(verletObjects);
            updatePositions(verletObjects, sub_dt);
        }
    }

    void applyGravity(std::vector<VerletObject>& verletObjects)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            verletObjects[i].accelerate(gravity);
        }
    }

    void applyConstraint(std::vector<VerletObject>& verletObjects)
    {
        const Vec2 position = { 0, 0 };
        const float radius = 800;

        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            VerletObject& obj = verletObjects[i];

            const Vec2 to_obj = obj.position_current - position;
            const float dist = Vec2Math::length(to_obj);

            if (dist > radius - obj.radius) {
                const Vec2 n = to_obj / dist;
                obj.position_current = position + n * (radius - obj.radius);
            }
        }
    }

    void solveCollisions(std::vector<VerletObject>& verletObjects)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            VerletObject& object_1 = verletObjects[i];

            for (int i2 = 0; i2 < verletObjects.size(); i2++)
            {
                VerletObject& object_2 = verletObjects[i2];

                const Vec2 collision_axis = object_1.position_current - object_2.position_current;
                const float dist = Vec2Math::length(collision_axis);
                const float min_dist = object_1.radius + object_2.radius;
                if (dist < min_dist) {
                    const Vec2 n = collision_axis / dist;
                    const float delta = min_dist - dist;
                    if (object_1.physics) object_1.position_current += n * delta * 0.5f;
                    if (object_2.physics) object_2.position_current -= n * delta * 0.5f;
                }
            }
        }
    }

    void updatePositions(std::vector<VerletObject>& verletObjects, float dt)
    {
        for (int i = 0; i < verletObjects.size(); i++)
        {
            if (!verletObjects[i].physics)
                continue;

            verletObjects[i].updatePosition(dt);
        }
    }
};

std::default_random_engine generator((unsigned int)time(0));
int random(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

int main(void)
{
    std::vector<VerletObject> verletObjects = {};

    for (int x = -250; x < 250; x += 55)
        verletObjects.push_back( { {(float)x, 100}, 25, WHITE, !(x == -250 || x == 245), {(float)x, 100} } );

    std::vector<Link> links = {};

    for (uint32_t i = 0; i < verletObjects.size() - 1; i++)
    {
        int obj = i;
        int obj_2 = i + 1;
        links.push_back( { obj, obj_2, 75 } );
    }

    Solver solver = {};

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Physics Engine");

    SetTargetFPS(60);

    float cameraX = (screenWidth / 2);
    float cameraY = (screenHeight / 2) + 15;
    float cameraZoom = 4;
    Vec2 camera = { cameraX, cameraY };

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            cameraY += GetFrameTime() * 500;

        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            cameraY -= GetFrameTime() * 500;
        
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            cameraX += GetFrameTime() * 500;

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            cameraX -= GetFrameTime() * 500;

        cameraZoom -= GetMouseWheelMove() / 3;
        
        camera = { cameraX, cameraY };

        if (IsMouseButtonPressed(0) || IsMouseButtonDown(1))
        {
            Vec2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };
            Vec2 ballPosition = (mousePosition - camera) * cameraZoom;
            verletObjects.push_back( { ballPosition, random(10, 50), ColorFromHSV(random(0, 360), random(0, 100), random(0, 100)), true, ballPosition } );
        }

        solver.update(verletObjects, links, GetFrameTime());

        BeginDrawing();

        ClearBackground(GRAY);

        float circleSize = 800;
        DrawCircle(0 + cameraX, 0 + cameraY, circleSize / cameraZoom, BLACK);

        for (uint32_t i = 0; i < verletObjects.size(); i++)
        {
            const VerletObject obj = verletObjects[i];
            DrawCircle((obj.position_current.x / cameraZoom) + cameraX, (obj.position_current.y / cameraZoom) + cameraY, obj.radius / cameraZoom, obj.color);
        }

        DrawRectangle(0, 0, screenWidth, 30, BLACK);
        DrawFPS(5, 5);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}