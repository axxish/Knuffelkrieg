#pragma once
#include <memory>
#include <vector>

#include "raylib.h"
#include "raymath.h"
struct Entity;

class IMovementStrategy
{
   public:
    virtual ~IMovementStrategy() = default;

    virtual void update(Entity& entity, float deltaTime) = 0;
};

class LinearMovement final : public IMovementStrategy
{
   public:
    void update(Entity& entity, float deltaTime) override;
};

class SineMovement final : public IMovementStrategy
{
   public:
    SineMovement(float amp, float freq, float phaseShift, Vector2 waveAxis);



    void update(Entity& entity, float deltaTime) override;
    Vector2 waveAxis = {0,0};
    float amplitude = 0;
    float frequency = 0;
    float phase = 0.0f;
};

class TargetPlayerMovement final : public IMovementStrategy
{
   private:
    Vector2 targetPos;
    Vector2 direction = {0, 0}; 

    bool isInitialized = false;

   public:
    explicit TargetPlayerMovement(const Vector2 playerPosAtSpawn)
        : targetPos(playerPosAtSpawn), isInitialized(false)
    {
    }

    void update(Entity& entity, float deltaTime) override;
};

class MoveToPoint final : public IMovementStrategy
{
private:
    Vector2 destinationPos; // The target destination point.
    Vector2 direction = {0, 0};

    bool isInitialized = false;
    bool hasArrived = false; // A flag to indicate we've reached the destination.

public:
    // The constructor takes the destination coordinates.
    explicit MoveToPoint(Vector2 destination);

    void update(Entity& entity, float deltaTime) override;

    // A helper function to check if the movement is complete.
    [[nodiscard]] bool hasReachedDestination() const;
};


struct MovementPhase
{
    std::unique_ptr<IMovementStrategy> strategy;
    float duration; // How long this phase should last in seconds.
    float newSpeedStat; // The speed the entity should have during this phase.
};

// A "super" strategy that executes a sequence of other strategies.
class ChainedMovement final : public IMovementStrategy
{
private:
    std::vector<MovementPhase> phases;
    int currentPhaseIndex;
    float timeInCurrentPhase;

public:
    // The constructor takes a list of phases.
    explicit ChainedMovement(std::vector<MovementPhase>&& phaseList);

    void update(Entity& entity, float deltaTime) override;
};

