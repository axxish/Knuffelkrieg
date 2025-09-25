//
// Created by axxy on 18.09.2025.
//

#include <iostream>
#include <memory>

#include "EmittingStrategies.h"
#include "Entity.h"
#include "LevelState.h"

#include "colourDefs.h"
#include "MovementStrategies.h"
#include "raylib.h"


Entity &LevelState::addEntity(Rectangle trans, const std::string &texName,
                              const std::shared_ptr<IMovementStrategy> &moveStrat, float speedTarget,
                              Vector2 velocity) {
    return entities.emplace_back(*this, trans, texName, speedTarget, moveStrat, velocity);
}



void LevelState::onInit() {

    backgroundRenderTexture = LoadRenderTexture(static_cast<int>(playArea.width), static_cast<int>(playArea.height));
    lightRenderTexture = LoadRenderTexture(static_cast<int>(playArea.width), static_cast<int>(playArea.height));
    entityRenderTexture = LoadRenderTexture(static_cast<int>(playArea.width), static_cast<int>(playArea.height));
    bloomyRenderTexture = LoadRenderTexture(static_cast<int>(playArea.height), static_cast<int>(playArea.height));


    camera = {{playArea.width / 2, playArea.height / 2}, {0, 0}, 0, 1};

    auto &player = addEntity({0, playArea.height / 2, 32, 32}, "res/Ship_1bright.png",
                             std::make_shared<LinearMovement>(), 220, {0, 0});

    player.trans.x = 0 - player.trans.width/2;
    player.trans.y = playArea.height/2 ;

    player.emittingStrategy = std::make_shared<PlayerEmittingStrategy>();

    playerIndex = entities.size() - 1;

    gfx.loadShader("bloom", "res/bloom.frag");


}

Vector2 LevelState::getMovementVector() {
    Vector2 movement;
    movement.x = 0;
    movement.y = 0;
    if (IsKeyDown(KEY_UP))
        movement.y -= 1;
    if (IsKeyDown(KEY_DOWN))
        movement.y += 1;
    if (IsKeyDown(KEY_LEFT))
        movement.x -= 1;
    if (IsKeyDown(KEY_RIGHT))
        movement.x += 1;
    return Vector2Normalize(movement);
}

void LevelState::onUpdate() {
    const float delta = GetFrameTime();
    loggerTimer += delta;

    if (playerIndex != -1) {

        auto &player = entities[playerIndex];
        player.isFiring = false;

        auto [x, y] = getMovementVector();
        auto speed = player.speedTarget;
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            speed = speed / 2;
        }

        if (IsKeyDown(KEY_Z)) {
            player
            .isFiring = true;
        }

        player.velocity.x = x * speed;
        player.velocity.y = y * speed;

        scrollPos += delta;


    }



    for (auto & entity : entities) {
        //culling logic
        if (!CheckCollisionRecs(cullingArea, entity.trans)) {
            entity.flaggedForRemoval = true;
        }

        entity.lifetime += delta;
        if (entity.movementStrategy != nullptr) {
            entity.movementStrategy->update(entity, delta);
        }
        if (entity.emittingStrategy != nullptr && entity.isFiring) {
            entity.emittingStrategy->update(entity, delta);
        }
    }

    for (unsigned int i = entities.size()-1; i > 0; i--) {
        if (entities[i].flaggedForRemoval) {
            entities[i] = std::move(entities.back());
            entities.pop_back();
        }
    }
}

void LevelState::onRender() {


    BeginTextureMode(backgroundRenderTexture);
    ClearBackground(SPACE_BLACK);
    BeginMode2D(camera);
    gfx.drawTexture("res/planet15.png", {-50, static_cast<float>((-100 + (scrollPos * 0.25))), 128*2, 128*2});

    EndMode2D();
    EndTextureMode();


    BeginTextureMode(lightRenderTexture);
    ClearBackground(BLACK);
    BeginMode2D(camera);
    BeginBlendMode(BLEND_ADDITIVE);
    for (const auto &entity: entities) {

        if (entity.bloomy) {

            DrawCircleGradient((entity.trans.x+entity.trans.width/2) , (entity.trans.y+entity.trans.width/2)+1,
                entity.trans.width ,
                {255, 255, 255, 100}, BLANK);
            DrawCircleGradient((entity.trans.x+entity.trans.width/2) , (entity.trans.y+entity.trans.width/2)+1,
                entity.trans.width*3,
                {255, 255, 255, 25}, BLANK);
            DrawCircleGradient((entity.trans.x+entity.trans.width/2) , (entity.trans.y+entity.trans.width/2)+1,
                    entity.trans.width*5,
                    {255, 255, 255, 5}, BLANK);

        }
        else {
            DrawCircleGradient((entity.trans.x+entity.trans.width/2) , (entity.trans.y+entity.trans.width/2)+1,
                (entity.trans.width*1.5 - (rand()%5)),
                {255, 255, 255, 70}, BLANK);
        }

    }
    EndBlendMode();
    EndMode2D();
    EndTextureMode();



    BeginTextureMode(entityRenderTexture);
    ClearBackground(BLANK);
    BeginMode2D(camera);
        for (const auto &entity: entities) {

            gfx.drawTexture(entity.textureName, entity.trans);
        }

    EndMode2D();
    EndTextureMode();

    BeginTextureMode(bloomyRenderTexture);
    ClearBackground(BLACK);
    BeginMode2D(camera);
    for (const auto &entity: entities) {
        if (entity.bloomy)gfx.drawTexture(entity.textureName, entity.trans);
    }

    EndMode2D();
    EndTextureMode();



    DrawTexturePro(backgroundRenderTexture.texture, {
                       0, 0,
                       static_cast<float>(backgroundRenderTexture.texture.width),
                       -static_cast<float>(backgroundRenderTexture.texture.height)
                   },
                   {
                       0, 0, static_cast<float>(backgroundRenderTexture.texture.width*2),
                       static_cast<float>(backgroundRenderTexture.texture.height*2)
                   },
                   {0,0}, 0, {150,150,150, 255});

    BeginBlendMode(BLEND_ADDITIVE);
    DrawTexturePro(lightRenderTexture.texture, {
                       0, 0,
                       static_cast<float>(backgroundRenderTexture.texture.width),
                       -static_cast<float>(backgroundRenderTexture.texture.height)
                   },
                   {
                       0, 0, static_cast<float>(backgroundRenderTexture.texture.width*2),
                       static_cast<float>(backgroundRenderTexture.texture.height*2)
                   },
                   {0,0}, 0, WHITE);
    EndBlendMode();

    DrawTexturePro(entityRenderTexture.texture, {
                    -2, -2,
                    static_cast<float>(backgroundRenderTexture.texture.width),
                    -static_cast<float>(backgroundRenderTexture.texture.height)
                },
                {
                    0, 0, static_cast<float>(backgroundRenderTexture.texture.width*2),
                    static_cast<float>(backgroundRenderTexture.texture.height*2)
                },
                {0,0}, 0, BLACK);

    DrawTexturePro(entityRenderTexture.texture, {
                       0, 0,
                       static_cast<float>(backgroundRenderTexture.texture.width),
                       -static_cast<float>(backgroundRenderTexture.texture.height)
                   },
                   {
                       0, 0, static_cast<float>(backgroundRenderTexture.texture.width*2),
                       static_cast<float>(backgroundRenderTexture.texture.height*2)
                   },
                   {0,0}, 0, WHITE);

    BeginShaderMode(gfx.getShader("bloom"));
    BeginBlendMode(BLEND_ADDITIVE);
    DrawTexturePro(bloomyRenderTexture.texture, {
                       0, 0,
                       static_cast<float>(backgroundRenderTexture.texture.width),
                       -static_cast<float>(backgroundRenderTexture.texture.height)
                   },
                   {
                       0, 0, static_cast<float>(backgroundRenderTexture.texture.width*2),
                       static_cast<float>(backgroundRenderTexture.texture.height*2)
                   },
                   {0,0}, 0, WHITE);
    EndBlendMode();
    EndShaderMode();
    if (loggerTimer >= loggerCooldown) {
        std::cout << "\033[2J";
        std::cout << "culling area: " << cullingArea.x << " " << cullingArea.y << " " << cullingArea.width <<" " << cullingArea.height <<  "\n";
        std::cout << "entity count : " << entities.size() << std::endl;
        loggerTimer = 0;
    }
}
