#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Muerte {
public:
    Muerte();
    void mostrarMuerte(sf::RenderWindow& window, float deltaTime);  // Dibuja la pantalla de muerte
    void activar();  // Activa la animaci�n de muerte
    bool estaActiva() const;  // Verifica si la animaci�n de muerte est� activa
    bool haTerminado() const;  // Indica si la animaci�n ha terminado
    void desactivar();



private:
    std::vector<sf::Texture> muerteFrames;
    sf::Sprite muerteSprite;
    bool activo = false;
    int frameActual = 0;  // �ndice del frame actual
    float tiempoFrame = 0.05f;  // Tiempo entre frames en segundos
    float tiempoAcumulado = 0.0f;  // Acumulador para el tiempo de cambio de frame
};
