#include "raylib.h"
#include "vector2_math.hpp"
#include "verlet_object.hpp"
#include "link.hpp"
#include "solver.hpp"
#include <math.h>
#include <string>
#include <vector>

Camera2D camera = { 0 };

std::vector<VerletObject> verletObjects = {};
std::vector<Link> links = {};
Solver solver = {};

bool simulationPaused = false;
float simulationSpeed = 1;

int screenWidth = 1200;
int screenHeight = 675;

Vector2 screenCenter;

int grabObject = -1;
bool grabObjectStatic = false;
Vector2 grabObjectOffset;

void spawnBall(const Vector2 position)
{
    verletObjects.push_back({ position, (float)GetRandomValue(10, 50), ColorFromHSV((float)GetRandomValue(0, 360), (float)GetRandomValue(0, 100), (float)GetRandomValue(0, 100)), false, position });
}

void test()
{
    verletObjects.clear();
    links.clear();

    for (float x = -250; x < 250; x += 55)
    {
        Vector2 obj_position = { x, 100 };

        VerletObject obj = { obj_position, 25, WHITE, (x == -250 || x == 245) };
        obj.position_old = obj.position_current;

        verletObjects.push_back(obj);
    }

    for (int i(verletObjects.size() - 1); i--;)
    {
        int obj_1 = i;
        int obj_2 = i + 1;
        links.push_back({ obj_1, obj_2, 75 });
    }
}

void controlsUpdate()
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        camera.target.y -= dt * 500 / camera.zoom;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        camera.target.y += dt * 500 / camera.zoom;

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        camera.target.x -= dt * 500 / camera.zoom;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        camera.target.x += dt * 500 / camera.zoom;

    if (IsKeyDown(KEY_M))
        simulationSpeed += dt * 2;

    if (IsKeyDown(KEY_N))
        simulationSpeed -= dt * 2;

    if (IsKeyPressed(KEY_R))
        test();

    camera.zoom += camera.zoom * GetMouseWheelMove() / 15;

    const Vector2 mousePosition = { (float)(GetMouseX() - screenCenter.x), (float)(GetMouseY() - screenCenter.y) };
    const Vector2 ballPosition = (mousePosition / camera.zoom) + camera.target;

    if (IsKeyPressed(KEY_P))
        simulationPaused = !simulationPaused;

    if (IsMouseButtonDown(1))
        spawnBall(ballPosition);

    if (grabObject > -1 && IsMouseButtonUp(0))
    {
        VerletObject& obj = verletObjects[grabObject];

        obj.static_object = grabObjectStatic;
        obj.position_old = obj.position_current;
        obj.acceleration = { };

        grabObject = -1;

        return;
    }

    if (grabObject > -1 && IsMouseButtonDown(0) && !IsMouseButtonPressed(0)) {
        VerletObject& obj_2 = verletObjects[grabObject];

        obj_2.position_current = ballPosition + grabObjectOffset;
        obj_2.position_old = obj_2.position_current;

        return;
    }

    if (IsMouseButtonPressed(0)) {
        if (grabObject < 0 && IsMouseButtonPressed(0)) {
            for (int i(verletObjects.size()); i--;) {
                VerletObject& obj = verletObjects[i];

                const Vector2 point = { (float)GetMouseX(), (float)GetMouseY() };
                const Vector2 center = { (float)(screenCenter.x + ((obj.position_current.x - camera.target.x) * camera.zoom)), (float)(screenCenter.y + ((obj.position_current.y - camera.target.y) * camera.zoom)) };
                const float radius = obj.radius * camera.zoom;

                if (CheckCollisionPointCircle(point, center, radius))
                {
                    grabObject = i;
                    
                    VerletObject& obj_2 = verletObjects[grabObject];
                    grabObjectStatic = obj_2.static_object;

                    Vector2 pointVec2 = { point.x, point.y };
                    Vector2 centerVec2 = { center.x, center.y };
                    grabObjectOffset = point - center;

                    obj_2.static_object = true;

                    obj_2.position_current = ballPosition + grabObjectOffset;
                    obj_2.position_old = obj_2.position_current;

                    break;
                }
            }

            if (grabObject > -1)
                return;
        }

        spawnBall(ballPosition);
    }
}

int main(void)
{
    // v Test v
    test();
    // ^ Test ^

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Physics Engine");

    SetTargetFPS(60);

    camera.target = { 0, -42 };
    camera.zoom = 0.25f;

    while (!WindowShouldClose()) 
    {
        float dt = GetFrameTime();
        dt *= simulationSpeed;

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        screenCenter = { (float)screenWidth / 2, (float)screenHeight / 2 };
        camera.offset = screenCenter;
        
        controlsUpdate();
        if (!simulationPaused) solver.update(verletObjects, links, dt);

        BeginDrawing();

        ClearBackground(DARKGRAY);

        BeginMode2D(camera);
            float circleSize = 800;
            DrawCircle(0, 0, circleSize, BLACK);

            for (uint32_t i = 0; i < verletObjects.size(); i++)
            {
                const VerletObject obj = verletObjects[i];
                DrawCircle((int)obj.position_current.x, (int)obj.position_current.y, obj.radius, obj.color);
            }
        EndMode2D();

        DrawRectangle(0, 0, screenWidth, 28, BLACK);

        DrawFPS(5, 5);
        DrawText((std::to_string(verletObjects.size()) + std::string(" OBJECTS")).c_str(), 155, 5, 20, WHITE);
        DrawText((std::to_string(links.size()) + std::string(" LINKS")).c_str(), 355, 5, 20, WHITE);
        DrawText((std::string("PAUSED: ") + (simulationPaused ? std::string("TRUE") : std::string("FALSE"))).c_str(), 555, 5, 20, WHITE);
        DrawText((std::string("SIMULATION SPEED: ") + (std::to_string(simulationSpeed))).c_str(), 755, 5, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}