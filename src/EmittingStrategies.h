//
// Created by axxy on 18.09.2025.
//

#pragma once
#include <iostream>
#include <ostream>

#include "raylib.h"

struct Entity;

//not really an interface but whatever
class IEmittingStrategy
{
public:
    virtual ~IEmittingStrategy();
    float fireRate = 1.0f;
    float cooldownTimer = 0.0f;
    Vector2 offset = {0, 0};

    void update(const Entity& entity, float delta);


    virtual void emitImp(const Entity& entity, float delta)=0;

};


class PlayerEmittingStrategy final : public IEmittingStrategy
{

    void emitImp(const Entity& entity, float delta) override;
};



