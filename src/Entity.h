#pragma once
#include <memory>
#include <string>
#include <utility>

#include "EmittingStrategies.h"
#include "MovementStrategies.h"
#include "raylib.h"
#include "raymath.h"
#include "EmittingStrategies.h"
struct GameState;
struct Entity
{

    GameState& parent;

    Rectangle trans = {0, 0, 32, 32};
    std::string textureName = {"entity"};
    Vector2 velocity = {0, 0};
    float speedStat = 320;

    std::shared_ptr<IMovementStrategy> movementStrategy = nullptr;
    std::shared_ptr<IEmittingStrategy> emittingStrategy = nullptr;

    bool isActive = true;  // for pooling
    float lifetime = 0.0f;

    [[nodiscard]] std::string dump() const
    {
        return "Entity(" + textureName + " @ " + std::to_string(velocity.x) + "," +
               std::to_string(velocity.y) + ")";
    }

    Entity() = delete;

    Entity(GameState& parent , const Rectangle transf, std::string  texName, const float speed,
           const std::shared_ptr<IMovementStrategy> &moveStrat, const Vector2 velocity = {0, 100})
        : parent(parent),
          trans(transf),
          textureName(std::move(texName)),
          velocity(velocity), speedStat(speed), movementStrategy(moveStrat)
    {
    }
};