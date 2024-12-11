#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

class ItemRenderer {
public:
    ItemRenderer();  // Constructor
    void addItem(Item* item);  // A�adir �tem al renderizador
    void render(sf::RenderWindow& window, const sf::View& view);  // Dibujar los �tems en la vista del jugador

private:
    Item* items[3];  // Suponiendo que solo hay 3 �tems (ajustar seg�n sea necesario)
    sf::Texture swordTexture;
    sf::Texture helmetTexture;
    sf::Texture armorTexture;
};
