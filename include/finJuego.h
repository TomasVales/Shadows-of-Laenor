#ifndef FINJUEGO_H
#define FINJUEGO_H
#include <SFML/Graphics.hpp>

class finJuego
{
public:
    finJuego();
    void mostrarCreditos(sf::RenderWindow& window, float deltaTime);
    void activar();
    bool estaActivo() const;
    bool haTerminado() const;
    void desactivar();

    private:
        std::vector<sf::Texture> creditosFrames;
        sf::Sprite creditosSprite;
        bool activo = false;
        int frameActual = 0;
        float tiempoFrame = 0.05f;
        float tiempoAcumulado = 0.0f;
};

#endif // FINJUEGO_H
