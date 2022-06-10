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

float cameraX = (screenWidth / 2);
float cameraY = (screenHeight / 2) + 15;
float cameraZoom = 4;

Sound hit;
Sound pop;

Vec2 camera = { cameraX, cameraY };

void controlsUpdate()
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        cameraY += GetFrameTime() * 2000 / cameraZoom;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        cameraY -= GetFrameTime() * 2000 / cameraZoom;

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        cameraX += GetFrameTime() * 2000 / cameraZoom;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        cameraX -= GetFrameTime() * 2000 / cameraZoom;

    cameraZoom -= cameraZoom * GetMouseWheelMove() / 10;
    if (cameraZoom < 0.01f) cameraZoom = 0.01f;

    camera = { cameraX, cameraY };

    if (IsMouseButtonPressed(0) || IsMouseButtonDown(1))
    {
        Vec2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };
        Vec2 ballPosition = (mousePosition - camera) * cameraZoom;

        verletObjects.push_back({ ballPosition, random(10, 50), ColorFromHSV(random(0, 360), random(0, 100), random(0, 100)), true, ballPosition });

        PlaySound(pop);
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

    InitAudioDevice();

    hit = LoadSound("hit.wav");
    pop = LoadSound("pop.wav");

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        controlsUpdate();

        solver.update(verletObjects, links, GetFrameTime(), hit);

        BeginDrawing();

        ClearBackground(DARKGRAY);

        float circleSize = 800;
        DrawCircle(0 + cameraX, 0 + cameraY, circleSize / cameraZoom, BLACK);

        for (uint32_t i = 0; i < verletObjects.size(); i++)
        {
            const VerletObject obj = verletObjects[i];
            DrawCircle((obj.position_current.x / cameraZoom) + cameraX, (obj.position_current.y / cameraZoom) + cameraY, obj.radius / cameraZoom, obj.color);
        }

        DrawRectangle(0, 0, screenWidth, 27.5f, BLACK);

        DrawFPS(5, 5);
        DrawText((std::to_string(verletObjects.size()) + std::string(" OBJECTS")).c_str(), 155, 5, 20, WHITE);
        DrawText((std::to_string(links.size()) + std::string(" LINKS")).c_str(), 355, 5, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}