//
// Created by axxy on 18.09.2025.
//

#include "Graphics.h"

#include <iostream>

void Graphics::loadTexture(const std::string &name) {
    if (!textures.contains(name)) {
        textures[name] = LoadTexture(name.c_str());
    }

}

Texture2D &Graphics::getTexture(const std::string &name) {
    if (textures.contains(name)) {
        return textures[name];
    }


    loadTexture(name);
    return textures[name];
}

void Graphics::drawTexture(const std::string &name, const Rectangle pos) {
    if (textures.contains(name)) {
        DrawTexturePro(textures[name],
                       {
                           0, 0, static_cast<float>(textures[name].width),
                           static_cast<float>(textures[name].height)
                       },
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    } else {
        loadTexture(name);
        DrawTexturePro(textures[name],
                       {
                           0, 0, static_cast<float>(textures[name].width),
                           static_cast<float>(textures[name].height)
                       },
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    }
}

void Graphics::loadShader(const std::string &name, const std::string& pathToFragment, const std::string& pathToVertex) {
    if (pathToVertex.empty()){
        shaders[name] = LoadShader(nullptr, pathToFragment.c_str());
    }
    else {
        shaders[name] = LoadShader(pathToVertex.c_str(), pathToFragment.c_str());
    }
}

Shader & Graphics::getShader(const std::string &name) {
    if (shaders.contains(name)) {
        return shaders[name];
    }

    std::cout<<"Shader not loaded: "<<name<<std::endl;
    shaders[""] = LoadShader(nullptr, nullptr);

    return shaders[""];
}
