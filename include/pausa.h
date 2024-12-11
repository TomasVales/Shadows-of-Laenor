#ifndef PAUSA_H
#define PAUSA_H

#include <SFML/Graphics.hpp>
#include <string>

class pausa
{
public:
    pausa();

    // Alternar estado de pausa
    void toggle();

    // Actualizar el men� de pausa
    void update(const sf::Event& event, bool& isPaused, bool& isMenuActive);

    // Dibujar el men� de pausa
    void draw(sf::RenderWindow& window);

    // Comprobar si se seleccion� la opci�n "Salir"
    bool isExitOptionSelected() const;

    // Reiniciar el estado de selecci�n de "Salir"
    void resetExitOption();

private:
    // M�todos auxiliares
    void moveUp();
    void moveDown();
    void updateOptionColors();
    void handleSelection(bool& isPaused, bool& isMenuActive);
    void configurarTexto(sf::Text& texto, const std::string& contenido);

    // Variables de estado
    bool isActive;
    int selectedOptionIndex;
    bool exitOptionSelected;

    // SFML elementos gr�ficos
    sf::Font fuente;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text texto1; // Opci�n "Reanudar"
    sf::Text texto2; // Opci�n "Salir"

    // Control de entrada (enfriamiento)
    sf::Clock keyCooldownClock;
    const sf::Time keyCooldown = sf::milliseconds(200); // Tiempo de enfriamiento entre pulsaciones
};

#endif // PAUSA_H
