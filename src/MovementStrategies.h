#pragma once
#include "raylib.h"
struct Entity;

class IMovementStrategy
{
   public:
    virtual ~IMovementStrategy() = default;

    virtual void update(Entity& entity, float deltaTime) = 0;
};

class LinearMovement : public IMovementStrategy
{
   public:
    void update(Entity& entity, float deltaTime) override;
};

class SineMovement : public IMovementStrategy
{
   public:
    SineMovement(float amp, float freq, float phaseShift);

    void update(Entity& entity, float deltaTime) override;
    float amplitude = 0;
    float frequency = 0;
    float phase = 0.0f;
};

class TargetPlayerMovement : public IMovementStrategy
{
   private:
    Vector2 targetPos;
    float speed;
    bool isInitialized = false;

   public:
    TargetPlayerMovement(Vector2 playerPosAtSpawn, float enemySpeed)
        : targetPos(playerPosAtSpawn), speed(enemySpeed), isInitialized(false)
    {
    }

    void update(Entity& entity, float deltaTime) override;
};