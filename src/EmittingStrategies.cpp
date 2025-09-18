//
// Created by axxy on 18.09.2025.
//

#include "EmittingStrategies.h"

#include "Entity.h"
#include "GameState.h"
#include <iostream>

IEmittingStrategy::~IEmittingStrategy() = default;
void IEmittingStrategy::update(const Entity &entity, const float delta)
{


    if (cooldownTimer <= 0)
    {
        cooldownTimer = fireRate;
        emitImp(entity, delta);

        return;
    }
    cooldownTimer -= delta;
}
void PlayerEmittingStrategy::emitImp(const Entity &entity, float delta)
{
    auto& state = entity.parent;
    auto bullet = Entity(state, {entity.trans.x, entity.trans.x, 96, 96}, "res/aster.png", 100, std::make_shared<LinearMovement>(), {0, 0});

    state.enemyEnt.insert({std::to_string(rand()), bullet});


}