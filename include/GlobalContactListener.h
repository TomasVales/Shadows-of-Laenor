#pragma once
#include <box2d/box2d.h>
#include "Jugador.h"
#include "UserData.h"
#include "Maliketh.h"
#include <iostream>

class GlobalContactListener : public b2ContactListener {
public:

    //Se ejecuta cuando comienza el contacto fisico entre dos cuerpos en el mundo fisico, administra todas las interacciones en contacto.

    void BeginContact(b2Contact* contact) override {

       // Obtener los UserData de cada fixture

        void* bodyUserDataA = (void*)contact->GetFixtureA()->GetUserData().pointer;
        void* bodyUserDataB = (void*)contact->GetFixtureB()->GetUserData().pointer;

        if (!bodyUserDataA || !bodyUserDataB) return;

        auto* userDataA = static_cast<UserData*>(bodyUserDataA);
        auto* userDataB = static_cast<UserData*>(bodyUserDataB);



        //El jugador puede saltar cuando está en contacto con el suelo, sin importar si el PLAYER es userDataA o userDataB

        if (userDataA->type == UserDataType::PLAYER && userDataB->type == UserDataType::GROUND) {
            auto* player = static_cast<Jugador*>(userDataA->data);
            player->setCanJump(true);
        } else if (userDataA->type == UserDataType::GROUND && userDataB->type == UserDataType::PLAYER) {
            auto* player = static_cast<Jugador*>(userDataB->data);
            player->setCanJump(true);
        }



        // Ataque jefe a player - player a jefe.

        //Verificacion de colision

        if ((userDataA->type == UserDataType::PLAYER && userDataB->type == UserDataType::BOSS) ||
            (userDataA->type == UserDataType::BOSS && userDataB->type == UserDataType::PLAYER)) {

            std::cout << "El jefe colisionó con el jugador" << std::endl;



            Maliketh* boss = nullptr;
            Jugador* player = nullptr;

                //Asignacion de punteros

            if (userDataA->type == UserDataType::BOSS) {
                boss = static_cast<Maliketh*>(userDataA->data);
                player = static_cast<Jugador*>(userDataB->data);
            } else {
                boss = static_cast<Maliketh*>(userDataB->data);
                player = static_cast<Jugador*>(userDataA->data);
            }

            //Ataque jefe a Personaje

            boss->attack(*player);

            //Ataque personaje a Jefe.

            if (player->isAttacking()) {
                std::cout << "¡Ataque exitoso! Maliketh recibe daño." << std::endl;
                boss->takeDamage(40);
            }
        }


        // Detección de colisión con el esqueleto

        if ((userDataA->type == UserDataType::PLAYER && userDataB->type == UserDataType::SKELETON) ||
            (userDataA->type == UserDataType::SKELETON && userDataB->type == UserDataType::PLAYER)) {

            std::cout << "El esqueleto colisionó con el jugador" << std::endl;
            Maliketh* skeleton = nullptr;
            Jugador* player = nullptr;

            if (userDataA->type == UserDataType::SKELETON) {
                skeleton = static_cast<Maliketh*>(userDataA->data);
                player = static_cast<Jugador*>(userDataB->data);
            } else {
                skeleton = static_cast<Maliketh*>(userDataB->data);
                player = static_cast<Jugador*>(userDataA->data);
            }

            //Ataque esqueleto a Jugador.
            skeleton->attack(*player);

            //Ataque Jugador a esqueleto.
            if (player->isAttacking()) {
                std::cout << "¡Ataque exitoso! El esqueleto recibe daño." << std::endl;
                skeleton->takeDamage(20);
            }
        }

        // Detección de colisión con un ítem

        if ((userDataA->type == UserDataType::PLAYER && userDataB->type == UserDataType::ITEM) ||
            (userDataA->type == UserDataType::ITEM && userDataB->type == UserDataType::PLAYER)) {

            Jugador* player = nullptr;
            Item* item = nullptr;

            if (userDataA->type == UserDataType::PLAYER) {
                player = static_cast<Jugador*>(userDataA->data);
                item = static_cast<Item*>(userDataB->data);
            } else {
                player = static_cast<Jugador*>(userDataB->data);
                item = static_cast<Item*>(userDataA->data);
            }

            std::cout << "El jugador ha recogido un ítem." << std::endl;
            item->applyEffect(*player);
            item->marcarComoRecogido();
        }
    }

    //Se ejecuta cuando dos cuerpos dejan de estar en contacto.

    void EndContact(b2Contact* contact) override {

        // Obtener los UserData de cada fixture

        void* bodyUserDataA = (void*)contact->GetFixtureA()->GetUserData().pointer;
        void* bodyUserDataB = (void*)contact->GetFixtureB()->GetUserData().pointer;

        if (!bodyUserDataA || !bodyUserDataB) return;

        auto* userDataA = static_cast<UserData*>(bodyUserDataA);
        auto* userDataB = static_cast<UserData*>(bodyUserDataB);

        // Detectar cuando el jugador deja de tocar el suelo

        if (userDataA->type == UserDataType::PLAYER && userDataB->type == UserDataType::GROUND) {
            auto* player = static_cast<Jugador*>(userDataA->data);
            player->setCanJump(false);
        } else if (userDataA->type == UserDataType::GROUND && userDataB->type == UserDataType::PLAYER) {
            auto* player = static_cast<Jugador*>(userDataB->data);
            player->setCanJump(false);
        }
    }
};
