#pragma once
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include <box2d/b2_body.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "UserData.h"



enum class PersonajeState{
    idle = 0,
    Move,
    Attack,
    Jump
 };
class Jugador: public sf::Drawable
{



    b2Body* body;

   sf::Sprite _sprite;
   sf::Texture _texture;
   sf::Vector2f _velocity;
   sf::Vector2f _movePosition={0,0};


   sf::Clock _attackCooldown;
   const sf::Time _cooldownTime = sf::seconds(0.75f);
   float _cooldownProgress = 0.0f;
   sf::Clock _attackTimer;  // Reloj para la duraci�n del ataque.
    const sf::Time _attackDuration = sf::seconds(0.5f);  // Tiempo que dura la animaci�n de ataque.
    bool _isAttacking = false;  // Indica si est� en un ataque activo.
    int _Health = 100;
    bool _canJump = false;  // Nueva bandera para controlar el salto.
    float _damageReduction = 1.0f;  // Factor de reducci�n de da�o (1.0f = sin reducci�n)
    int _damageBonus = 0;  // Bonificaci�n de da�o

    bool armadura = false;  // Bandera para saber si el jugador tiene armadura
    bool espada = false;    // Bandera para saber si el jugador tiene espada
    bool casco = false;

    sf::Sound _sonidoPasos;
    sf::SoundBuffer _bufferPasos;
    sf::Sound _sonidoEspada;
    sf::SoundBuffer _bufferEspada;




   float _frame;
    PersonajeState _state = PersonajeState::idle;
   public:

       Jugador();
       void cmd();
       void update(float deltaTime);
       void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
       float angle{};
       void Begin();

        // M�todo para configurar el estado de salto
        void setCanJump(bool canJump) {
        _canJump = canJump;
        }
        void takeDamage(int danio)
        {
             int danioReducido = static_cast<int>(danio * _damageReduction); // Aplica la reducci�n de da�o
            _Health -= danioReducido;
            std::cout << "El jugador recibi� da�o reducido: " << danioReducido << std::endl;
        }


        sf::Vector2f getPosition() const {
            return _sprite.getPosition();
        }
        bool isAttacking() const { return _isAttacking; }

        bool haLlegadoAlFinal(float puntoFinalX) {
        return _sprite.getPosition().x >= puntoFinalX; // Ajusta seg�n tu l�gica
    }
        int getHealth() const { return _Health; }
        void setPosition(const sf::Vector2f& position);
        void RecuperarVida();
         // M�todos para aplicar efectos
    void reduceDamage(float factor) {
        _damageReduction *= factor;  // Reducir el da�o recibido
    }

    void increaseDamage(int bonus) {
        _damageBonus += bonus;  // Incrementar el da�o de ataque
    }

    void recogerArmadura() { armadura = true; }

    void recogerEspada() { espada = true; }

    void recogerCasco() { casco = true; }

    bool getArmadura()const{return armadura;}
    bool getCasco() const{return casco; }
    bool getEspada() const{return espada;}
    int getDamageBonus(){return _damageBonus;}

};
