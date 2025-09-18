//
// Created by axxy on 18.09.2025.
//
#include <unordered_map>
#include <string>
#include "raylib.h"

struct Graphics
{
    std::unordered_map<std::string, Texture2D> textures;

    void loadTexture(const std::string &name);

    void drawTexture(const std::string &name, const Rectangle pos);
};