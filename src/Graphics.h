//
// Created by axxy on 18.09.2025.
//

#pragma
#include <unordered_map>
#include <string>
#include "raylib.h"



struct ParallaxLayer {
    std::string textureName;
    float speed;
    float scale;
    Vector2 pos;
};

struct Graphics
{



    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Shader> shaders;

    void loadTexture(const std::string &name);

    Texture2D& getTexture(const std::string &name);

    void drawTexture(const std::string &name, Rectangle pos);


    void loadShader(const std::string &name, const std::string& pathToFragment, const std::string& pathToVertex="");

    Shader& getShader(const std::string &name);
};