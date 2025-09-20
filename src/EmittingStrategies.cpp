//
// Created by axxy on 18.09.2025.
//

#include "EmittingStrategies.h"

#include <iostream>

#include "Entity.h"
#include "LevelState.h"

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
    state.addEntity({entity.trans.x, entity.trans.y, 96, 96}, "res/aster.png",
        std::make_shared<LinearMovement>(), 100, {0, -100});



}