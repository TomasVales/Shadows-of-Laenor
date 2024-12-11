#include <iostream>
#include "klosterian.h"


Klosterian::Klosterian() : _frame(0), _animationTime(0), _frameDuration(0.5f), _frameHeight(1200) // _frameDuration para bajar o subir velocidad del frame
{
}

//Inicializa el sprite de klosterian, texturas, escala, posicion, etc.

void Klosterian::Begin()
{

    //Carga de textura
    _frame = 0;
    if (!_texture.loadFromFile("resources/textures/klosterian.png")) {
        std::cerr << "Error al cargar la textura de Klosterian" << std::endl;
    }


    _sprite.setTexture(_texture);  // Asigna la textura al sprite
    _sprite.setPosition(200, 600);

   //Tamaño
    float scaleX = 0.08f;
    float scaleY = 0.08f;
    _sprite.setScale(scaleX, scaleY);


    int frameWidth = 9600 / 8;  //
    _sprite.setTextureRect(sf::IntRect(_frame * frameWidth, 0, frameWidth, _frameHeight));

}

//Actualiza la animacion de klosterian mostrando el siguiente frame.

void Klosterian::update(float deltaTime)
{
    _animationTime += deltaTime;
    if (_animationTime >= _frameDuration)
    {
        _animationTime = 0;
        _frame = (_frame + 1) % 8;
        int frameWidth = 9600 / 8;
        _sprite.setTextureRect(sf::IntRect(_frame * frameWidth, 0, frameWidth, _frameHeight));
    }
}

//Renderiza el sprite

void Klosterian::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}
