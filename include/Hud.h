// Hud.h
#pragma once

#include <SFML/Graphics.hpp>
#include "HealthBar.h"
#include "Jugador.h"



class Hud {
public:
    Hud(); // Constructor para inicializar los elementos del HUD
    void update(float deltaTime, sf::View& view , Jugador& player); // Actualiza los elementos del HUD
    void render(sf::RenderWindow& window, sf::View& view); // Dibuja los elementos del HUD en la pantalla



private:
    HealthBar healthBarJugador;
    sf::Sprite _spriteArmadura;
    sf::Sprite _spriteEspada;
    sf::Sprite _spriteCasco;
    sf::Texture texturaArmadura;
     sf::Texture texturaEspada;
     sf::Texture texturaCasco;
    // Agrega otros elementos del HUD según sea necesario
    bool armadura = false;
    bool casco = false;
    bool espada = false;
};

