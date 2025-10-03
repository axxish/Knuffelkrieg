#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ResourceLoader.h"
#include "raylib.h"
#include "raymath.h"

class IMovementStrategy;
class IEmittingStrategy;
struct Entity;

constexpr int PLAYER_AREA_HEIGHT = 320;
constexpr int PLAYER_AREA_WIDTH = 240;

struct LevelState
{



    ResourceLoader gfx;

    std::vector<Entity> entities;

    Entity& addEntity(Rectangle trans, const std::string&  texName,
           const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget= 220, Vector2 velocity = {0, 100});

    Rectangle playArea = {0, 0, 240, 320 };
    float entityCullGracePeriod = 500; //in Pixels
    Rectangle cullingArea = {-playArea.width/2 - entityCullGracePeriod, -playArea.height /2 - entityCullGracePeriod,
        playArea.width + entityCullGracePeriod, playArea.height + entityCullGracePeriod};
    void onInit();

    static Vector2 getMovementVector();

    void onUpdate();

    void onRender();

private:
    Camera2D camera = {{}};
    double scrollPos = 0.0f;
    RenderTexture2D backgroundRenderTexture = {0};
    RenderTexture2D lightRenderTexture = {0};
    RenderTexture2D entityRenderTexture = {0};
    RenderTexture2D bloomyRenderTexture = {0};

    int bossIndex = -1;

    std::vector<Entity> spawnQueue; // dirty hack to avoid losing references when u spawn an entity
    const float loggerCooldown = 0.5f;
    float loggerTimer = 0;
    int findPlayer() const;

    const float playerClampAllowance = 10;
};
