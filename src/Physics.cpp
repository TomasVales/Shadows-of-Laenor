#include "Physics.h"
#include <SFML/Graphics.hpp>
#include "GlobalContactListener.h"  // Agregar esta línea
#include <iostream>

b2World Physics::world{ b2Vec2(0.0 , 9.2) };


void Physics::init() {
    static GlobalContactListener contactListener;
    world.SetContactListener(&contactListener);
}

void Physics::Update(float deltaTime)
{

    world.Step(deltaTime , 6 , 2);

}
