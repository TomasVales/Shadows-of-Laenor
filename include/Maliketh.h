// Maliketh.h
#pragma once
#include "Boss.h"
#include "Jugador.h"
#include "UserData.h"
#include "Physics.h"
#include <SFML/Audio.hpp>
#include "Item.h"
class Maliketh : public Boss {
private:
    sf::Clock _specialAttackCooldown;
    const sf::Time _specialCooldownTime = sf::seconds(5.0f);  // Ataque especial cada 5 segundos
    bool _isAttacking = false;
    float _frameTime = 0.0f;
    float _attackRange = 200.0f;  // Rango del ataque normal.
    int _attackDamage = 20;  // Daño que inflige cada ataque.
    float _attackFrame=0;
    bool _isAlive=true;
    sf::Clock _attackCooldownClock;  // Controlar cooldown entre ataques.
    const sf::Time _attackCooldown = sf::seconds(1.5f);  // Cooldown de 1.5 segundos.
    bool _isEnraged = false;
    int _frameSpeed = 10;
    sf::Clock _teleportCooldownClock;  // Reloj para cooldown de teletransporte
    sf::Time _teleportCooldownTime = sf::seconds(3);  // Tiempo de cooldown de 3 segundos
    sf::Clock _damageClock;
    sf::Sound _sonidoPasos;
    sf::SoundBuffer _bufferPasos;
    sf::Sound _sonidoEspada;
    sf::SoundBuffer _bufferEspada;
    sf::Sound _sonidoGolpe;
    sf::SoundBuffer _bufferGolpe;



public:
    Maliketh();
    void update(float deltaTime , Jugador& Prota)override;
    void attack(Jugador& Prota) override;
    void Begin() override;  // Inicializar las características de Maliketh
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool isAttacking() const { return _isAttacking; }
    void teleportNearPlayer(Jugador& Prota);
    void die();
    bool isAlive(){return _isAlive;}
    void takeDamage(int amount);
    void playDeathAnimation(float deltaTime);


};
