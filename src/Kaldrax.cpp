
#include "Kaldrax.h"
#include <iostream>

#include <cmath>

Kaldrax::Kaldrax()
{
    _health = 300;  // Salud específica de Maliketh
    _speed = 2.0f;  // Velocidad de movimiento
    _isEnraged = false;
    _hasShield = false;
    _shieldDuration = sf::seconds(5);  // Duración del escudo
    _isDying = false;
}

void Kaldrax::Begin()
{
    // Cargar textura y configurar el cuerpo físico de Maliketh
    _texture.loadFromFile("resources/textures/Kaldrax.png");
    _deathtexture.loadFromFile("resources/textures/KaldraxDeath.png");
    _sprite.setTexture(_texture);
    _sprite.setTextureRect({90, 0, 155, 266});
    _sprite.setScale(0.7, 0.7);

    _sprite.setOrigin(_sprite.getGlobalBounds().width / 2, _sprite.getGlobalBounds().height / 2);

    _shieldCircle.setRadius(90.0f);  // Ajusta el tamaño para que envuelva al sprite
    _shieldCircle.setFillColor(sf::Color::Transparent);  // Sin relleno
    _shieldCircle.setOutlineThickness(5.0f);
    _shieldCircle.setOutlineColor(sf::Color(0, 255, 255, 150));  // Color cian semitransparente
    _shieldCircle.setOrigin(_shieldCircle.getRadius(), _shieldCircle.getRadius()); // Ajuste al centro del sprite

    if (!_bufferPasos.loadFromFile("1116.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoPasos.setBuffer(_bufferPasos);

    if (!_bufferEspada.loadFromFile("1111.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoEspada.setBuffer(_bufferEspada);

    if (!_bufferEscudo.loadFromFile("1110.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoEscudo.setBuffer(_bufferEscudo);


    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(800 / 30.0f, 800 / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(1.5, 1.0f);  // Tamaño del cuerpo del jefe

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

void Kaldrax::update(float deltaTime, Jugador& Prota)
{
    if (_isDying)
    {
        playDeathAnimation(deltaTime);  // Reproducir la animación de muerte

        return;  // Evitar cualquier otra acción mientras está muriendo
    }

    if (_damageClock.getElapsedTime() > sf::seconds(0.2f))
    {
        _sprite.setColor(sf::Color::White);  // Restaura el color original del sprite
    }


    if (_health <= 150 && !_isEnraged)
    {
        _speed = 4.0f;
        _frameSpeed = 18;
        _isEnraged = true;
        _hasShield = true;  // Activa el escudo al entrar en modo furia
        _shieldClock.restart();  // Reinicia el temporizador del escudo

        if (!_hasShieldSoundPlayed)
        {
            _sonidoEscudo.play();
            _hasShieldSoundPlayed = true;  // Marcar que el sonido ya se ha reproducido
        }
    }
    if (_hasShield && _shieldClock.getElapsedTime() > _shieldDuration)
    {

        _hasShield = false;

    }
    if(_health <= 0)
    {
        _isDying = true;  // Cambiar a estado de muerte
        _frameTime = 0;  // Reiniciar la animación de muerte
        _sprite.setTexture(_deathtexture);  // Cambiar la textura al sprite sheet de muerte
        return;

    }

    sf::Vector2f bossPosition(body->GetPosition().x * 30.0f, body->GetPosition().y *30.0f);
    sf::Vector2f direction = Prota.getPosition() - bossPosition;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);



    // Si está atacando, ejecutamos la animación de ataque
    if (_isAttacking)
    {
        _attackFrame += _frameSpeed * deltaTime;

        _sonidoEspada.play();

        // Mostrar la animación de ataque usando los cuadros adecuados
        _sprite.setTextureRect({3187 + (int)_attackFrame * 335, 0, 220, 266});

        // Si la animación ha terminado (por ejemplo, tras 15 cuadros)
        if (_attackFrame > 6)
        {
            _isAttacking = false;  // Finalizamos el ataque

            // Verificamos si el jugador está en rango para aplicar daño
            if (distance <= _attackRange)
            {
                Prota.takeDamage(10);  // Aplicamos el daño al jugador

            }
            if (_isEnraged)
            {
                int healthRecovered = 10;  // Vida recuperada al hacer daño
                _health = std::min(_health + healthRecovered, 300);  // Limita la salud máxima a 300

            }
        }

        // Detenemos el movimiento mientras ataca
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else
    {
        // Lógica de movimiento normal si no está atacando
        if (distance < 1000.0f)    // Rango de detección
        {
            if (distance > 100.0f)
            {
                direction /= distance;
                _frameTime += 4 * deltaTime;

                // Animación de movimiento del enemigo.
                if (_frameTime > 9)
                {
                    _frameTime = 0;
                }
                _sprite.setTextureRect({90 + (int)_frameTime * 347, 0, 155, 266});


                b2Vec2 velocity(direction.x * _speed, 0);
                body->SetLinearVelocity(velocity);

                if (_sonidoPasos.getStatus() != sf::Sound::Playing)
                {
                    _sonidoPasos.play();
                }

                // Ajusta la escala en función de la dirección en X

                if (direction.x < 0)
                {
                    _sprite.setScale(0.7, 0.7);  // Mirando hacia la izquierda
                }
                else
                {
                    _sprite.setScale(-0.7, 0.7);   // Mirando hacia la derecha
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

    // Sincronizar la posición del cuerpo físico con el sprite

    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y *26 );
    if (_hasShield)
    {
        _shieldCircle.setPosition(_sprite.getPosition());
    }
}


void Kaldrax::attack(Jugador& Prota)
{
    if (_isAttacking || _attackCooldownClock.getElapsedTime() < _attackCooldown) return;

    _isAttacking = true;  // Iniciamos el ataque
    _attackFrame = 0;  // Reiniciamos la animación de ataque
    _attackCooldownClock.restart();  // Reiniciamos el cooldown
}


void Kaldrax::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
    if (_hasShield)
    {
        target.draw(_shieldCircle, states);
    }


}


void Kaldrax::die()
{
    if (body)
    {
        Physics::world.DestroyBody(body);

        body = nullptr;
        _isAlive = false;
    }

    // Podrías también cambiar la textura o animación para indicar la muerte
    _sprite.setColor(sf::Color::Transparent); // Hacer el sprite invisible, por ejemplo

}


void Kaldrax::takeDamage(int amount)
{
    if (_hasShield)
    {
        amount /= 2;  // Reduce el daño a la mitad si el escudo está activo
        std::cout << "¡El escudo de Kaldrax reduce el daño recibido a " << amount << "!" << std::endl;
    }

    _health -= amount;

    // Verifica si está muriendo antes de cambiar el color
    if (!_isDying)
    {
        _sprite.setColor(sf::Color::Red);  // Cambia el color del sprite a rojo
        _damageClock.restart();            // Reinicia el temporizador del daño
    }


    if (_health <= 0 && !_isDying)
    {
        _isDying = true;
        _frameTime = 0;
        _sprite.setTexture(_deathtexture);
        _sprite.setColor(sf::Color::White);

    }
}


void Kaldrax::playDeathAnimation(float deltaTime)
{
    // Avanzar en la animación de muerte
    _frameTime += 4 * deltaTime;
    _sprite.setTextureRect({0 + (int)_frameTime*590, 0, 590, 720});
    _sprite.setScale(0.2, 0.2);

    // Verificar si la animación ha terminado (ej., 10 frames)
    if (_frameTime > 18)
    {
        die();  // Llamar a die() cuando termine la animación de muerte
    }
    else
    {

        _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y *25 );
    }
}
