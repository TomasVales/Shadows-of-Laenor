// Item.h
#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Jugador.h"

enum class ItemType { ARMOR, SWORD, HELMET };

class Item : public sf::Drawable {
public:
    Item(ItemType type,  const sf::Vector2f& position);
    void applyEffect(Jugador& player);
    void draw(sf::RenderTarget& target);
    b2Body* getBody() const { return body; }
    bool isRecogido() const { return Recogido; }  // Verificar si el ítem fue recogido
    void marcarComoRecogido() { Recogido = true; }  // Marcar ítem como recogido
    ItemType getType(){return _type;}
    sf::Sprite getSprite(){return _sprite;}

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(_sprite);  // Dibuja el sprite del item
    }
    ItemType _type;
    sf::Texture _texture;
    sf::Sprite _sprite;
    b2Body* body;
    bool Recogido = false;
};

