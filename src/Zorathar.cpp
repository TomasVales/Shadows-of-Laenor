
#include "Zorathar.h"
#include <iostream>

#include <cmath>

Zorathar::Zorathar()
{
    _health = 300;  // Salud específica de Maliketh
    _speed = 2.0f;  // Velocidad de movimiento
    _isEnraged = false;
    _isDying = false;
}

void Zorathar::Begin()
{
    // Cargar textura y configurar el cuerpo físico de Maliketh
    _texture.loadFromFile("resources/textures/Zorathor.png");
    _deathtexture.loadFromFile("resources/textures/ZoratharDeath.png");
    _sprite.setTexture(_texture);
    _sprite.setTextureRect({39, 0, 379, 380});
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height);
    _sprite.setScale(0.7f, 0.7f);

    if (!_bufferArrastrar.loadFromFile("1114.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoArrastrar.setBuffer(_bufferArrastrar);

    if (!_bufferEspada.loadFromFile("1115.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoEspada.setBuffer(_bufferEspada);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(800 / 30.0f, 800 / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(2.5f, 2.0f);  // Tamaño del cuerpo del jefe

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0001; // Categoria para Zorathor
    fixtureDef.filter.maskBits = 0x0001; // Puede colisionar con los esqueletos y el jugador

    UserData* bossUserData = new UserData();
    bossUserData->type = UserDataType::BOSS;
    bossUserData->data = this;  // Asignar el puntero de Maliketh al campo data
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(bossUserData);  // Asignar el UserData al fixture
    body->CreateFixture(&fixtureDef);
}

void Zorathar::update(float deltaTime, Jugador& Prota)
{
    if (_isDying)
    {
        playDeathAnimation(deltaTime);// Reproducir la animación de muerte
         _spawnedSkeletons.clear();
        return;  // Evitar cualquier otra acción mientras está muriendo
    }

    if (_health <= 150 && !_isDying)    // Invoca esqueletos si la salud es baja, por ejemplo
    {
        spawnSkeleton();
    }

    if (_damageClock.getElapsedTime() > sf::seconds(0.2f))
    {
        _sprite.setColor(sf::Color::White);  // Restaura el color original del sprite
    }

    for (auto& skeleton : _spawnedSkeletons)
    {
        if (skeleton->isAlive())
        {
            skeleton->update(deltaTime, Prota);
        }
    }

    if (_damageClock.getElapsedTime() > sf::seconds(0.2f))
    {
        // Restaura el color original del sprite
        _sprite.setColor(sf::Color::White);
    }
    damageBonus= Prota.getDamageBonus();

    if(_health <= 150 && !_isEnraged)
    {
        _speed = 4.0f;
        _frameSpeed = 18;
        _isEnraged = true;
    }
    if(_health <= 0)
    {
        _isDying = true;  // Cambiar a estado de muerte
        _frameTime = 0;  // Reiniciar la animación de muerte
        _sprite.setTexture(_deathtexture);  // Cambiar la textura al sprite sheet de muerte

        return;

    }

    sf::Vector2f malikethPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);  // Definir la posición
    sf::Vector2f direction = Prota.getPosition() - malikethPosition;  // Calcular la dirección
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);



    // Si está atacando, ejecuta la animación de ataque
    if (_isAttacking)
    {
        _attackFrame += _frameSpeed * deltaTime;

        _sonidoEspada.play();


        _sprite.setTextureRect({2917 + (int)_attackFrame * 479, 0, 380, 380});

        // Si la animación ha terminado (por ejemplo, tras 15 cuadros)
        if (_attackFrame > 10)
        {
            _isAttacking = false;  // Finalizamos el ataque

            // Verificamos si el jugador está en rango para aplicar daño
            if (distance <= _attackRange)
            {
                Prota.takeDamage(10);
                std::cout << "¡Maliketh ataca! Daño infligido: 10" << std::endl;
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
                if (_frameTime > 6)
                {
                    _frameTime = 0;
                }
                _sprite.setTextureRect({39 + (int)_frameTime * 479, 0, 379, 380});

                b2Vec2 velocity(direction.x * _speed, 0);
                body->SetLinearVelocity(velocity);

                if (_sonidoArrastrar.getStatus() != sf::Sound::Playing)
                {
                    _sonidoArrastrar.play();
                }

                // Ajusta la escala en función de la dirección en X
                if (direction.x < 0)
                {
                    _sprite.setScale(0.7f, 0.7f);  // Mirando hacia la izquierda
                }
                else
                {
                    _sprite.setScale(-0.7f, 0.7f);   // Mirando hacia la derecha
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

    // Sincroniza la posición del cuerpo físico con el sprite
    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 31.0f);
}


void Zorathar::attack(Jugador& Prota)
{
    if (_isAttacking || _attackCooldownClock.getElapsedTime() < _attackCooldown) return;

    _isAttacking = true;  // Iniciamos el ataque
    _attackFrame = 0;  // Reiniciamos la animación de ataque
    _attackCooldownClock.restart();  // Reiniciamos el cooldown
}

void Zorathar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
    for (auto& skeleton : _spawnedSkeletons)
    {
        if (skeleton->isAlive())
        {
            skeleton->draw(target, states);
        }
    }


}

void Zorathar::die()
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


void Zorathar::takeDamage(int amount)
{
    _health -= amount + damageBonus;

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


void Zorathar::playDeathAnimation(float deltaTime)
{
    // Avanzar en la animación de muerte
    _frameTime += 4 * deltaTime;
    _sprite.setTextureRect({38 + (int)_frameTime*480, 0, 420, 380});
    _sprite.setScale(0.7, 0.7);

    // Verificar si la animación ha terminado (ej., 10 frames)
    if (_frameTime > 21)
    {
        die();  // Llamar a die() cuando termine la animación de muerte
    }
    else
    {

        _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y *30 );
    }

}


void Zorathar::spawnSkeleton()
{
    // Verifica si ha pasado el tiempo de cooldown para invocar esqueletos
    if (_spawnCooldownClock.getElapsedTime() >= _spawnCooldown)
    {
        // Crear dos nuevos esqueletos
        for (int i = 0; i < 2; ++i)
        {
            Skeleton* newSkeleton = new Skeleton();  // Crea un nuevo esqueleto

            newSkeleton->Begin();  // Inicializa el esqueleto

            // Posiciona los esqueletos cerca de Zorathor
            float spawnOffsetX = (i == 0) ? 50.0f : -50.0f;  // El primer esqueleto se desplaza a la derecha y el segundo a la izquierda
            float spawnPosX = body->GetPosition().x * 30.0f + spawnOffsetX;
            float spawnPosY = body->GetPosition().y * 30.0f;

            newSkeleton->setPosition(spawnPosX, spawnPosY);

            _spawnedSkeletons.push_back(newSkeleton);  // Agrega el esqueleto a la lista
        }

        _spawnCooldownClock.restart();  // Reinicia el reloj de cooldown
    }
}




