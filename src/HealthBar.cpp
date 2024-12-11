
#include "HealthBar.h"
#include <iostream>

// Constructor de HealthBar
HealthBar::HealthBar(const std::string& texturePath) {
    maxHealth = 100.0f; // Salud máxima
    currentHealth = maxHealth; // Salud inicial

    // Cargar textura inicial de la barra de vida
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error al cargar la imagen de la barra de vida.\n";
    }

    sprite.setTexture(texture); // Asignar textura al sprite
    sprite.setPosition(20.0f, 20.0f); // Posición inicial en pantalla
}

// Configurar salud actual
void HealthBar::setHealth(float health) {
    currentHealth = health; // Actualizar salud
    if (currentHealth < 0) currentHealth = 0; // Limitar a 0 como mínimo
    if (currentHealth > maxHealth) currentHealth = maxHealth; // Limitar al máximo

    // Cambiar textura según nivel de vida
    int healthIndex = std::min(static_cast<int>((1 - (currentHealth / maxHealth)) * 10), 9); // calculamos el indice segund la vida
    std::string texturePath = "UI/sprite_0" + std::to_string(healthIndex) + ".png"; // Ruta de textura basada en salud

    // Cargar nueva textura según vida
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error al cargar la imagen de la barra de vida.\n";
    }

    sprite.setTexture(texture); // Actualizar textura en el sprite
    float healthPercentage = currentHealth / maxHealth; // Calcular porcentaje de vida
    sprite.setScale(healthPercentage, 1.0f); // Escalar solo el ancho según la vida
}

// Dibujar barra de vida en la ventana
void HealthBar::draw(sf::RenderWindow& window , sf::View& vista) {
    float scaleFactorX = 0.15f; // Escala horizontal para ajustar ancho
    float scaleFactorY = 0.15f; // Escala vertical para ajustar alto
    sprite.setScale(scaleFactorX, scaleFactorY); // Aplicar escala

    // Posicionar barra en la esquina superior izquierda de la vista
    sprite.setPosition(vista.getCenter().x - vista.getSize().x / 2,
                       vista.getCenter().y - vista.getSize().y / 2 - 30);

    window.draw(sprite); // Dibujar sprite en la ventana
}

// Obtener la posición inferior de la barra
sf::Vector2f HealthBar::getBottomPosition() const {
    return sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height);
}

// Actualizar posición según la vista
void HealthBar::updatePosition(const sf::View& view) {
    sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 20.0f, // Ajustar a la esquina superior izquierda
                       view.getCenter().y - view.getSize().y / 2 + 20.0f);
}

