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
    state.addEntity({entity.trans.x + (3*2), entity.trans.y, 8*2, 8*2}, "res/particle_1.png",
        std::make_shared<LinearMovement>(), 100, {0, -1000});

    state.addEntity({entity.trans.x + entity.trans.width - (8*2) - (3*2), entity.trans.y, 8*2 ,8*2}, "res/particle_1.png",
        std::make_shared<LinearMovement>(), 100, {0, -1000});


}
