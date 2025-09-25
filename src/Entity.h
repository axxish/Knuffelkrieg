#pragma once
#include <memory>
#include <string>
#include <utility>

#include "EmittingStrategies.h"
#include "MovementStrategies.h"
#include "raylib.h"
#include "raymath.h"
#include "EmittingStrategies.h"
struct LevelState;


enum class EntityType {
    player = 0, bullet = 1, enemy = 2
};

struct Entity
{
    LevelState& parent;

    EntityType type = EntityType::enemy;

    Rectangle trans = {0, 0, 32, 32};
    std::string textureName = {"entity"};
    bool bloomy = false;
    Vector2 velocity = {0, 0};
    float speedTarget = 220;

    std::shared_ptr<IMovementStrategy> movementStrategy = nullptr;
    std::shared_ptr<IEmittingStrategy> emittingStrategy = nullptr;
    bool flaggedForRemoval = false;
    bool isFiring = true;

    float lifetime = 0.0f;

    // --- Utility Function ---
    [[nodiscard]] std::string dump() const
    {
        return "Entity(" + textureName + " @ " + std::to_string(velocity.x) + "," +
               std::to_string(velocity.y) + ")";
    }



    Entity() = delete;


    Entity(LevelState& parent_ref, const Rectangle rect, std::string texName, const float speed,
           const std::shared_ptr<IMovementStrategy>& moveStrat, const Vector2 vel = {0, 100})
        : parent(parent_ref),
          trans(rect),
          textureName(std::move(texName)), // Always move from a by-value parameter
          velocity(vel),
          speedTarget(speed),
          movementStrategy(moveStrat)
    {
    }

    Entity& operator=(Entity&& other) noexcept
    {
        // Prevent self-assignment
        if (this == &other) {
            return *this;
        }

        // NOTE: The reference member 'parent' cannot be reseated.
        // It will retain its original reference. This is a fundamental
        // property of references in C++. If you must assign between
        // entities, you must ensure they share the same parent context.
        // You could add an assert here if that's an invariant:
        // assert(&parent == &other.parent);

        trans = other.trans;
        textureName = std::move(other.textureName);
        velocity = other.velocity;
        speedTarget = other.speedTarget;

        if (other.movementStrategy)movementStrategy = std::move(other.movementStrategy);
        if (other.emittingStrategy)emittingStrategy = std::move(other.emittingStrategy);
        flaggedForRemoval = other.flaggedForRemoval;
        lifetime = other.lifetime;

        return *this;
    }


    Entity(const Entity& other) = default;


};