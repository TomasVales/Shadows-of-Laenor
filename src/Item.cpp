// Item.cpp
#include "Item.h"
#include <iostream>
#include "Physics.h"

Item::Item(ItemType type, const sf::Vector2f& position)
    : _type(type) {
    // Cargar textura y asignar sprite basado en el tipo de ítem
    switch (_type) {
        case ItemType::ARMOR:
            _texture.loadFromFile("resources/textures/armor.png");
            break;
        case ItemType::SWORD:
            _texture.loadFromFile("resources/textures/sword.png");
            break;
        case ItemType::HELMET:
            _texture.loadFromFile("resources/textures/helmet.png");
            break;
    }
    _sprite.setTexture(_texture);
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height / 2);
    _sprite.setPosition(position.x , position.y  - 30);
    _sprite.setScale(1.5 , 1.5);

    // Crear el cuerpo físico como un sensor para detectar colisiones sin efectos físicos
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position.x / 30.0f, position.y / 30.0f);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(_sprite.getLocalBounds().width / 60.0f, _sprite.getLocalBounds().height / 60.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;  // Actúa como sensor para detectar colisiones
    UserData* itemData = new UserData{UserDataType::ITEM, this};
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(itemData);
    body->CreateFixture(&fixtureDef);
    Recogido = false;
}

void Item::applyEffect(Jugador& player) {
    // Aplica el efecto basado en el tipo de ítem

    switch (_type) {
        case ItemType::ARMOR:
            player.recogerArmadura(); //HUD
            player.reduceDamage(0.8f);  // Reduce el daño en un 20%


            break;
        case ItemType::SWORD:
            player.recogerEspada();
            player.increaseDamage(10);  // Aumenta el daño del jugador

            break;
        case ItemType::HELMET:
            player.recogerCasco();
            player.reduceDamage(0.9f);  // Reduce el daño en un 10%

            break;
    }
}

void Item::draw(sf::RenderTarget& target) {
    target.draw(_sprite);
}


