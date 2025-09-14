#pragma once
#include <memory>
#include <string>

#include "MovementStrategies.h"
#include "raylib.h"
#include "raymath.h"

struct Entity
{
    Rectangle trans = {0, 0, 32, 32};
    std::string textureName = {"entity"};
    Vector2 velocity = {0, 0};
    int speedStat = 320;  // for player

    std::shared_ptr<IMovementStrategy> movementStrategy = nullptr;

    bool isActive = true;  // for pooling
    float lifetime = 0.0f;

    std::string dump()
    {
        return "Entity(" + textureName + " @ " + std::to_string(velocity.x) + "," +
               std::to_string(velocity.y) + ")";
    }

    Entity() = default;

    Entity(Rectangle transf, std::string texName, int speed,
           std::shared_ptr<IMovementStrategy> moveStrat)
        : trans(transf),
          textureName(texName),
          speedStat(speed),
          movementStrategy(moveStrat)
    {
    }
};