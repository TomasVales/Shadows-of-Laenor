#include "Game.h"
#include "Physics.h"
#include "Jugador.h"
#include "Maliketh.h"
#include "Kaldrax.h"
#include "HealthBar.h"
#include "HealthBarBoss.h"
#include "Nivel.h"
#include "muerte.h"
#include "DialogManager.h"
#include "Klosterian.h"
#include <iostream>
#include "Creditos.h"
#include <finJuego.h>
#include <SFML/System/Clock.hpp>

float dialogCooldown = 0.5f;  // Tiempo de cooldown en segundos
sf::Clock dialogClock;        // Reloj para medir el tiempo transcurrido

// Crear la instancia del nivel actual
Nivel* nivelActual;

int nivelActualIndex = 1;


DialogManager dialogManager;
Klosterian klosterian;

bool paused{};
bool isGameFinished = false;
bool isInRange = false;

// Declarar jugador

Jugador prota;
finJuego finalJuego;
Muerte muerteAnimacion;
sf::View vista;


sf::RectangleShape blackout(sf::Vector2f (1.0f, 1.0f));


//Le da un valor minimo y maximo a la camara para evitar que salga de rango del mapa.

float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


//Inicializa el Juego, configurando los niveles, al jugador, dialogos, vista, mapa, armaduras.


void Begin(const sf::Window& window)
{

    //Nivel Actual.
    nivelActualIndex = 1;

    vista.setSize(window.getSize().x / 2, window.getSize().y / 2);
    vista.setCenter(prota.getPosition().x, prota.getPosition().y);

    // Cargado de nivel.

    std::string nombreMapa;
    bool tieneJefe = false;
    if (nivelActualIndex == 1)
    {
        nombreMapa = "Mapa.png";
        guardarProgreso(1);
    }
    else if (nivelActualIndex == 2)
    {
        nombreMapa = "MALIKETH.png";
        tieneJefe = true;
    }
    else if (nivelActualIndex == 3)
    {
        nombreMapa = "KARLDRAXMAP.png";
    }
    else if (nivelActualIndex == 4)
    {
        nombreMapa = "KARLDRAXFIGHT.png";
        tieneJefe = true;
    }
    else if (nivelActualIndex == 5)
    {
        nombreMapa = "ZoratharMap.png";
    }
    else if (nivelActualIndex == 6)
    {
        nombreMapa = "ZoratharFigth.png";
        tieneJefe = true;

    }
    else if (nivelActualIndex == 7)
    {
        nombreMapa = "RADAHNMAP.png";


    }
    else if (nivelActualIndex == 8)
    {
        nombreMapa = "RADAHNFIGHT.png";
        tieneJefe = true;

    }

    if(nivelActualIndex >= 5)
    {
        prota.recogerCasco();
        prota.reduceDamage(0.8f);
        prota.recogerArmadura();
        prota.reduceDamage(0.8f);
        prota.recogerEspada();
        prota.increaseDamage(10);
    }

    //Inicia el nivel actual del juego cargando el mapa y los elementos de dicho mapa.

    nivelActual = new Nivel(nombreMapa, tieneJefe);

    nivelActual->cargarNivel(nivelActualIndex);


    //Inicializacion Klosterian - prota.
    klosterian.Begin();
    prota.Begin();

   dialogManager.loadDialog(
    {
        "Acercate joven...", "He escuchado historias sobre ti...", "Vagas por las tierras desiertas", "con el peso del fracaso sobre tus hombros", "te ofrezco una ultima oportunidad de redencion...",
        "tendras que romper la maldicion que asola a Laenor", "tendras que enfrentar a los tres guardianes corrompidos", "adelante, caballero caido. El reino aguarda tu redencion."
    });


    blackout.setFillColor(sf::Color(0, 0, 0, 150));
    blackout.setOrigin(0.5f, 0.5f);

    Physics::init();

}


//Actualiza la logica del juego, Fin del juego, Dialogo, Siguiente mapa, animacion muerte.


void Update(float deltaTime, sf::RenderWindow& window)
{

     // Cierra el juego al finalizar el juego y estar mostrandose los creditos.

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        if (isGameFinished || finalJuego.estaActivo()) {
            window.close();
            return;
        }
    }


    // Detiene la actualización si el juego terminó o está pausado
    if (paused || isGameFinished) return;

    if(finalJuego.estaActivo()){
        if(finalJuego.haTerminado()){
            isGameFinished = true;
        }
        return;
    }


    Physics::Update(deltaTime);
    prota.cmd();
    prota.update(deltaTime);

    //Animacion de muerte

    if (prota.getHealth() <= 0 && !muerteAnimacion.estaActiva())
    {
        std::cout << "MORISTE GG" << std::endl;
        muerteAnimacion.activar();
    }

    // Reinicia desde el nivel guardado cuando la animación termina

    if (muerteAnimacion.estaActiva() && muerteAnimacion.haTerminado())
    {
        nivelActualIndex = cargarProgreso();
        muerteAnimacion.desactivar();
        prota.RecuperarVida();
        prota.setPosition(sf::Vector2f(100, prota.getPosition().y));
    }


    // Clamp nos ayuda para no salir del limite de la pantalla
    vista.setCenter(
        clamp(prota.getPosition().x, vista.getSize().x / 2, 1360 - vista.getSize().x / 2),
        clamp(prota.getPosition().y, vista.getSize().y / 2, 768 - vista.getSize().y / 2)
    );



    //Cargar siguiente nivel

    if (nivelActual)
    {
        nivelActual->actualizar(deltaTime, prota, vista);


        if (nivelActualIndex == 1 && prota.haLlegadoAlFinal(960))
        {
            cargarNuevoNivel(++nivelActualIndex, "MALIKETH.png", true);

        }
        else if (nivelActualIndex == 2 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1330))
        {
            cargarNuevoNivel(++nivelActualIndex, "KARLDRAXMAP.png", false);

        }
        else if (nivelActualIndex == 3 && prota.haLlegadoAlFinal(1250))
        {
            cargarNuevoNivel(++nivelActualIndex, "KARLDRAXFIGHT.png", true);

        }
        else if (nivelActualIndex == 4 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1330))
        {
            cargarNuevoNivel(++nivelActualIndex, "ZoratharMap.png", false);

        }
        else if (nivelActualIndex == 5 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1320))
        {
            cargarNuevoNivel(++nivelActualIndex, "ZoratharFigth.png", true);
        }
        else if (nivelActualIndex == 6 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1320))
        {
            cargarNuevoNivel(++nivelActualIndex, "RADAHNMAP.png", false);
        }
        else if (nivelActualIndex == 7 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1320))
        {
            cargarNuevoNivel(++nivelActualIndex, "RADAHNFIGHT.png", true);
        }
        else if (nivelActualIndex == 8 && nivelActual->nivelCompleto() && prota.haLlegadoAlFinal(1320))
        {
            std::cout<<"Activando creditos..."<<std::endl;
            finalJuego.activar();
            delete nivelActual;
            nivelActual = nullptr;

        }
    }

    float rangeX = 90.0f; // Rango horizontal
    float rangeY = 150.0f; // Rango vertical

    isInRange = estaDentroDeRango(prota.getPosition(), klosterian.getPosition(), rangeX, rangeY);



                // Dialogo

    if (isInRange && nivelActualIndex == 1)
    {

        dialogManager.letraEsprite.setPosition(255, 560);
        dialogManager.letraVisible = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            if(dialogClock.getElapsedTime().asSeconds() >= dialogCooldown)
            {
                if (!dialogManager.isActive())
                {
                    dialogManager.startDialog();
                }
                else
                {
                    dialogManager.nextFrame();
                }
                dialogClock.restart();
            }

        }

    }
    else
    {
        dialogManager.letraVisible = false;
        dialogManager.stopDialog();
    }
}

//Renderiza los elementos en pantalla, creditos, muerte animacion, niveles, dialogo.

void Render(sf::RenderWindow& window, float deltaTime)
{
    if (finalJuego.estaActivo())
    {
        window.setView(window.getDefaultView());
        finalJuego.mostrarCreditos(window, deltaTime);
        return; // No dibujar nada mas mientras los creditos esten activos
    }

    window.setView(vista);
    if (muerteAnimacion.estaActiva())
    {
        window.setView(window.getDefaultView());
        muerteAnimacion.mostrarMuerte(window, deltaTime);  // Solo muestra la pantalla de muerte
    }
    else if (nivelActual)
    {
        nivelActual->renderizar(window, vista);  // Dibujar el nivel actual
        window.draw(prota); // Dibujar al jugador

        dialogManager.draw(window, klosterian.getPosition());
    }



    if (paused)
    {

        blackout.setScale(5000, 5000);
        window.draw(blackout);
    }
    if(dialogManager.letraVisible)
    {
        window.draw(dialogManager.letraEsprite);
    }
}


//Guarda el proceso del nivel.


void guardarProgreso(int nivelActualIndex)
{
    FILE* archivo = fopen("savegame.dat", "wb");
    if (archivo != nullptr)
    {
        fwrite(&nivelActualIndex, sizeof(int), 1, archivo);
        fclose(archivo);
        std::cout << "Progreso guardado correctamente en el nivel: " << nivelActualIndex << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar el progreso." << std::endl;
    }
}

//Carga el proceso o inicia en nivel 1 si no hay proceso.

int cargarProgreso()
{
    int nivelCargado = 1; // Nivel por defecto si no hay guardado previo
    FILE* archivo = fopen("savegame.dat", "rb");
    if (archivo != nullptr)
    {
        fread(&nivelCargado, sizeof(int), 1, archivo);
        fclose(archivo);
        std::cout << "Progreso cargado correctamente, nivel: " << nivelCargado << std::endl;
    }
    else
    {
        std::cerr << "No se encontró un archivo de guardado. Comenzando desde el nivel 1." << std::endl;
    }
    return nivelCargado;
}

//Cambia al siguiente nivel, configurando su mapa, su jefe y al jugador (posicion y vida).

void cargarNuevoNivel(int nivelActualIndex, const std::string& nombreMapa, bool tieneJefe)
{
    guardarProgreso(nivelActualIndex);
    delete nivelActual;  // Libera la memoria del nivel anterior

    nivelActual = new Nivel(nombreMapa, tieneJefe);
    nivelActual->cargarNivel(nivelActualIndex);
    prota.setPosition(sf::Vector2f(100, prota.getPosition().y));  // Posición por defecto
    prota.RecuperarVida();

}

//Verifica si el jugador esta dentro de un rango determinado

bool estaDentroDeRango(const sf::Vector2f& posicionJugador, const sf::Vector2f& posicionObjetivo, float rangeX, float rangeY)
{
    return (std::abs(posicionJugador.x - posicionObjetivo.x) <= rangeX) &&
           (std::abs(posicionJugador.y - posicionObjetivo.y) <= rangeY);
}

//lo que hace es dar un valor absoluto , para verificar , en el area si se encuentra el jugador



