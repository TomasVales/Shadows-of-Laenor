#pragma once
#include <box2d/box2d.h>



class Physics
{
    public:
        static void init();
        static void Update(float deltaTime);
        static b2World world;

};
