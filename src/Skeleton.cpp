
#include "Skeleton.h"
#include <iostream>

#include <cmath>

Skeleton::Skeleton() {
    _health = 20;
    _speed = 6.0f;  // Velocidad de movimiento
    _isEnraged = false;
    _isDying = false;
}

void Skeleton::Begin() {

    _texture.loadFromFile("resources/textures/Skeleton.png");
    _deathtexture.loadFromFile("resources/textures/SkeletonDead.png");
    _sprite.setTexture(_texture);
    _sprite.setTextureRect({36, 0, 84, 140});
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height);
    _sprite.setScale(0.7f , 0.7f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(200 / 30.0f, 800 / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0002; // Categoria para los esqueletos
    fixtureDef.filter.maskBits = 0x0001 ;// Puede colisionar con Zorathor y el jugador
    UserData* SkeletonUserData = new UserData();
    SkeletonUserData->type = UserDataType::SKELETON;
    SkeletonUserData->data = this;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(SkeletonUserData);  // Asignar el UserData al fixture
    body->CreateFixture(&fixtureDef);
}


void Skeleton::update(float deltaTime, Jugador& Prota) {
    if (_isDying) {
        playDeathAnimation(deltaTime);  // Reproducir la animación de muerte
        return;  // Evitar cualquier otra acción mientras está muriendo
    }



    if (_damageClock.getElapsedTime() > sf::seconds(0.2f)) {
        // Restaura el color original del sprite
        _sprite.setColor(sf::Color::White);
    }



    if(_health <= 0)
    {
        _isDying = true;  // Cambiar a estado de muerte
        _frameTime = 0;  // Reiniciar la animación de muerte
        _sprite.setTexture(_deathtexture);  // Cambiar la textura al sprite sheet de muerte


        return;

    }

    sf::Vector2f SkeletonPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);
    sf::Vector2f direction = Prota.getPosition() - SkeletonPosition;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);



    // Si está atacando, ejecuta la animación de ataque
    if (_isAttacking) {
        _attackFrame += _frameSpeed * deltaTime;


        _sprite.setTextureRect({2110 + (int)_attackFrame * 163, 0, 84, 140});

        // Si la animación ha terminado (por ejemplo, tras 15 cuadros)
        if (_attackFrame > 10) {
            _isAttacking = false;  // Finalizamos el ataque

            // Verificamos si el jugador está en rango para aplicar daño
            if (distance <= _attackRange) {
                Prota.takeDamage(5);
                std::cout << "¡Enemigo ataca! Daño infligido: 5" << std::endl;
            }
        }

        // Detenemos el movimiento mientras ataca
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else {
        // Lógica de movimiento normal si no está atacando
        if (distance < 1000.0f) {  // Rango de detección
            if (distance > 100.0f) {
                direction /= distance;
                _frameTime += 4 * deltaTime;

                // Animación de movimiento del enemigo.
                if (_frameTime > 6) {
                    _frameTime = 0;
                }
                _sprite.setTextureRect({36 + (int)_frameTime * 164, 0, 84, 140});

                b2Vec2 velocity(direction.x * _speed, 0);
                body->SetLinearVelocity(velocity);

                // Ajusta la escala en función de la dirección en X
                if (direction.x < 0) {
                    _sprite.setScale(-0.7f, 0.7f);  // Mirando hacia la izquierda
                } else {
                    _sprite.setScale(0.7f, 0.7f);   // Mirando hacia la derecha
                }

            } else {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            }
        } else {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
    }

    // Sincroniza la posición del cuerpo físico con el sprite
    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);
}


void Skeleton::attack(Jugador& Prota) {
    if (_isAttacking || _attackCooldownClock.getElapsedTime() < _attackCooldown) return;

    _isAttacking = true;  // Iniciamos el ataque
    _attackFrame = 0;  // Reiniciamos la animación de ataque
    _attackCooldownClock.restart();  // Reiniciamos el cooldown
}

void Skeleton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);


}

void Skeleton::die()
{
    if (body) {
        Physics::world.DestroyBody(body);
        body = nullptr;
        _isAlive = false;
    }

}
void Skeleton::takeDamage(int amount) {
    _health -= amount;
    if (_health <= 0 && !_isDying) {
        _isDying = true;
        _frameTime = 0;
        _sprite.setTexture(_deathtexture);
         _sprite.setColor(sf::Color::White);
        std::cout << "¡Kaldrax ha sido derrotado!" << std::endl;
    }

}

void Skeleton::playDeathAnimation(float deltaTime) {
   _frameTime += 4 * deltaTime;
    _sprite.setTextureRect({39 + (int)_frameTime*32, 0 , 34 , 31});
    _sprite.setScale(1.5 , 1.5);

    // Verificar si la animación ha terminado (ej., 10 frames)
    if (_frameTime > 18) {
        die();  // Llamar a die() cuando termine la animación de muerte
    }else{

     _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y *37 );
    }

}


void Skeleton::setPosition(float x, float y) {
    if (body) {
        body->SetTransform(b2Vec2(x / 30.0f, y / 30.0f), body->GetAngle());
    }
    _sprite.setPosition(x, y);
}

