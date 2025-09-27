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

PlayerEmittingStrategy::PlayerEmittingStrategy() {
    fireRate = 0.1f;
}

void PlayerEmittingStrategy::emitImp(const Entity &entity, float delta)
{
    auto& state = entity.parent;
    state.addEntity({entity.trans.x + (3), entity.trans.y - 12, 6, 12}, "res/particle_2.png",
        std::make_shared<LinearMovement>(), 100, {0, -500}).bloomy=true;

    state.addEntity({entity.trans.x + entity.trans.width - (4) - (3), entity.trans.y - 12, 6,12}, "res/particle_2.png",
        std::make_shared<LinearMovement>(), 100, {0, -500}).bloomy=true;


}
