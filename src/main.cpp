#include <iostream>
#include <string>
#include <unordered_map>

#include "Entity.h"
#include "raylib.h"
#include "raymath.h"

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
    std::unordered_map<std::string, Entity> friendlyEnt;
    std::unordered_map<std::string, Entity> enemyEnt;

    double scrollPos = 0.0f;
    double scrollSpeed = 1.0f;

    void onInit()
    {
        const int screenWidth = GetRenderWidth();
        const int screenHeight = GetRenderHeight();
        friendlyEnt.insert({"player", Entity({0, 0, 124, 135}, "res/player.png", 320,
                                             std::make_shared<LinearMovement>())});
        auto& player = friendlyEnt.find("player")->second;
        player.trans.x = screenWidth / 2 - player.trans.width / 2;
        player.trans.y = screenHeight - player.trans.height - 20;

        auto a = enemyEnt.insert({"asteroid1", Entity({100, -50, 96, 96}, "res/aster.png", 100,
                                                      std::make_shared<LinearMovement>())});

        a.first->second.velocity.y = 100;

        a = enemyEnt.insert({"asteroid2", Entity({400, -50, 96, 96}, "res/aster.png", 100,
                                                 std::make_shared<SineMovement>(100, 2, 0))});

        a.first->second.velocity.y = 100;

        a = enemyEnt.insert({"asteroid3", Entity({200, -50, 96, 96}, "res/aster.png.png", 150,
                                              std::make_shared<TargetPlayerMovement>(
                                                  Vector2{player.trans.x, player.trans.y}, 11))});
    }

    Vector2 getMovementVector()
    {
        Vector2 movement;
        movement.x = 0;
        movement.y = 0;
        if (IsKeyDown(KEY_UP))
            movement.y -= 1;
        if (IsKeyDown(KEY_DOWN))
            movement.y += 1;
        if (IsKeyDown(KEY_LEFT))
            movement.x -= 1;
        if (IsKeyDown(KEY_RIGHT))
            movement.x += 1;
        return Vector2Normalize(movement);
    }

    void onUpdate()
    {
        float delta = GetFrameTime();

        auto playerIt = friendlyEnt.find("player");
        Entity* player = nullptr;
        if (playerIt != friendlyEnt.end())
        {
            player = &playerIt->second;
        }

        if (player)
        {
            Vector2 movement = getMovementVector();
            auto speed = player->speedStat;
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                speed = speed / 2;
            }

            player->velocity.x = movement.x * speed;
            player->velocity.y = movement.y * speed;

            scrollPos += scrollSpeed * delta;
        }

        for (auto& [name, entity] : enemyEnt)
        {
            entity.lifetime += delta;
            entity.movementStrategy.get()->update(entity, delta);
        }
        for (auto& [name, entity] : friendlyEnt)
        {
            entity.lifetime += delta;
            entity.movementStrategy.get()->update(entity, delta);
        }
    }

    void onRender()
    {
        std::cout << "\x1b[2J\x1b[H";

        std::cout << "Scroll: " << scrollPos << "\n";

        for (auto& [name, entity] : friendlyEnt)
        {
            gfx.drawTexture(entity.textureName, entity.trans);
            std::cout << name << ": " << entity.dump() << "\n";
        }

        for (auto& [name, entity] : enemyEnt)
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