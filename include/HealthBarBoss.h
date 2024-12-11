#pragma once
#include <SFML/Graphics.hpp>

class HealthBarBoss {
public:
    HealthBarBoss(const std::string& textureBasePath, const std::string& prefix, float maxHealth);
    void setHealth(float health); // Establece la salud actual
    void draw(sf::RenderWindow& window, sf::View& vista); // Dibuja la barra de vida

private:
    sf::Texture texture; // Textura de la barra de vida
    sf::Sprite sprite; // Sprite que representa la barra de vida
    float maxHealth; // Salud máxima
    float currentHealth; // Salud actual
    std::string textureBasePath; // Ruta base para los archivos de textura
    std::string prefix; // Prefijo del archivo de textura

    void updateTexture(); // Actualiza la textura basada en el estado actual de salud
};
