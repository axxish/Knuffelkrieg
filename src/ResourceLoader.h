//
// Created by axxy on 18.09.2025.
//

#pragma once
#include <unordered_map>
#include <string>
#include "raylib.h"



struct ParallaxLayer {
    std::string textureName;
    float speed;
    float scale;
    Vector2 pos;
};

struct Animation {
    std::string spriteSheetName;
    Vector2 dimensions;
    int frameCount;
    int frameRow;
    float frameDuration;
    bool loop;
};

struct Animator {
    std::string currentAnimation = "";
    int currentFrame = 0;
    float timer = 0;
    bool isPlaying = true;

};

struct ResourceLoader
{


    std::unordered_map<std::string, Texture2D> spritesheets;
    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, Shader> shaders;

    void loadSpriteSheet(const std::string &name);

    void addAnimation(const std::string &name, const Animation &animation);

    Texture2D& getTexture(const std::string &name);

    void drawTexture(const std::string &name, Rectangle pos);


    void loadShader(const std::string &name, const std::string& pathToFragment, const std::string& pathToVertex="");

    Shader& getShader(const std::string &name);
};