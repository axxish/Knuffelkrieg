#include <iostream>
#include <string>
#include <unordered_map>

#include "Entity.h"
#include "LevelState.h"
#include "raylib.h"
#include "raymath.h"

int main()
{
    constexpr int screenWidth = 240*2;
    constexpr int screenHeight = 320*2;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Knuffelkrieg");
    SetTargetFPS(144);
    LevelState state;
    state.onInit();
    state.gfx.loadSpriteSheet("res/particle_2.png");

    while (!WindowShouldClose())
    {
        state.onUpdate();

        BeginDrawing();
        ClearBackground({100, 149, 237, 255});

        state.onRender();

        DrawFPS(GetScreenWidth() - 100, 20);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
