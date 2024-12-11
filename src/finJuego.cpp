#include "finJuego.h"
#include <iostream>
#include <string>

finJuego::finJuego()
{
    for (int i = 1; i < 52; i++){
        sf::Texture textura;
        std::string ruta = "creditosFin/ezgif-frame-0" + std::to_string(i) + ".jpg";
        if(!textura.loadFromFile(ruta)){
            std::cout<<"Error cargando la textura" << ruta << std::endl;
        }
        creditosFrames.push_back(textura);
    }

    if(!creditosFrames.empty()){
        creditosSprite.setTexture(creditosFrames[0]);
    }
}

void finJuego::activar(){
    activo = true;
    frameActual = 0;
    tiempoAcumulado = 0.0f;

    if(!creditosFrames.empty()){
        creditosSprite.setTexture(creditosFrames[0]);
    }
}

void finJuego::desactivar(){
    activo = false;
    frameActual = 0;
    tiempoAcumulado = 0.0f;
}

bool finJuego::estaActivo() const {
    return activo;
}

bool finJuego::haTerminado() const{
    return activo && frameActual >= creditosFrames.size() - 1;
}

void finJuego::mostrarCreditos(sf::RenderWindow& window, float deltaTime){
    if(!activo || creditosFrames.empty()){
        return;
    }

    tiempoAcumulado += deltaTime;
    if(tiempoAcumulado >= tiempoFrame){
        tiempoAcumulado = 0.0f;

        if(frameActual < creditosFrames.size() - 1){
            frameActual++;
                creditosSprite.setTexture(creditosFrames[frameActual]);
        }
    }
    creditosSprite.setScale(
      static_cast<float>(window.getSize().x) / creditosSprite.getTexture()->getSize().x,
      static_cast<float>(window.getSize().y) / creditosSprite.getTexture()->getSize().y
    );

    window.draw(creditosSprite);
}
