//
// Created by axxy on 18.09.2025.
//

#include <iostream>
#include <memory>

#include "EmittingStrategies.h"
#include "Entity.h"
#include "LevelState.h"
#include "MovementStrategies.h"
#include "raylib.h"

Entity& LevelState::addEntity(Rectangle trans, const std::string& texName,
                          const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget,
                          Vector2 velocity)
{
    return entities.emplace_back(*this, trans, texName, speedTarget, moveStrat, velocity);

}
void LevelState::onInit()
{
    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();

    auto &player = addEntity({0, 0, 124, 135}, "res/player.png", std::make_shared<LinearMovement>(), 320, {0, 0});

    player.trans.x = static_cast<float>(screenWidth) / 2 - player.trans.width / 2;
    player.trans.y = static_cast<float>(screenHeight) - player.trans.height - 20;

    player.emittingStrategy = std::make_shared<PlayerEmittingStrategy>();

    playerIndex = entities.size()-1;

    std::vector<MovementPhase> phaseList;

    phaseList.push_back(
        {std::make_unique<MoveToPoint>(
             Vector2{static_cast<float>(screenWidth) / 2, static_cast<float>(screenHeight) / 2}),
         2.0f, 50});

    phaseList.push_back({std::make_unique<MoveToPoint>(Vector2{0, 0}), 1.0f, 300});

    phaseList.push_back({std::make_unique<SineMovement>(10, 10, 0, Vector2{0, 1}), 10.0f, 300});

    phaseList.push_back({std::make_unique<LinearMovement>(), 2.0f, 300});

    entities.push_back(Entity(*this, {200, -50, 96, 96}, "res/aster.png", 100,
                                         std::make_shared<ChainedMovement>(std::move(phaseList)),
                                         Vector2{0, 150}));

    entities.push_back(Entity(*this, {200, 100, 96, 96}, "res/aster.png", 100,
                                         std::make_shared<SineMovement>(20, 10, 0, Vector2{0, 1}),
                                         Vector2{150, 0}));
}
Vector2 LevelState::getMovementVector()
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
void LevelState::onUpdate()
{
    const float delta = GetFrameTime();



    if (playerIndex != -1)
    {
        auto& player = entities[playerIndex];


        auto [x, y] = getMovementVector();
        auto speed = player.speedTarget;
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            speed = speed / 2;
        }

        player.velocity.x = x * static_cast<float>(speed);
        player.velocity.y = y * static_cast<float>(speed);

        scrollPos += delta;
    }

    for (auto &entity : entities)
    {
        entity.lifetime += delta;
        if (entity.movementStrategy != nullptr)
        {
            entity.movementStrategy->update(entity, delta);
        }
        if (entity.emittingStrategy != nullptr)
        {
            entity.emittingStrategy->update(entity, delta);
        }
    }

}
void LevelState::onRender()
{
    // std::cout << "\x1b[2J\x1b[H";

    for (const auto& entity : entities)
    {
        gfx.drawTexture(entity.textureName, entity.trans);
    }


}