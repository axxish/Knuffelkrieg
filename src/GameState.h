#pragma once
#include <string>
#include <unordered_map>
#include "raylib.h"
#include "Graphics.h"
struct Entity;

struct GameState
{
    Graphics gfx;
    std::unordered_map<std::string, Entity> friendlyEnt;
    std::unordered_map<std::string, Entity> enemyEnt;

    double scrollPos = 0.0f;

    void onInit();

    static Vector2 getMovementVector();

    void onUpdate();

    void onRender();
};
