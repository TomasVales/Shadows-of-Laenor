#pragma once
#include <SFML/Graphics.hpp>

class Klosterian : public sf::Drawable
{
    sf::Sprite _sprite;        // Sprite del NPC
    sf::Texture _texture;      // Textura del sprite
    int _frame;                // Fotograma actual de la animación
    float _animationTime;       // Tiempo acumulado para la animación
    float _frameDuration;       // Duración de cada fotograma
    float _frameHeight;         // Altura de cada fotograma

public:
    Klosterian();               // Constructor
    void update(float deltaTime); // Método para actualizar el NPC
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Método para dibujar el NPC en pantalla
    void Begin();               // Método para inicializar el NPC
     sf::Vector2f getPosition() const { return _sprite.getPosition(); }

};
