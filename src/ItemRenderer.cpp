#include "ItemRenderer.h"
#include <iostream>

/*ItemRenderer::ItemRenderer() {
    // Inicializamos los ítems en nullptr
    for (int i = 0; i < 3; ++i) {
        items[i] = nullptr;
    }

    // Cargar las texturas de cada ítem
    if (!swordTexture.loadFromFile("UI/Espada.png")) {
        std::cerr << "Error loading sword texture" << std::endl;
    }
    if (!helmetTexture.loadFromFile("UI/Casco.png")) {
        std::cerr << "Error loading helmet texture" << std::endl;
    }
    if (!armorTexture.loadFromFile("UI/Armadura.png")) {
        std::cerr << "Error loading armor texture" << std::endl;
    }
}

void ItemRenderer::addItem(Item* item) {
    for (int i = 0; i < 3; ++i) {
        if (items[i] == nullptr) {
            items[i] = item;

            // Asigna la textura correcta en función del tipo de ítem
            if (item->getType() == ItemType::SWORD) {
                item->getSprite().setTexture(swordTexture);
            } else if (item->getType() == ItemType::HELMET) {
                item->getSprite().setTexture(helmetTexture);
            } else if (item->getType() == ItemType::ARMOR) {
                item->getSprite().setTexture(armorTexture);

            }
            break;
        }
    }
}

void ItemRenderer::render(sf::RenderWindow& window, const sf::View& view) {
    float scaleFactor = 0.04f;

    // Iteramos sobre los ítems y los dibujamos si han sido recogidos
    for (int i = 0; i < 3; ++i) {
        if (items[i] != nullptr && items[i]->isRecogido()) {
            sf::Sprite sprite = items[i]->getSprite();
            sprite.setScale(scaleFactor, scaleFactor);

            // Calcula la posición de cada ítem en el HUD
            if (items[i]->getType() == ItemType::SWORD) {
                sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 48.0f,
                                   view.getCenter().y + view.getSize().y / 2 - sprite.getGlobalBounds().height - 353.0f);
            } else if (items[i]->getType() == ItemType::HELMET) {
                sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 100.0f,
                                   view.getCenter().y + view.getSize().y / 2 - sprite.getGlobalBounds().height - 353.0f);
            } else if (items[i]->getType() == ItemType::ARMOR) {
                sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 150.0f,
                                   view.getCenter().y + view.getSize().y / 2 - sprite.getGlobalBounds().height - 353.0f);
            }

            window.draw(sprite);
        }
    }
}
*/
