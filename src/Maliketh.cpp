// Maliketh.cpp
#include "Maliketh.h"
#include <iostream>

#include <cmath>

Maliketh::Maliketh()
{
    _health = 300;  // Salud espec�fica de Maliketh
    _speed = 2.0f;  // Velocidad de movimiento
    _isEnraged = false;
    _isDying = false;

}

void Maliketh::Begin()
{
    // Cargar textura y configurar el cuerpo f�sico de Maliketh
    _texture.loadFromFile("resources/textures/Maliketh.png");
    _deathtexture.loadFromFile("resources/textures/MalikethDeath.png");
    _sprite.setTexture(_texture);
    _sprite.setTextureRect({111, 0, 300, 338});
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height);
    _sprite.setScale(0.7f, 0.7f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(800 / 30.0f, 800 / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    if (!_bufferPasos.loadFromFile("1015.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoPasos.setBuffer(_bufferPasos);

    if (!_bufferGolpe.loadFromFile("1107.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoGolpe.setBuffer(_bufferGolpe);
    _sonidoGolpe.setVolume(30);

    if (!_bufferEspada.loadFromFile("1108.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoEspada.setBuffer(_bufferEspada);

    b2PolygonShape shape;
    shape.SetAsBox(1.5, 1.0f);  // Tama�o del cuerpo de Maliketh

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


void Maliketh::update(float deltaTime, Jugador& Prota)
{


    if (_isDying)
    {
        playDeathAnimation(deltaTime);  // Reproducir la animaci�n de muerte

        return;  // Evitar cualquier otra acci�n mientras est� muriendo
    }



    if (_damageClock.getElapsedTime() > sf::seconds(0.2f))
    {
        // Restaura el color original del sprite
        _sprite.setColor(sf::Color::White);
    }


    if(_health <= 150 && !_isEnraged)
    {
        _speed = 4.0f;
        _frameSpeed = 18;
        _isEnraged = true;
    }


    if(_health <= 0)
    {
        _isDying = true;  // Cambiar a estado de muerte
        _frameTime = 0;  // Reiniciar la animaci�n de muerte
        _sprite.setTexture(_deathtexture);  // Cambiar la textura al sprite sheet de muerte
        return;

    }


    sf::Vector2f BossPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);

    sf::Vector2f direction = Prota.getPosition() - BossPosition;

    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (_isEnraged && distance > 400.0f && _teleportCooldownClock.getElapsedTime() >= _teleportCooldownTime )
    {
        teleportNearPlayer(Prota);
        _teleportCooldownClock.restart();  // Reinicia el cooldown despu�s del teletransporte
        std::cout << "�Maliketh se teletransporta cerca del jugador!" << std::endl;
    }

    // Si est� atacando, ejecuta la animaci�n de ataque

    if (_isAttacking)
    {
        _attackFrame += _frameSpeed * deltaTime;

        _sonidoEspada.play();

        _sprite.setTextureRect({2275 + (int)_attackFrame * 375, 0, 300, 338});

        // Si la animaci�n ha terminado (por ejemplo, tras 15 cuadros)
        if (_attackFrame > 14)
        {
            _isAttacking = false;  // Finalizamos el ataque

            // Verificamos si el jugador est� en rango para aplicar da�o
            if (distance <= _attackRange)
            {
                Prota.takeDamage(10);
                std::cout << "�Maliketh ataca! Da�o infligido: 10" << std::endl;
            }
        }

        // Detenemos el movimiento mientras ataca
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else
    {
        // L�gica de movimiento normal si no est� atacando
        if (distance < 1000.0f)    // Rango de detecci�n
        {
            if (distance > 100.0f)
            {
                direction /= distance;
                _frameTime += 4 * deltaTime;

                // Animaci�n de movimiento del enemigo.
                if (_frameTime > 6)
                {
                    _frameTime = 0;
                }

                _sprite.setTextureRect({100 + (int)_frameTime * 375, 0, 300, 338});

                b2Vec2 velocity(direction.x * _speed, 0);
                body->SetLinearVelocity(velocity);

                if (_sonidoPasos.getStatus() != sf::Sound::Playing)
                {
                    _sonidoPasos.play();
                }

                // Ajusta la escala en funci�n de la direcci�n en X
                if (direction.x < 0)
                {
                    _sprite.setScale(-0.7f, 0.7f);  // Mirando hacia la izquierda
                }
                else
                {
                    _sprite.setScale(0.7f, 0.7f);   // Mirando hacia la derecha
                }

            }
            else
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            }
        }
        else
        {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
    }

    // Sincroniza la posici�n del cuerpo f�sico con el sprite
    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);
}


void Maliketh::attack(Jugador& Prota)
{
    if (_isAttacking || _attackCooldownClock.getElapsedTime() < _attackCooldown) return;

    _isAttacking = true;  // Iniciamos el ataque
    _attackFrame = 0;  // Reiniciamos la animaci�n de ataque
    _attackCooldownClock.restart();  // Reiniciamos el cooldown
}

void Maliketh::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);


}


void Maliketh::teleportNearPlayer(Jugador& Prota)
{


    sf::Vector2f targetPosition = Prota.getPosition();

    // Obtener la posici�n original de Maliketh en el eje Y
    float originalY = body->GetPosition().y * 30.0f;

    // Calcula una nueva posici�n cerca del jugador en el eje X con una variaci�n aleatoria
    float offsetX = ((rand() % 3) - 1) * 100;  // Desplazamiento entre -100, 0 o +100 en X
    body->SetTransform(b2Vec2((targetPosition.x + offsetX) / 30.0f, originalY / 30.0f), body->GetAngle());


}


void Maliketh::die()
{
    if (body)
    {
        Physics::world.DestroyBody(body);
        _isAlive = false;
        body = nullptr;
    }

    // Podr�as tambi�n cambiar la textura o animaci�n para indicar la muerte

}


void Maliketh::takeDamage(int amount)
{
    _health -= amount;

    if (!_isDying)
    {
        _sprite.setColor(sf::Color::Red);
        _damageClock.restart();
    }

    if (_health <= 0 && !_isDying)
    {
        _isDying = true;
        _frameTime = 0;
        _sprite.setTexture(_deathtexture);
        _sprite.setColor(sf::Color::White);
        std::cout << "�Maliketh ha sido derrotado!" << std::endl;
    }
    else
    {
        std::cout << "�Maliketh recibi� " << amount << " puntos de da�o! Salud restante: " << _health << std::endl;
        if (_sonidoGolpe.getStatus() != sf::Sound::Playing)
        {
            _sonidoGolpe.play();
        }
    }
}


void Maliketh::playDeathAnimation(float deltaTime)
{
    // Avanzar en la animaci�n de muerte
    _frameTime += 4 * deltaTime;
    _sprite.setTextureRect({103 + (int)_frameTime*377, 0, 178, 338});

    // Verificar si la animaci�n ha terminado (ej., 10 frames)
    if (_frameTime > 27)
    {
        die();  // Llamar a die() cuando termine la animaci�n de muerte
    }
}
