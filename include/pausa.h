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

    // Actualizar el menú de pausa
    void update(const sf::Event& event, bool& isPaused, bool& isMenuActive);

    // Dibujar el menú de pausa
    void draw(sf::RenderWindow& window);

    // Comprobar si se seleccionó la opción "Salir"
    bool isExitOptionSelected() const;

    // Reiniciar el estado de selección de "Salir"
    void resetExitOption();

private:
    // Métodos auxiliares
    void moveUp();
    void moveDown();
    void updateOptionColors();
    void handleSelection(bool& isPaused, bool& isMenuActive);
    void configurarTexto(sf::Text& texto, const std::string& contenido);

    // Variables de estado
    bool isActive;
    int selectedOptionIndex;
    bool exitOptionSelected;

    // SFML elementos gráficos
    sf::Font fuente;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text texto1; // Opción "Reanudar"
    sf::Text texto2; // Opción "Salir"

    // Control de entrada (enfriamiento)
    sf::Clock keyCooldownClock;
    const sf::Time keyCooldown = sf::milliseconds(200); // Tiempo de enfriamiento entre pulsaciones
};

#endif // PAUSA_H
