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

    // Métodos principales
    void start();                            // Inicia los créditos (incluye música)
    void stop();                             // Detiene los créditos (pausa música y resetea estados)
    void update(sf::Time elapsedTime);       // Actualiza la animación de los créditos
    void draw(sf::RenderWindow& window);     // Dibuja el frame actual en la ventana

    // Método para verificar si los créditos están activos
    bool isActive() const;

private:
    // Métodos privados para los creditos
    void loadResources();                    // Carga las texturas y la música para los créditos
    void updateFrame();                      // Actualiza el frame de la animación de créditos

    // Variables para controlar los recursos
    std::vector<sf::Texture> creditFrames;   // Vector de texturas para cada imagen de los créditos
    sf::Sprite creditSprite;                 // Sprite que muestra cada imagen de los créditos
    sf::SoundBuffer musicBuffer;             // Buffer de sonido para la música de créditos
    sf::Sound music;                         // Sonido que reproduce la música de créditos

    // Control de la animación de los créditos
    size_t currentFrame;
    sf::Time frameDuration;                  // Duración de cada frame
    sf::Time elapsedTime;                    // Tiempo acumulado para cambiar de frame
    bool active;                             // Indica si los créditos están activos

    // Dimensiones de la ventana para ajustar el tamaño del contenido
    float windowWidth;
    float windowHeight;
};

#endif // CREDITOS_H
