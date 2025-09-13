#include <iostream>
#include <string>
#include <unordered_map>

#include "raylib.h"
#include "raymath.h"

enum class MovementPattern
{
    NONE,          // For the player or static objects
    LINEAR,        // Moves in a straight line
    SINE_WAVE,     // Moves in a sine wave pattern
    TARGET_PLAYER  // Aims at the player's position at spawn time
};

struct Entity
{
    Rectangle trans = {0, 0, 32, 32};
    std::string textureName = {"entity"};
    Vector2 velocity = {0, 0};
    int speedStat = 320;  // for player

    bool isActive = true;  // for pooling
    float lifetime = 0.0f;
    MovementPattern movementPattern = MovementPattern::NONE;

    float pattern_param1 = 0.0f;
    float pattern_param2 = 0.0f;
    Vector2 initialDirection = {0, 0};

    std::string dump()
    {
        return "Entity(" + textureName + " @ " + std::to_string(velocity.x) + "," +
               std::to_string(velocity.y) + ")";
    }
};

struct Graphics
{
    std::unordered_map<std::string, Texture2D> textures;
    void loadTexture(const std::string& name)
    {
        textures[name] = LoadTexture(name.c_str());
    }

    void drawTexture(const std::string& name, Rectangle pos)
    {
        if (textures.find(name) != textures.end())
        {
            DrawTexturePro(textures[name],
                           {0, 0, (float)textures[name].width, (float)textures[name].height},
                           {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
        }
        else
        {
            loadTexture(name);
            DrawTexturePro(textures[name],
                           {0, 0, (float)textures[name].width, (float)textures[name].height},
                           {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
        }
    }
};

struct GameState
{
    Graphics gfx;
    std::unordered_map<std::string, Entity> entities;

    float scrollPos = 0.0f;
    float scrollSpeed = 120.0f;

    void onInit()
    {
        const int screenWidth = GetRenderWidth();
        const int screenHeight = GetRenderHeight();
        entities.insert({"player", Entity{{0, 0, 124 / 1.5, 135 / 1.5}, "res/player.png"}});
        auto& player = entities.find("player")->second;
        player.trans.x = screenWidth / 2 - player.trans.width / 2;
        player.trans.y = screenHeight - player.trans.height - 20;
    }

    Vector2 getMovementVector()
    {
        Vector2 movement;
        movement.x = 0;
        movement.y = 0;
        if (IsKeyDown(KEY_KP_8))
            movement.y -= 1;
        if (IsKeyDown(KEY_KP_5))
            movement.y += 1;
        if (IsKeyDown(KEY_KP_4))
            movement.x -= 1;
        if (IsKeyDown(KEY_KP_6))
            movement.x += 1;
        return Vector2Normalize(movement);
    }

    void onUpdate()
    {
        float delta = GetFrameTime();

        auto playerIt = entities.find("player");
        Entity* player = nullptr;
        if (playerIt != entities.end())
        {
            player = &playerIt->second;
        }

        if (!player)
        {
            return;
        }

        Vector2 movement = getMovementVector();
        auto speed = player->speedStat;
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            speed = speed / 2;
        }

        player->velocity.x = movement.x * speed;
        player->velocity.y = movement.y * speed;

        player->trans.x += player->velocity.x * delta;
        player->trans.y += player->velocity.y * delta;

        scrollPos += scrollSpeed * delta;
    }

    void onRender()
    {
        std::cout << "\x1b[2J\x1b[H";

        for (auto& [name, entity] : entities)
        {
            gfx.drawTexture(entity.textureName, entity.trans);
            std::cout << name << ": " << entity.dump() << "\n";
        }
    }
};

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Knuffelkrieg");
    SetTargetFPS(144);
    GameState state;
    state.onInit();
    state.gfx.loadTexture("res/player.png");
    while (!WindowShouldClose())
    {
        state.onUpdate();

        BeginDrawing();
        ClearBackground({100, 149, 237, 255});

        state.onRender();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}