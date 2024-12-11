// Creditos.h
#ifndef CREDITOS_H
#define CREDITOS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Creditos {
public:
    // Constructor y destructor
    Creditos(float width, float height);

    // M�todos principales
    void start();                            // Inicia los cr�ditos (incluye m�sica)
    void stop();                             // Detiene los cr�ditos (pausa m�sica y resetea estados)
    void update(sf::Time elapsedTime);       // Actualiza la animaci�n de los cr�ditos
    void draw(sf::RenderWindow& window);     // Dibuja el frame actual en la ventana

    // M�todo para verificar si los cr�ditos est�n activos
    bool isActive() const;

private:
    // M�todos privados para los creditos
    void loadResources();                    // Carga las texturas y la m�sica para los cr�ditos
    void updateFrame();                      // Actualiza el frame de la animaci�n de cr�ditos

    // Variables para controlar los recursos
    std::vector<sf::Texture> creditFrames;   // Vector de texturas para cada imagen de los cr�ditos
    sf::Sprite creditSprite;                 // Sprite que muestra cada imagen de los cr�ditos
    sf::SoundBuffer musicBuffer;             // Buffer de sonido para la m�sica de cr�ditos
    sf::Sound music;                         // Sonido que reproduce la m�sica de cr�ditos

    // Control de la animaci�n de los cr�ditos
    size_t currentFrame;
    sf::Time frameDuration;                  // Duraci�n de cada frame
    sf::Time elapsedTime;                    // Tiempo acumulado para cambiar de frame
    bool active;                             // Indica si los cr�ditos est�n activos

    // Dimensiones de la ventana para ajustar el tama�o del contenido
    float windowWidth;
    float windowHeight;
};

#endif // CREDITOS_H
