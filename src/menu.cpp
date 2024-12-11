#include "Menu.h"
#include "Creditos.h"
#include <iostream>
using namespace std;

// Constructor de la clase Menu, inicializa el men� con las configuraciones iniciales
Menu::Menu(float width, float height) : currentFrame(0), frameDuration(sf::seconds(0.1f)), selectedOptionIndexOptions(0) {
    // Inicializaci�n de recursos
    if (!buffer.loadFromFile("fuentes/musicamenu.ogg")) {
        cout << "Error al cargar la musica" << endl;
    }

    // Restablece el texto para el estado de la m�sica
    musicStatusText.setString("Musica: ON");
    sound.setBuffer(buffer);
    sound.setLoop(true); // Hacer que la m�sica se repita
    sound.setVolume(30); // Establecer el volumen inicial
    sound.play(); // Reproducir la m�sica

    // Cargar el sonido de movimiento entre las opciones
    if (!moveBuffer.loadFromFile("fuentes/boton.ogg")) {
        cout << "Error al cargar el sonido de movimiénto" << endl;
    }
    moveSound.setBuffer(moveBuffer); // Asignar el buffer de sonido
    isPlaySubmenuActive = false;


    // Cargar la textura de fondo para el men�
    if (!backgroundTexture.loadFromFile("fuentes/fondo1.png")) {
        cout << "Error al cargar la téxtura de fondo" << endl;
    }
    // Ajustar el fondo a la pantalla
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundTexture.getSize().x, height / backgroundTexture.getSize().y);

    // Cargar las im�genes para la animaci�n del t�tulo
    for (int i = 1; i <= 12; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("titulo/" + to_string(i) + ".png")) {
            cout << "Error al cargar la imagén del t�tulo " << i << endl;
        }
        titleFrames.push_back(texture); // Guardar las texturas en un vector
    }

    titleSprite.setTexture(titleFrames[0]); // Establecer la primera imagen del t�tulo
    titleSprite.setPosition(width / 2 - titleFrames[0].getSize().x / 2, 340); // Posicionar el t�tulo

    // Cargar la fuente del texto del men�
    if (!font.loadFromFile("fuentes/im.ttf")) {
        cout << "Error al cargar la fuénté" << endl;
    }

    // Inicializa el men�
    string options[] = {"Salir", "Creditos", "Opciones", "Jugar"};
    float startPosY = height - 150;
    float offset = 50;

    // Crear los textos de las opciones del men� principal
    for (size_t i = 0; i < 4; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(i == 3 ? sf::Color(255, 215, 0) : sf::Color::White); // Resaltar la opci�n seleccionada
        text.setString(options[i]); // Asignar el texto de la opci�n
        text.setPosition(sf::Vector2f(width / 2 - text.getGlobalBounds().width / 2, startPosY - offset * i)); // Posicionar las opciones
        menuOptions.push_back(text); // Guardar cada opci�n en el vector de opciones
    }

    selectedOptionIndex = 3; // Inicializar la opci�n seleccionada en "Jugar"

    // Configuraci�n del texto para el volumen de m�sica
    volumeText.setFont(font);
    volumeText.setCharacterSize(30);  // Tama�o de la fuente
    volumeText.setFillColor(sf::Color::White); // Color del texto
    volumeText.setPosition(width / 2 - 50, height / 2 - 50); // Posici�n en pantalla
    volumeText.setString("Volumen Musica: " + to_string(static_cast<int>(sound.getVolume())) + "%");

    // Configuraci�n del texto para mostrar el estado de la m�sica (encendido/apagada)
    musicStatusText.setFont(font);
    musicStatusText.setCharacterSize(30);
    musicStatusText.setFillColor(sf::Color::Yellow); // Color amarillo para diferenciarlo
    musicStatusText.setPosition(width / 2 - 50, height / 2); // Posici�n en pantalla

}



Menu::~Menu() {}


void Menu::moveUpPlaySubmenu() {
    if (selectedOptionIndexPlaySubmenu - 1 >= 0) {
        playSubmenuOptions[selectedOptionIndexPlaySubmenu].setFillColor(sf::Color::White);
        selectedOptionIndexPlaySubmenu--;
        playSubmenuOptions[selectedOptionIndexPlaySubmenu].setFillColor(sf::Color(255, 215, 0));
        moveSound.play();
    }
}

void Menu::moveDownPlaySubmenu() {
    if (selectedOptionIndexPlaySubmenu + 1 < playSubmenuOptions.size()) {
        playSubmenuOptions[selectedOptionIndexPlaySubmenu].setFillColor(sf::Color::White);
        selectedOptionIndexPlaySubmenu++;
        playSubmenuOptions[selectedOptionIndexPlaySubmenu].setFillColor(sf::Color(255, 215, 0));
        moveSound.play();
    }
}

bool Menu::getStartGameOptionSelected() const {
    return startGameSelected;
}

void Menu::draw(sf::RenderWindow &window) {
    // Dibujar los créditos si están activos
    if (getIsCreditsPlaying()) {
        drawCredits(window); // Dibujar la pantalla de créditos
    } else {
        // Dibujar el menú principal u opciones
        window.draw(backgroundSprite);
        window.draw(titleSprite);

        if (isOptionsScreen) {
            drawOptions(window); // Dibujar las opciones
        } else {
            for (const auto &option : menuOptions) {
                window.draw(option); // Dibujar las opciones del menú principal
            }
        }
    }
}

void Menu::moveUp() {
    if (!isOptionsScreen && selectedOptionIndex - 1 >= 0) { // Verificar que no est� en la pantalla de opciones y que haya opciones por encima
        menuOptions[selectedOptionIndex].setFillColor(sf::Color::White); // Desmarcar la opci�n actual
        selectedOptionIndex--; // Cambiar la opci�n seleccionada
        menuOptions[selectedOptionIndex].setFillColor(sf::Color(255, 215, 0)); // Resaltar la nueva opci�n
        moveSound.play(); // Reproducir sonido de movimiento
    } else if (isOptionsScreen && selectedOptionIndexOptions > 0) { // Si est� en la pantalla de opciones
        selectedOptionIndexOptions--; // Cambiar la selecci�n dentro de las opciones
        moveSound.play(); // Reproducir sonido de movimiento
    }
}

void Menu::moveDown() {
    if (!isOptionsScreen && selectedOptionIndex + 1 < menuOptions.size()) {
        menuOptions[selectedOptionIndex].setFillColor(sf::Color::White);
        selectedOptionIndex++;
        menuOptions[selectedOptionIndex].setFillColor(sf::Color(255, 215, 0));
        moveSound.play();
    } else if (isOptionsScreen && selectedOptionIndexOptions < 1) {
        selectedOptionIndexOptions++;
        moveSound.play();
    }
}

int Menu::getSelectedOption() const {
    return selectedOptionIndex;
}

void Menu::decreaseVolume() {
    float currentVolume = sound.getVolume();
    setVolume(currentVolume - 10.0f); // Aseg�rate de usar setVolume para validar el rango

    // Redondear el volumen al entero m�s cercano
    int roundedVolume = static_cast<int>(sound.getVolume() + 0.5f);
    setVolume(static_cast<float>(roundedVolume)); // Aseg�rate de que se mantenga como float

}

void Menu::increaseVolume() {
    float currentVolume = sound.getVolume();
    setVolume(currentVolume + 10.0f); // Aseg�rate de usar setVolume para validar el rango

    // Redondear el volumen al entero m�s cercano
    int roundedVolume = static_cast<int>(sound.getVolume() + 0.5f);
    setVolume(static_cast<float>(roundedVolume)); // Aseg�rate de que se mantenga como float

}

int Menu::getSelectedOptionIndexOptions() const {
    return selectedOptionIndexOptions;
}

void Menu::updateTitleAnimation(sf::Time elapsedTime) {
    // Solo avanza si no est� en el �ltimo frame
    if (currentFrame < titleFrames.size() - 1) {
        totalElapsedTime += elapsedTime;

        if (totalElapsedTime >= frameDuration) {
            totalElapsedTime -= frameDuration;  // Mantiene el excedente
            currentFrame++;  // Avanza al siguiente frame

            // Cambia la textura al siguiente frame
            titleSprite.setTexture(titleFrames[currentFrame]);
        }
    }
}

void Menu::drawOptions(sf::RenderWindow &window) {
    window.draw(backgroundSprite);

    sf::Text optionsText;
    optionsText.setFont(font);


    optionsText.setCharacterSize(50);
    optionsText.setFillColor(selectedOptionIndexOptions == 3 ? sf::Color(255, 215, 0) : sf::Color::White);
    optionsText.setPosition(window.getSize().x / 2 - optionsText.getGlobalBounds().width / 2, 700);
    window.draw(optionsText);

    // Actualizar y dibujar la opci�n de Volumen M�sica
    volumeText.setFillColor(selectedOptionIndexOptions == 0 ? sf::Color(255, 215, 0) : sf::Color::White);
    window.draw(volumeText);

    // Actualizar y dibujar la opci�n de M�sica ON/OFF
    musicStatusText.setString(isMusicPlaying ? "Musica: ON" : "Musica: OFF");
    musicStatusText.setFillColor(selectedOptionIndexOptions == 1 ? sf::Color(255, 215, 0) : sf::Color::White);
    window.draw(musicStatusText);

}

void Menu::activateOptionsScreen() {
    isOptionsScreen = true;  // Activa las opciones

}

void Menu::deactivateOptionsScreen() {
    isOptionsScreen = false;
}

void Menu::handleOptionSelection() {
    if (selectedOptionIndex == 3) {  // Si "Jugar" est� seleccionado
        isPlaySubmenuActive = true;  // Activar el submen� "Jugar"
    } else if (selectedOptionIndexOptions == 0) {
        // Control de volumen
    } else if (selectedOptionIndexOptions == 1) {
        toggleMusic();
    } else if (selectedOptionIndexOptions == 2) {
        // Cambiar el idioma
    } else if (selectedOptionIndexOptions == 3) {
        deactivateOptionsScreen();  // Volver al men� principal
    }
}

bool Menu::getIsOptionsScreen() const {
    return isOptionsScreen;
}

void Menu::setVolume(float volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    } else if (volume > 100.0f) {
        volume = 100.0f;
    }
    sound.setVolume(volume);
    volumeText.setString("Volumen musica: " + std::to_string(static_cast<int>(volume)) + "%");
}

void Menu::startCredits() {
    currentFrame = 0;  // Reiniciar el frame al iniciar la animaci�n
    totalElapsedTime = sf::Time::Zero;  // Reiniciar el tiempo
    isCreditsPlaying = true;

    creditsMusic.setVolume(50);  // Ajustar el volumen
    creditsMusic.play();
}

void Menu::stopCredits() {
    isCreditsPlaying = false;    // Indicar que los créditos han terminado
    creditsMusic.stop();         // Detener la música de créditos
    currentFrame = 0;            // Reiniciar la animación de los créditos
    totalElapsedTime = sf::Time::Zero; // Reiniciar el temporizador

    // Reanudar la música del menú
    if (isMusicPlaying && sound.getStatus() != sf::Sound::Playing) {
        sound.play();
    }
}

void Menu::drawCredits(sf::RenderWindow &window) {
    totalElapsedTime += frameDuration;
    if (totalElapsedTime >= frameDuration) {
        totalElapsedTime = sf::Time::Zero;
        currentFrame++;
        if (currentFrame >= creditFrames.size()) {
            currentFrame = creditFrames.size() - 1;
        }
        creditsSprite.setTexture(creditFrames[currentFrame]);
    }

    window.draw(creditsSprite);
}

void Menu::toggleMusic() {
    isMusicPlaying = !isMusicPlaying;

    if (isMusicPlaying) {
        if (sound.getStatus() != sf::Sound::Playing) {
            sound.play();  // Solo reproducir si no est� ya en reproducci�n
        }
        musicStatusText.setString("M�sica: ON");
    } else {
        sound.stop();  // Detener completamente la m�sica
        musicStatusText.setString("M�sica: OFF");
    }
}

bool Menu::getIsMusicOn() const {
    return isMusicOn;
}

void Menu::detenerMusica() {
    sound.stop();
}

void Menu::handleInput(sf::Event &event, bool &isMenuActive, sf::RenderWindow &window) {
    if (event.type == sf::Event::KeyPressed) {

        // Manejo del menú principal
        if (!getIsOptionsScreen() && !getIsCreditsPlaying()) {
            updateTitleAnimation(sf::seconds(1.0f / 60.0f));

            if (event.key.code == sf::Keyboard::Up) {
                moveDown(); // Subir en el menú principal
            } else if (event.key.code == sf::Keyboard::Down) {
                moveUp(); // Bajar en el menú principal
            } else if (event.key.code == sf::Keyboard::Enter) {
                int selectedOption = getSelectedOption();
                if (selectedOption == 3) {
                    startGameSelected = true;
                    isMenuActive = false;
                    sound.stop();
                } else if (selectedOption == 2) {
                    activateOptionsScreen(); // Activa las opciones
                } else if (selectedOption == 1) {
                    isMenuActive = false;
                    detenerMusica();
                } else if (selectedOption == 0) {
                    window.close();
                }
            }
        }


        // Manejo del menú de opciones
        if (getIsOptionsScreen()) {
            int selectedOptionIndex = getSelectedOptionIndexOptions();

            if (event.key.code == sf::Keyboard::Up) {
                moveUp();
            } else if (event.key.code == sf::Keyboard::Down) {
                moveDown();
            } else if (event.key.code == sf::Keyboard::Left && selectedOptionIndex == 0) {
                decreaseVolume();
            } else if (event.key.code == sf::Keyboard::Right && selectedOptionIndex == 0) {
                increaseVolume();
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (selectedOptionIndex == 1) toggleMusic();
                else if (selectedOptionIndex == 3) {
                    deactivateOptionsScreen();  // Salir de las opciones
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                deactivateOptionsScreen();  // Salir de las opciones
            }
        }
    }
}



