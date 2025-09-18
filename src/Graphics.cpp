//
// Created by axxy on 18.09.2025.
//

#include "Graphics.h"
void Graphics::loadTexture(const std::string &name)
{
    textures[name] = LoadTexture(name.c_str());
}
void Graphics::drawTexture(const std::string &name, const Rectangle pos)
{
    if (textures.find(name) != textures.end())
    {
        DrawTexturePro(textures[name],
                       {0, 0, static_cast<float>(textures[name].width),
                        static_cast<float>(textures[name].height)},
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    }
    else
    {
        loadTexture(name);
        DrawTexturePro(textures[name],
                       {0, 0, static_cast<float>(textures[name].width),
                        static_cast<float>(textures[name].height)},
                       {pos.x, pos.y, pos.width, pos.height}, Vector2{0, 0}, 0.0f, WHITE);
    }
}