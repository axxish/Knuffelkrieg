
#include "MovementStrategies.h"

#include "Entity.h"

bool IMovementStrategy::isComplete() const {
    return false;
}

void LinearMovement::update(Entity& entity, const float deltaTime)
{

    entity.trans.x += entity.velocity.x * deltaTime;
    entity.trans.y += entity.velocity.y * deltaTime;
}


SineMovement::SineMovement(const float amp, const float freq, const float phaseShift, Vector2 waveAxis)
    : waveAxis(waveAxis), amplitude(amp), frequency(freq), phase(phaseShift)
{
    if (Vector2LengthSqr(waveAxis) > 0) {
        waveAxis = Vector2Normalize(waveAxis);
    }

}

void SineMovement::update(Entity& entity, const float deltaTime)
{
    const Vector2 base_direction = Vector2Normalize(entity.velocity);


    auto [x, y] = Vector2Scale(base_direction, entity.speedTarget);


    const float lateral_speed = amplitude * frequency * cos(entity.lifetime * frequency + phase);


    Vector2 total_velocity;

    total_velocity.x = x + waveAxis.x * lateral_speed;
    total_velocity.y = y + waveAxis.y * lateral_speed;

    entity.trans.x += total_velocity.x * deltaTime;
    entity.trans.y += total_velocity.y * deltaTime;
}

void TargetPlayerMovement::update(Entity& entity, const float deltaTime)
{
    if (!isInitialized)
    {
        const Vector2 startPos = {entity.trans.x, entity.trans.y};

        direction = Vector2Normalize(Vector2Subtract(targetPos, startPos));

        isInitialized = true;
    }

    entity.velocity = Vector2Scale(direction, entity.speedTarget);

    entity.trans.x += entity.velocity.x * deltaTime;
    entity.trans.y += entity.velocity.y * deltaTime;
}

MoveToPoint::MoveToPoint(const Vector2 destination): destinationPos(destination), isInitialized(false), hasArrived(false) {}

void MoveToPoint::update(Entity &entity, const float deltaTime) {

    if (hasArrived) {
        return;
    }

    if (!isInitialized)
    {
        const Vector2 startPos = {entity.trans.x, entity.trans.y};
        direction = Vector2Normalize(Vector2Subtract(destinationPos, startPos));
        isInitialized = true;
    }

    const Vector2 currentPos = {entity.trans.x, entity.trans.y};


    const float distanceToTarget = Vector2Distance(currentPos, destinationPos);

    // Calculate how far we will move this frame.

    // Check if we will reach or overshoot the target this frame.
    if (const float moveDistanceThisFrame = entity.speedTarget * deltaTime; distanceToTarget <= moveDistanceThisFrame)
    {
        // We have arrived. Snap the position directly to the destination
        // to prevent overshooting, and set the flag.
        entity.trans.x = destinationPos.x;
        entity.trans.y = destinationPos.y;
        hasArrived = true;
    }
    else
    {
        // We are not there yet, so continue moving.
        auto [x, y] = Vector2Scale(direction, entity.speedTarget);
        entity.trans.x += x * deltaTime;
        entity.trans.y += y * deltaTime;
    }
}


bool MoveToPoint::isComplete() const {
    return hasArrived;
}

ChainedMovement::ChainedMovement(std::vector<MovementPhase> &&phaseList): phases(std::move(phaseList)), currentPhaseIndex(0), timeInCurrentPhase(0.0f) {}
void ChainedMovement::update(Entity& entity, const float deltaTime)
{
    if (currentPhaseIndex >= phases.size()) {
        return;
    }

    if (timeInCurrentPhase == 0.0f) {
        entity.speedTarget = phases[currentPhaseIndex].newSpeedTarget;
    }

    const MovementPhase& currentPhase = phases[currentPhaseIndex];
    IMovementStrategy* currentStrategy = currentPhase.strategy.get();

    if (currentStrategy) {
        currentStrategy->update(entity, deltaTime);
    }

    timeInCurrentPhase += deltaTime;

    // --- NEW, HIERARCHICAL TRANSITION LOGIC ---
    bool transition = false;

    // Rule 1: Check duration first, if it's specified.
    if (currentPhase.duration > 0.0f)
    {
        if (timeInCurrentPhase >= currentPhase.duration) {
            transition = true;
        }
    }
    // Rule 2: If duration is not specified (<= 0), check for completion.
    else
    {
        if (currentStrategy && currentStrategy->isComplete()) {
            transition = true;
        }
    }

    if (transition) {
        currentPhaseIndex++;
        timeInCurrentPhase = 0.0f;

        if (currentPhaseIndex < phases.size()) {
            entity.speedTarget = phases[currentPhaseIndex].newSpeedTarget;
        }
    }
}

