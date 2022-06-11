#include "raylib.h"
#include "vec2.hpp"
#include "verletObject.hpp"
#include "link.hpp"
#include "solver.hpp"
#include "random.hpp"
#include <math.h>
#include <string>
#include <vector>

std::vector<VerletObject> verletObjects = {};
std::vector<Link> links = {};
Solver solver = {};

int screenWidth = 800;
int screenHeight = 450;

int centerX = (int)(screenWidth / 2);
int centerY = (int)(screenHeight / 2);

float cameraX = 0;
float cameraY = -42;
float cameraZoom = 0.25f;

Vec2 camera = { cameraX, cameraY };

void controlsUpdate()
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        cameraY -= GetFrameTime() * 500 / cameraZoom;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        cameraY += GetFrameTime() * 500 / cameraZoom;

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        cameraX -= GetFrameTime() * 500 / cameraZoom;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        cameraX += GetFrameTime() * 500 / cameraZoom;

    cameraZoom += cameraZoom * GetMouseWheelMove() / 15;
    if (cameraZoom < 0.01f) cameraZoom = 0.01f;

    camera = { cameraX, cameraY };

    if (IsMouseButtonPressed(0) || IsMouseButtonDown(1))
    {
        Vec2 mousePosition = { (float)(GetMouseX() - centerX), (float)(GetMouseY() - centerY) };
        Vec2 ballPosition = (mousePosition / cameraZoom) + camera;

        verletObjects.push_back({ ballPosition, randomFloat(10, 50), ColorFromHSV(randomFloat(0, 360), randomFloat(0, 100), randomFloat(0, 100)), true, ballPosition });
    }
}

int main(void)
{
    // v Test v
    for (int x = -250; x < 250; x += 55)
    {
        Vec2 obj_position = { (float)x, 100 };

        VerletObject obj = { obj_position, 25, WHITE, !(x == -250 || x == 245) };
        obj.position_old = obj.position_current;

        verletObjects.push_back(obj);
    }

    for (int i(verletObjects.size() - 1); i--;)
    {
        int obj_1 = i;
        int obj_2 = i + 1;
        links.push_back( { obj_1, obj_2, 75 } );
    }

    // ^ Test ^

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Physics Engine");

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        SetWindowSize(screenWidth, screenHeight);

        controlsUpdate();
        solver.update(verletObjects, links, GetFrameTime());

        BeginDrawing();

        ClearBackground(DARKGRAY);

        float circleSize = 800;
        DrawCircle(centerX + (int)((0 - cameraX) * cameraZoom), centerY + (int)((0 - cameraY) * cameraZoom), circleSize * cameraZoom, BLACK);

        for (uint32_t i = 0; i < verletObjects.size(); i++)
        {
            const VerletObject obj = verletObjects[i];
            DrawCircle(centerX + (int)((obj.position_current.x - cameraX) * cameraZoom), centerY + (int)((obj.position_current.y - cameraY) * cameraZoom), obj.radius * cameraZoom, obj.color);
        }

        DrawRectangle(0, 0, screenWidth, 28, BLACK);

        DrawFPS(5, 5);
        DrawText((std::to_string(verletObjects.size()) + std::string(" OBJECTS")).c_str(), 155, 5, 20, WHITE);
        DrawText((std::to_string(links.size()) + std::string(" LINKS")).c_str(), 355, 5, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}