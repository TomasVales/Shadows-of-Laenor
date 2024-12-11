#include "pausa.h"
#include <iostream>

pausa::pausa() : isActive(false), selectedOptionIndex(0), exitOptionSelected(false)
{
    if (!fuente.loadFromFile("fuentes/im.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
    }

    if (!backgroundTexture.loadFromFile("UI/fondo.png")) {
        std::cerr << "Error al cargar la imagen de fondo." << std::endl;
    } else {
        backgroundSprite.setTexture(backgroundTexture);
    }

    // Configuración de textos
    configurarTexto(texto1, "Reanudar");
    configurarTexto(texto2, "Salir");
}

void pausa::configurarTexto(sf::Text& texto, const std::string& contenido)
{
    texto.setFont(fuente);
    texto.setString(contenido);
    texto.setCharacterSize(24);
    texto.setFillColor(sf::Color::White);
}

void pausa::toggle()
{
    isActive = !isActive;
}

void pausa::update(const sf::Event& event, bool& isPaused, bool& isMenuActive)
{
    if (!isActive || keyCooldownClock.getElapsedTime() < keyCooldown) return;

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                moveUp();
                break;
            case sf::Keyboard::Down:
                moveDown();
                break;
            case sf::Keyboard::Enter:
                handleSelection(isPaused, isMenuActive);
                break;
            default:
                return;
        }
        keyCooldownClock.restart();
    }
}

void pausa::draw(sf::RenderWindow& window)
{
    sf::Vector2f center = window.getView().getCenter();

    // Posicionar fondo y dibujar
    backgroundSprite.setPosition(center.x - backgroundSprite.getGlobalBounds().width / 2 - 20,
                                 center.y - backgroundSprite.getGlobalBounds().height / 2 - 40);
    window.draw(backgroundSprite);

    // Posicionar y dibujar opciones
    texto1.setPosition(center.x - texto1.getGlobalBounds().width / 2, center.y - 25);
    texto2.setPosition(center.x - texto2.getGlobalBounds().width / 2, center.y + 25);

    window.draw(texto1);
    window.draw(texto2);
}

void pausa::moveUp()
{
    selectedOptionIndex = (selectedOptionIndex == 0) ? 1 : 0;
    updateOptionColors();
}

void pausa::moveDown()
{
    selectedOptionIndex = (selectedOptionIndex == 1) ? 0 : 1;
    updateOptionColors();
}

void pausa::updateOptionColors()
{
    texto1.setFillColor(selectedOptionIndex == 0 ? sf::Color(255, 215, 0) : sf::Color::White);
    texto2.setFillColor(selectedOptionIndex == 1 ? sf::Color(255, 215, 0) : sf::Color::White);
}

void pausa::handleSelection(bool& isPaused, bool& isMenuActive)
{
    if (selectedOptionIndex == 0) {
        isPaused = false;
        isActive = false;
    } else {
        exitOptionSelected = true;
        isActive = false; // Desactivar menú después de seleccionar salir
    }
}

bool pausa::isExitOptionSelected() const
{
    return exitOptionSelected;
}

void pausa::resetExitOption()
{
    exitOptionSelected = false;
}
