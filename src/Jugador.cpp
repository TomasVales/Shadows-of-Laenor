#include "Jugador.h"
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846

const float jumpvelocity = 10.0f;

//Carga de archivos, define vida y estados.

Jugador::Jugador()
    : _frame(0), _isAttacking(false), _canJump(false), _Health(100)
{
    // Configuración del sprite
    _texture.loadFromFile("resources/textures/protagonista.png"); //Carga imagen
    _sprite.setTexture(_texture); // Asigna textura
    _sprite.setTextureRect({770, 380, 100, 202}); //Subregion de la textura (ubicacion)
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height); // Origen del sprite
    _sprite.setScale(0.5f, 0.5f); // Escala del sprite.

    //Sonido Espada
    if (!_bufferEspada.loadFromFile("1010.ogg"))
    {
        std::cout << "Error al cargar el sonido de espada." << std::endl;
    }

    _sonidoEspada.setBuffer(_bufferEspada);
    _sonidoEspada.setVolume(70);

    //Sonido Pasos
    if (!_bufferPasos.loadFromFile("1105.ogg"))
    {
        std::cout << "Error al cargar el sonido de pasos." << std::endl;
    }

    _sonidoPasos.setBuffer(_bufferPasos);
}

//Inicializa el cuerpo, forma, fixture y asignacion del userdata del personaje.

void Jugador::Begin()
{
    // Definición del cuerpo Box2D
    b2BodyDef bodyDef{};
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(200 / 30.0f, 200 / 30.0f); // Conversión de píxeles a metros
    body = Physics::world.CreateBody(&bodyDef);

    // Definición de la forma
    b2PolygonShape shape{};
    shape.SetAsBox(1.0f, 1.0f); // Tamaño del jugador

    // Definición de la fixture
    b2FixtureDef fixtureDef{};
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Asignación de UserData
    UserData* playerData = new UserData{UserDataType::PLAYER, this};
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(playerData);

    body->CreateFixture(&fixtureDef);
}

// Controla el movimiento, ataque y estados del jugador.

void Jugador::cmd()
{
    if (_isAttacking)
    {
        return; // No hacer nada si está atacando
    }

    _movePosition = {0, 0};
    angle = body->GetAngle() * (180.0f / M_PI);

    // Manejo de ataque

    if (!_isAttacking && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) &&
        _attackCooldown.getElapsedTime() >= _cooldownTime)
    {
        _state = PersonajeState::Attack;
        _attackCooldown.restart();
        _isAttacking = true;
        _sonidoEspada.play();
        return;
    }

    // Movimiento horizontal

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        _movePosition.x = -1;
        _state = PersonajeState::Move;
        if (_sonidoPasos.getStatus() != sf::Sound::Playing)
        {
            _sonidoPasos.play();
        }
    }

    // Movimiendo horizontal

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        _movePosition.x = 1;
        _state = PersonajeState::Move;
        if (_sonidoPasos.getStatus() != sf::Sound::Playing)
        {
            _sonidoPasos.play();
        }
    }

    // Salto
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && _canJump)
    {
        b2Vec2 velocity = body->GetLinearVelocity();
        velocity.y = -jumpvelocity;
        body->SetLinearVelocity(velocity);
        _state = PersonajeState::Move;
    }
    else
    {
        _state = PersonajeState::idle;
    }

    //Velocidad

    float velocidad = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 5.0f : 2.0f;

    if (_movePosition.x != 0)
    {
        b2Vec2 velocity = body->GetLinearVelocity();
        velocity.x = _movePosition.x * velocidad;
        body->SetLinearVelocity(velocity);
    }

    // Prevenir que el sprite se salga de los límites

    if (_sprite.getPosition().x < 0)
    {
        _sprite.setPosition(0, _sprite.getPosition().y);
    }
}

//Actualiza el estado del jugador por cada frame.

void Jugador::update(float deltaTime)
{
    // Actualizar cooldown
    _cooldownProgress = std::min(1.0f, _attackCooldown.getElapsedTime().asSeconds() / _cooldownTime.asSeconds());

    // Sincroniza la posición del sprite con la del cuerpo
    _sprite.setPosition(body->GetPosition().x * 30.0f, body->GetPosition().y * 30.0f);

    // Animaciones según el estado

    switch (_state)
    {
    case PersonajeState::idle:
        _frame += 1 * deltaTime;
        if (_frame > 3) _frame = 0;
        _sprite.setTextureRect({340 + static_cast<int>(_frame) * 150, 0, 150, 202});
        break;

    case PersonajeState::Move:
        _frame += 5 * deltaTime;
        if (_frame > 4) _frame = 0;
        _sprite.setTextureRect({770 + static_cast<int>(_frame) * 150, 380, 150, 202});

        // Ajustar dirección del sprite
        _sprite.setScale((_movePosition.x < 0) ? -0.5f : 0.5f, 0.5f);
        break;

    case PersonajeState::Attack:
        _frame += 6 * deltaTime;
        if (_frame > 5)
        {
            _frame = 0;
            _isAttacking = false;
        }
        _sprite.setTextureRect({470 + static_cast<int>(_frame) * 150, 1510, 120, 187});
        break;
    }
}

//Renderizado del sprite y la barra de cooldown.

void Jugador::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    // Cambiar a la vista predeterminada para la barra de cooldown
    sf::View originalView = target.getView();
    target.setView(target.getDefaultView());

    // Dibujar barra de cooldown
    sf::RectangleShape cooldownBar(sf::Vector2f(100 * _cooldownProgress, 5));
    cooldownBar.setFillColor(sf::Color::Green);
    cooldownBar.setPosition(108, 90);
    target.draw(cooldownBar, states);

    // Restaurar la vista original
    target.setView(originalView);
}

//Establece la posicion inicial o actualiza la posicion del jugador nivel fisico y grafico.

void Jugador::setPosition(const sf::Vector2f& position)
{
    _sprite.setPosition(position);
    if (body)
    {
        body->SetTransform(b2Vec2(position.x / 30.0f, position.y / 30.0f), body->GetAngle());
    }
}

//Funcion para recuperar la vida del jugador al pasar de nivel/muera.

void Jugador::RecuperarVida()
{
    if (_Health < 100)
    {
        _Health = 100;
    }
}
