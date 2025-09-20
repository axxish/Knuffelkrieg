#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Graphics.h"
#include "raylib.h"
#include "raymath.h"

class IMovementStrategy;
class IEmittingStrategy;
struct Entity;


struct LevelState
{
    Graphics gfx;

    std::vector<Entity> entities;

    Entity& addEntity(Rectangle trans, const std::string&  texName,
           const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget= 320, Vector2 velocity = {0, 100});



    void onInit();

    static Vector2 getMovementVector();

    void onUpdate();

    void onRender();

private:
    double scrollPos = 0.0f;
    int playerIndex = -1;
    int bossIndex = -1;
};
