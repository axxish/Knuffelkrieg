//
// Created by axxy on 18.09.2025.
//

#include "ResourceLoader.h"

#include <iostream>

void ResourceLoader::loadSpriteSheet(const std::string &name) {
    if (!spritesheets.contains(name)) {
        spritesheets[name] = LoadTexture(name.c_str());
    }

}

Texture2D &ResourceLoader::getTexture(const std::string &name) {
    if (spritesheets.contains(name)) {
        return spritesheets[name];
    }


    loadSpriteSheet(name);
    return spritesheets[name];
}

void ResourceLoader::drawTexture(const std::string &name, const Rectangle pos) {
    if (spritesheets.contains(name)) {
        DrawTexturePro(spritesheets[name],
                       {
                           0, 0, static_cast<float>(spritesheets[name].width),
                           static_cast<float>(spritesheets[name].height)
                       },
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    } else {
        loadSpriteSheet(name);
        DrawTexturePro(spritesheets[name],
                       {
                           0, 0, static_cast<float>(spritesheets[name].width),
                           static_cast<float>(spritesheets[name].height)
                       },
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    }
}

void ResourceLoader::loadShader(const std::string &name, const std::string& pathToFragment, const std::string& pathToVertex) {
    if (pathToVertex.empty()){
        shaders[name] = LoadShader(nullptr, pathToFragment.c_str());
    }
    else {
        shaders[name] = LoadShader(pathToVertex.c_str(), pathToFragment.c_str());
    }
}

Shader & ResourceLoader::getShader(const std::string &name) {
    if (shaders.contains(name)) {
        return shaders[name];
    }

    std::cout<<"Shader not loaded: "<<name<<std::endl;
    shaders[""] = LoadShader(nullptr, nullptr);

    return shaders[""];
}
