#pragma once
#include <SFML/Graphics.hpp>

class Klosterian : public sf::Drawable
{
    sf::Sprite _sprite;        // Sprite del NPC
    sf::Texture _texture;      // Textura del sprite
    int _frame;                // Fotograma actual de la animaci�n
    float _animationTime;       // Tiempo acumulado para la animaci�n
    float _frameDuration;       // Duraci�n de cada fotograma
    float _frameHeight;         // Altura de cada fotograma

public:
    Klosterian();               // Constructor
    void update(float deltaTime); // M�todo para actualizar el NPC
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // M�todo para dibujar el NPC en pantalla
    void Begin();               // M�todo para inicializar el NPC
     sf::Vector2f getPosition() const { return _sprite.getPosition(); }

};
