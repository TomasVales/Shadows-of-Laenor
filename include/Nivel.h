#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <box2d/box2d.h>
#include "UserData.h"
#include "Physics.h"
#include "Jugador.h"
#include "Maliketh.h"
#include "Kaldrax.h"
#include "Radahn.h"
#include "HealthBar.h"
#include "Zorathar.h"
#include "HealthBarBoss.h"
#include "Klosterian.h"
#include <vector>
#include "Item.h"
#include "Hud.h"


class Nivel {
public:
    Nivel(const std::string& fondoPath, bool tieneJefe);  // Constructor para configurar fondo y jefe
    ~Nivel();
    void cargarNivel(int nivel);                          // Cargar plataformas y otros elementos según el nivel
    void actualizar(float deltaTime, Jugador& jugador , sf::View& view);   // Actualizar elementos del nivel
    void renderizar(sf::RenderWindow& window , sf::View& view);            // Dibujar fondo y plataformas
    bool nivelCompleto() const;// Revisa si el nivel está completo
    std::vector<Item*> items;

private:
    int nivelactual;
    void addPlatform(size_t x, size_t y, size_t width);   // Agrega una plataforma
    bool jefeDerrotado;                                   // Estado del jefe (si ha sido derrotado)
    UserData sueloData{UserDataType::GROUND, nullptr};    // Datos del suelo
    sf::Texture fondoTextura;                             // Textura del fondo
    sf::Sprite spriteFondo;                               // Sprite para el fondo
    std::vector<b2Body*> plataformas;                     // Lista de plataformas
    Klosterian* klosterian; // Puntero para Klosterian
    Maliketh maliketh;
    Kaldrax kaldrax;
    Zorathar zorathar;
    Radahn radahn;
    bool tienejefeRadahn   = false;
    bool tieneJefeMaliketh = false;
    bool tieneJefeKaldrax  = false;
    bool tieneJefeZorathar = false;
    HealthBarBoss healthBarJefe{"UI/boss", "sprite", 300.0f};
    HealthBarBoss healthBarKaldrax{"UI/boss", "spriteKaldrax" , 300.0f};
    HealthBarBoss healthBarZorathar{"UI/boss", "spriteZorathar" , 300.0f};
    HealthBarBoss healthBarRadahn{"UI/boss" , "spriteRadahn" , 300.0f};
    HealthBar healthBar{"UI/sprite_00.png"};
    Hud hud;



    bool tieneJefe;                                       // Indica si el nivel tiene un jefe
};
