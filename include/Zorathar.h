// Maliketh.h
#pragma once
#include "Boss.h"
#include "Jugador.h"
#include "UserData.h"
#include "Physics.h"
#include "Skeleton.h"
#include <vector>
#include <SFML/Audio.hpp>
class Zorathar : public Boss {
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
    sf::Clock _spawnCooldownClock;
    const sf::Time _spawnCooldown = sf::seconds(10.0f);  // Tiempo de cooldown entre invocaciones
    std::vector<Skeleton*> _spawnedSkeletons;  // Lista de esqueletos invocados
    sf::Clock _damageClock;
    sf::Sound _sonidoEspada;
    sf::SoundBuffer _bufferEspada;
    sf::Sound _sonidoArrastrar;
    sf::SoundBuffer _bufferArrastrar;

    int damageBonus;

public:
    Zorathar();
    void update(float deltaTime , Jugador& Prota)override;
    void attack(Jugador& Prota) override;
    void Begin() override;  // Inicializar las características de Maliketh
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool isAttacking() const { return _isAttacking; }
    void spawnSkeleton();  // Método para invocar esqueletos
    void die();
    bool isAlive(){return _isAlive;}
    void takeDamage(int amount);
    void playDeathAnimation(float deltaTime);


};

