#include "raylib.h"
#include "vec2.hpp"
#include "verletObject.hpp"
#include "link.hpp"
#include "solver.hpp"
#include "random.hpp"
#include <math.h>
#include <vector>

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