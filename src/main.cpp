#include <windows.h>  // Para manipular la ventana del CMD
#include <iostream>   // Para imprimir mensajes en consola
#include "Game.h"
#include "Jugador.h"
#include "Physics.h"
#include "menu.h"     // Incluir el menú principal
#include "pausa.h"    // Incluir el menú de pausa
#include "Creditos.h" // Incluir la clase de créditos
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_body.h>

// Función para establecer el ícono de la consola
void setConsoleIcon(const std::string& iconPath) {
    HICON hIcon = (HICON)LoadImage(NULL, iconPath.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (!hIcon) {
        std::cerr << "No se pudo cargar el ícono para la consola." << std::endl;
        return;
    }

    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole) {
        SendMessage(hwndConsole, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwndConsole, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
}

int main() {
    // Configurar el ícono de la consola
    setConsoleIcon("icon/5556.ico");

    // Crear ventana del juego
    sf::RenderWindow window(sf::VideoMode(1360, 768), "Shadows of Laenor");
    window.setFramerateLimit(60);

    // Configuración de reloj y delta time
    sf::Clock deltaClock;
    sf::Clock clock;

    // Inicialización de clases
    pausa pausaMenu;
    Menu menu(window.getSize().x, window.getSize().y);
    Creditos creditos(window.getSize().x, window.getSize().y);
    Jugador prota;

    // Variables de estado
    bool isMenuActive = true;
    bool isPaused = false;
    bool startGame = false;
    bool inCredits = false;





    // Configuración de música
    sf::Music musicafondo;
    if (!musicafondo.openFromFile("musicafondo.ogg")) {
        std::cerr << "No se pudo cargar la música de fondo." << std::endl;
    } else {
        musicafondo.setVolume(10);
        musicafondo.setLoop(true);
    }

    // Configuración del ícono de la ventana
    sf::Image icon;
    if (!icon.loadFromFile("icon/pp.png")) {
        std::cerr << "No se pudo cargar el ícono para el juego." << std::endl;
    } else {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    // Iniciar el menú principal
    Begin(window);


   // GAME LOOP

while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Time elapsed = clock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            // Cerrar ventana
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejo del menú principal
            if (isMenuActive) {
                menu.handleInput(event, isMenuActive, window);

                if (!isMenuActive) { // El usuario seleccionó algo en el menú
                    if (menu.getSelectedOption() == 1) { // Opción de créditos
                        inCredits = true;
                        creditos.start();
                        musicafondo.pause();
                    } else if (menu.getStartGameOptionSelected()) { // Opción de iniciar juego
                        startGame = true;
                        musicafondo.play();
                    }
                }
            }

            // Manejo de créditos
            else if (inCredits) {

                // Salir de los créditos manualmente con Escape

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    creditos.stop(); // Detiene los créditos manualmente
                    inCredits = false;
                    isMenuActive = true;
                    musicafondo.stop();
                }
            }

            // Manejo del menú de pausa
            else if (isPaused) {
                pausaMenu.update(event, isPaused, isMenuActive);
                if (pausaMenu.isExitOptionSelected()) {
                    window.close();
                }
            }

            // Manejo del juego principal
            else if (startGame) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    isPaused = !isPaused;
                    pausaMenu.toggle();
                }
            }
        }

        // Lógica de actualización automática para créditos
        if (inCredits && !creditos.isActive()) { // Créditos han terminado automáticamente
            creditos.stop();
            inCredits = false;
            isMenuActive = true;
            musicafondo.play();
        }

        // Renderizado

        window.clear();

        if (inCredits) {
            creditos.update(sf::seconds(deltaTime));
            creditos.draw(window);
        } else if (isMenuActive) {
            menu.updateTitleAnimation(elapsed);
            menu.draw(window);
        } else if (isPaused) {
            pausaMenu.draw(window);
        } else if (startGame) {
            Update(deltaTime, window);
            Render(window, deltaTime);
        }

        window.display();
    }

    return 0;
}
