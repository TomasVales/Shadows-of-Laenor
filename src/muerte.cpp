#include "muerte.h"
#include <iostream>
#include <string>

Muerte::Muerte() {
    // Cargar las 98 texturas de los frames de la animación
    for (int i = 0; i < 98; ++i) {
        sf::Texture textura;
        std::string ruta = "youdied/ezgif-frame-" + std::to_string(i) + ".png";
        if (!textura.loadFromFile(ruta)) {
            std::cerr << "Error cargando la textura " << ruta << std::endl;
        }
        muerteFrames.push_back(textura);
    }

    // Asignar el primer frame al sprite si hay texturas disponibles
    if (!muerteFrames.empty()) {
        muerteSprite.setTexture(muerteFrames[0]);
    }
}

void Muerte::activar() {
    activo = true;
    frameActual = 0;
    tiempoAcumulado = 0.0f;

    if (!muerteFrames.empty()) {
        muerteSprite.setTexture(muerteFrames[0]);
    }
}

void Muerte::desactivar() {
    activo = false;
    frameActual = 0;
    tiempoAcumulado = 0.0f;
}

bool Muerte::estaActiva() const {
    return activo;
}

bool Muerte::haTerminado() const {
    return activo && frameActual >= muerteFrames.size() - 1;
}

void Muerte::mostrarMuerte(sf::RenderWindow& window, float deltaTime) {
    if (!activo || muerteFrames.empty()) {
        return;
    }

    // Actualizar el frame basado en el tiempo acumulado
    tiempoAcumulado += deltaTime;
    if (tiempoAcumulado >= tiempoFrame) {
        tiempoAcumulado = 0.0f;

        if (frameActual < muerteFrames.size() - 1) {
            frameActual++;
            muerteSprite.setTexture(muerteFrames[frameActual]);
        }
    }

    // Escalar el sprite a las dimensiones de la ventana
    muerteSprite.setScale(
        static_cast<float>(window.getSize().x) / muerteSprite.getTexture()->getSize().x,
        static_cast<float>(window.getSize().y) / muerteSprite.getTexture()->getSize().y
    );

    // Dibujar el sprite en la ventana
    window.draw(muerteSprite);
}
