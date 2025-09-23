//
// Created by axxy on 18.09.2025.
//

#include <iostream>
#include <memory>

#include "EmittingStrategies.h"
#include "Entity.h"
#include "LevelState.h"

#include "colourDefs.h"
#include "MovementStrategies.h"
#include "raylib.h"


Entity &LevelState::addEntity(Rectangle trans, const std::string &texName,
                              const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget,
                              Vector2 velocity) {
    return entities.emplace_back(*this, trans, texName, speedTarget, moveStrat, velocity);
}

void LevelState::onInit() {
    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();

    playerAreaRenderTexture = LoadRenderTexture(static_cast<int>(playArea.width), static_cast<int>(playArea.height));

    camera = {{playArea.width / 2, playArea.height / 2}, {0, 0}, 0, 1};

    auto &player = addEntity({0, playArea.height / 2, 32*2, 32*2}, "res/Ship_1.png",
                             std::make_shared<LinearMovement>(), 400, {0, 0});

    player.trans.x = 0 - player.trans.width/2;
    player.trans.y = playArea.height/2 ;

    player.emittingStrategy = std::make_shared<PlayerEmittingStrategy>();

    playerIndex = entities.size() - 1;
}

Vector2 LevelState::getMovementVector() {
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

void LevelState::onUpdate() {
    const float delta = GetFrameTime();
    loggerTimer += delta;

    if (playerIndex != -1) {

        auto &player = entities[playerIndex];
        player.isFiring = false;

        auto [x, y] = getMovementVector();
        auto speed = player.speedTarget;
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            speed = speed / 2;
        }

        if (IsKeyDown(KEY_Z)) {
            player.isFiring = true;
        }

        player.velocity.x = x * static_cast<float>(speed);
        player.velocity.y = y * static_cast<float>(speed);

        scrollPos += delta;


    }



    for (auto & entity : entities) {
        //culling logic
        if (!CheckCollisionRecs(cullingArea, entity.trans)) {
            entity.flaggedForRemoval = true;
        }

        entity.lifetime += delta;
        if (entity.movementStrategy != nullptr) {
            entity.movementStrategy->update(entity, delta);
        }
        if (entity.emittingStrategy != nullptr && entity.isFiring) {
            entity.emittingStrategy->update(entity, delta);
        }
    }

    for (unsigned int i = entities.size()-1; i > 0; i--) {
        if (entities[i].flaggedForRemoval) {
            entities[i] = std::move(entities.back());
            entities.pop_back();
        }
    }
}

void LevelState::onRender() {
    // std::cout << "\x1b[2J\x1b[H";

    BeginTextureMode(playerAreaRenderTexture);
    ClearBackground(SPACE_BLACK);
    BeginMode2D(camera);

    for (const auto &entity: entities) {
        gfx.drawTexture(entity.textureName, entity.trans);
    }

    EndMode2D();
    EndTextureMode();

    auto& tex = playerAreaRenderTexture.texture;

    DrawTexturePro(playerAreaRenderTexture.texture, {
                       0, 0,
                       static_cast<float>(playerAreaRenderTexture.texture.width),
                       -static_cast<float>(playerAreaRenderTexture.texture.height)
                   },
                   {
                       0, 0, static_cast<float>(playerAreaRenderTexture.texture.width),
                       static_cast<float>(playerAreaRenderTexture.texture.height)
                   },
                   {0,0}, 0, WHITE);

    if (loggerTimer >= loggerCooldown) {
        std::cout << "\033[2J";
        std::cout << "culling area: " << cullingArea.x << " " << cullingArea.y << " " << cullingArea.width <<" " << cullingArea.height <<  "\n";
        std::cout << "entity count : " << entities.size() << std::endl;
        loggerTimer = 0;
    }
}
