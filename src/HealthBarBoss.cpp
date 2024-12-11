#include "HealthBarBoss.h"
#include <iostream>
#include <sstream>
#include <iomanip>

HealthBarBoss::HealthBarBoss(const std::string& textureBasePath, const std::string& prefix, float maxHealth)
    : textureBasePath(textureBasePath), prefix(prefix), maxHealth(maxHealth), currentHealth(maxHealth) {
    updateTexture();
}

void HealthBarBoss::setHealth(float health) {
    currentHealth = std::clamp(health, 0.0f, maxHealth); // Asegurar que la salud esté dentro de los límites
    updateTexture();
}

void HealthBarBoss::updateTexture() {
    int healthIndex = static_cast<int>((1 - (currentHealth / maxHealth)) * 15);
    healthIndex = std::clamp(healthIndex, 0, 15); // Asegurar que el índice esté en el rango

    std::ostringstream texturePath;
    texturePath << textureBasePath << "/" << prefix << "_"
                << std::setw(2) << std::setfill('0') << healthIndex << ".png";

    if (!texture.loadFromFile(texturePath.str())) {
        std::cerr << "Error al cargar la imagen: " << texturePath.str() << '\n';
    }

    sprite.setTexture(texture);
}

void HealthBarBoss::draw(sf::RenderWindow& window, sf::View& view) {
    sprite.setScale(1.7f, 1.7f);
    sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 240,
                       view.getCenter().y - view.getSize().y / 2 - 30);
    window.draw(sprite);
}
