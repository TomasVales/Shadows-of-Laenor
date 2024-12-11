#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

class Resources
{
public:
    static std::unordered_map<std::string , sf::Texture> textures;

};
