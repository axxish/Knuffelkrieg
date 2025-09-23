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

constexpr int PLAYER_AREA_HEIGHT = 320;
constexpr int PLAYER_AREA_WIDTH = 180;

struct LevelState
{
    Graphics gfx;

    std::vector<Entity> entities;

    Entity& addEntity(Rectangle trans, const std::string&  texName,
           const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget= 320, Vector2 velocity = {0, 100});



    Rectangle playArea = {0, 0, 480, 620 };
    float entityCullGracePeriod = 200; //in Pixels
    Rectangle cullingArea = {-playArea.width/2 - entityCullGracePeriod, -playArea.height /2 - entityCullGracePeriod,
        playArea.width + entityCullGracePeriod, playArea.height + entityCullGracePeriod};
    void onInit();

    static Vector2 getMovementVector();

    void onUpdate();

    void onRender();

private:
    Camera2D camera = {{}};
    double scrollPos = 0.0f;
    RenderTexture2D playerAreaRenderTexture = {0};
    int playerIndex = -1;
    int bossIndex = -1;
    const float loggerCooldown = 0.5f;
    float loggerTimer = 0;
};
