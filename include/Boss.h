// Boss.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "Jugador.h"
#include <iostream>

class Boss : public sf::Drawable {
protected:
    b2Body* body;
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::Texture _deathtexture;
    int _health;
    float _speed;
    bool _isAttacking;
    bool _isDying;
    sf::Clock _attackCooldown;
    const sf::Time _cooldownTime = sf::seconds(1.5f);


public:
    Boss(){}
    virtual void update(float deltaTime , Jugador& Prota) = 0;  // Método virtual puro para actualizar al jefe
    virtual void attack(Jugador& Prota) = 0;  // Método virtual puro para el ataque del jefe

    sf::Vector2f getPosition() const {
        return _sprite.getPosition();
    }

    virtual void Begin() = 0;  // Método virtual puro para inicializar el jefe
     virtual void takeDamage(int amount) =0;


     int getHealth() const { return _health; }
};
