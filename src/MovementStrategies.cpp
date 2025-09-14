
#include "MovementStrategies.h"

#include "Entity.h"

void LinearMovement::update(Entity& entity, float deltaTime)
{
    entity.trans.x += entity.velocity.x * deltaTime;
    entity.trans.y += entity.velocity.y * deltaTime;
}

SineMovement::SineMovement(float amp, float freq, float phaseShift)
    : amplitude(amp), frequency(freq), phase(phaseShift)
{
}

void SineMovement::update(Entity& entity, float deltaTime)
{
    Vector2 perpendicular_dir = {-entity.velocity.y, entity.velocity.x};
    perpendicular_dir = Vector2Normalize(perpendicular_dir);

    float lateral_speed = amplitude * frequency * cos(entity.lifetime * frequency + phase);

    Vector2 total_velocity;
    total_velocity.x = entity.velocity.x + perpendicular_dir.x * lateral_speed;
    total_velocity.y = entity.velocity.y + perpendicular_dir.y * lateral_speed;

    entity.trans.x += total_velocity.x * deltaTime;
    entity.trans.y += total_velocity.y * deltaTime;
};

void TargetPlayerMovement::update(Entity& entity, float deltaTime)
{
    if (!isInitialized)
    {
        Vector2 startPos = {entity.trans.x, entity.trans.y};

        Vector2 direction = Vector2Subtract(targetPos, startPos);

        direction = Vector2Normalize(direction);

        entity.velocity = Vector2Scale(direction, speed);

        isInitialized = true;
    }

    entity.trans.x += entity.velocity.x * deltaTime;
    entity.trans.y += entity.velocity.y * deltaTime;
}