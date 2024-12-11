#pragma once

#include <SFML/Graphics.hpp>

class HealthBar {
public:
    HealthBar(const std::string& texturePath);
    void setHealth(float health); // Establece la salud actual
    void draw(sf::RenderWindow& window , sf::View& vista); // Dibuja la barra de vida
    sf::Vector2f getBottomPosition() const; // Declaración del método
    void updatePosition(const sf::View& view); // Nueva función para actualizar la posición de la barra de vida


private:
    sf::Texture texture; // Textura de la barra de vida
    sf::Sprite sprite; // Sprite que representa la barra de vida
    float maxHealth; // Salud máxima
    float currentHealth; // Salud actual
};


