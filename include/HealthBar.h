#pragma once

#include <SFML/Graphics.hpp>

class HealthBar {
public:
    HealthBar(const std::string& texturePath);
    void setHealth(float health); // Establece la salud actual
    void draw(sf::RenderWindow& window , sf::View& vista); // Dibuja la barra de vida
    sf::Vector2f getBottomPosition() const; // Declaraci�n del m�todo
    void updatePosition(const sf::View& view); // Nueva funci�n para actualizar la posici�n de la barra de vida


private:
    sf::Texture texture; // Textura de la barra de vida
    sf::Sprite sprite; // Sprite que representa la barra de vida
    float maxHealth; // Salud m�xima
    float currentHealth; // Salud actual
};


