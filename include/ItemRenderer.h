#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

class ItemRenderer {
public:
    ItemRenderer();  // Constructor
    void addItem(Item* item);  // Añadir ítem al renderizador
    void render(sf::RenderWindow& window, const sf::View& view);  // Dibujar los ítems en la vista del jugador

private:
    Item* items[3];  // Suponiendo que solo hay 3 ítems (ajustar según sea necesario)
    sf::Texture swordTexture;
    sf::Texture helmetTexture;
    sf::Texture armorTexture;
};
