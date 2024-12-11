#include <SFML/Window.hpp>
#include "Radahn.h"
#include <iostream>
#include <cmath>


Radahn::Radahn() {
    _health = 300.0f;  // Salud específica de Radahn
    _speed = 2.0f;  // Velocidad de movimiento
    _isEnraged = false;
    _isDying = false;
    _hasShield = false;
}

void Radahn::Begin() {
    // Cargar textura y configurar el cuerpo físico de Maliketh
    _texture.loadFromFile("resources/textures/Radahn.png");

    _sprite.setTexture(_texture);
    _sprite.setTextureRect({17 , 113, 38, 67});
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height);
    _sprite.setScale(5.7f , 5.7f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(200 / 30.0f, 800 / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(1.0f, 1.0f);  // Tamaño del cuerpo de Maliketh

    if(!_bufferPasos.loadFromFile("1109.ogg")){
        std::cout<<"Error al cargar el sonido."<<std::endl;
    }
    _sonidoPasos.setBuffer(_bufferPasos);

    if(!_bufferEspada.loadFromFile("1117.ogg")){
        std::cout<<"Error al cargar el sonido."<<std::endl;
    }
    _sonidoEspada.setBuffer(_bufferEspada);


    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    UserData* bossUserData = new UserData();
    bossUserData->type = UserDataType::BOSS;
    bossUserData->data = this;  // Asignar el puntero de Maliketh al campo data
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(bossUserData);  // Asignar el UserData al fixture
    body->CreateFixture(&fixtureDef);
}

void Radahn::update(float deltaTime, Jugador& Prota) {
    if (_isDying) {
        playDeathAnimation(deltaTime); // Reproducir la animación de muerte
        return;  // Evitar cualquier otra acción mientras está muriendo
    }

    if (_damageClock.getElapsedTime() > sf::seconds(0.2f)) {
        // Restaura el color original del sprite
        _sprite.setColor(sf::Color::White);
    }

    if (_health <= 300 && !_isEnraged) {
        _speed = 4.0f;
        _frameSpeed = 18;
        _isEnraged = true;
        _hasShield = true;  // Activa el escudo al entrar en modo furia
        _shieldClock.restart();  // Reinicia el temporizador del escudo
        std::cout << "¡Radahn está enfurecido y ha activado un escudo temporal!" << std::endl;
    }

    if (_hasShield && _shieldClock.getElapsedTime() > _shieldDuration) {
        _hasShield = false;
        std::cout << "¡El escudo de Radahn ha desaparecido!" << std::endl;
    }

    if (_health <= 0) {
        _isDying = true;  // Cambiar a estado de muerte
        _frameTime = 0;  // Reiniciar la animación de muerte
        return;
    }

    sf::Vector2f bossPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);
    sf::Vector2f direction = Prota.getPosition() - bossPosition;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (_isEnraged && distance > 400.0f && _teleportCooldownClock.getElapsedTime() >= _teleportCooldownTime) {
        teleportNearPlayer(Prota);
        _teleportCooldownClock.restart();  // Reinicia el cooldown después del teletransporte
        std::cout << "¡Radahn se teletransporta cerca del jugador!" << std::endl;
    }

    // Si está dentro del rango de ataque, atacar continuamente
    if (distance < _attackRange) {
        attack(Prota);  // Intenta atacar siempre que esté en el rango
    }

    if (_isAttacking) {
        // Animación de ataque
        _attackFrame += _frameSpeed * deltaTime;
        _sonidoEspada.play();
        _sprite.setTextureRect({17 + (int)_attackFrame * 80, 190, 60, 67});

        // Verifica si la animación ha terminado
        if (_attackFrame > 10) {
            _isAttacking = false;  // Finaliza el ataque

            // Verificamos si el jugador está en rango para aplicar daño
            if (distance <= _attackRange) {
                Prota.takeDamage(5);
                std::cout << "¡Radahn ataca! Daño infligido: 10" << std::endl;
            }
        }

        // Detener el movimiento mientras ataca
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    } else {
        // Lógica de movimiento normal si no está atacando
        if (distance < 1000.0f) {  // Rango de detección
            if (distance > 100.0f) {
                direction /= distance;
                _frameTime += 4 * deltaTime;

                // Animación de movimiento del enemigo.
                if (_frameTime > 6) {
                    _frameTime = 0;
                }
                _sprite.setTextureRect({17 + (int)_frameTime * 80, 113, 38, 57});

                b2Vec2 velocity(direction.x * _speed, 0);
                body->SetLinearVelocity(velocity);

                if (_sonidoPasos.getStatus() != sf::Sound::Playing)
                {
                    _sonidoPasos.play();
                }

                // Ajusta la escala en función de la dirección en X
                if (direction.x < 0) {
                    _sprite.setScale(-5.7f, 5.7f);  // Mirando hacia la izquierda
                } else {
                    _sprite.setScale(5.7f, 5.7f);   // Mirando hacia la derecha
                }

            } else {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            }
        } else {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
    }

    // Sincroniza la posición del cuerpo físico con el sprite
    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 39.0f);
}

void Radahn::attack(Jugador& Prota) {
    if (_isAttacking || _attackCooldownClock.getElapsedTime() < _attackCooldown) return;

    _isAttacking = true;  // Iniciamos el ataque
    _attackFrame = 0;  // Reiniciamos la animación de ataque
    _attackCooldownClock.restart();  // Reiniciamos el cooldown
}

void Radahn::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
    if (_hasShield) {
        target.draw(_shieldCircle, states);
    }


}

void Radahn::die(){
    if (body) {
        Physics::world.DestroyBody(body);
        body = nullptr;
        _isAlive = false;
    }

    // Podrías también cambiar la textura o animación para indicar la muerte

}

void Radahn::takeDamage(int amount) {
    if (_hasShield) {
        amount /= 2;  // Reduce el daño a la mitad si el escudo está activo
        std::cout << "¡El escudo de Radahn reduce el daño recibido a " << amount << "!" << std::endl;
    }

     _sprite.setColor(sf::Color::Red);
    _damageClock.restart(); // Reinicia el reloj para restaurar el color más ad

    _health -= amount;
    if (_health <= 0 && !_isDying) {
        _isDying = true;
        _frameTime = 0;



        _sprite.setColor(sf::Color::White);
        std::cout << "¡Rahan ha sido derrotado!" << std::endl;
    } else {
        std::cout << "¡Rahan recibió " << amount << " puntos de daño! Salud restante: " << _health << std::endl;
    }


}

void Radahn::playDeathAnimation(float deltaTime) {

    _frameTime += 4 * deltaTime;
    _sprite.setTextureRect({20 + (int)_frameTime * 80, 348, 38, 67});

    // Verificar si la animación ha terminado (ej., 10 frames)
    if (_frameTime > 6) {
        die();  // Llamar a die() cuando termine la animación de muerte
        _isDeadAnimationFinished = true;  // Marca que la animación ha terminado
    }
}

void Radahn::teleportNearPlayer(Jugador& Prota) {
    sf::Vector2f targetPosition = Prota.getPosition();

    // Obtener la posición original de Maliketh en el eje Y
    float originalY = body->GetPosition().y * 30.0f;

    // Calcula una nueva posición cerca del jugador en el eje X con una variación aleatoria
    float offsetX = ((rand() % 3) - 1) * 100;  // Desplazamiento entre -100, 0 o +100 en X
    body->SetTransform(b2Vec2((targetPosition.x + offsetX) / 30.0f, originalY / 30.0f), body->GetAngle());


}
