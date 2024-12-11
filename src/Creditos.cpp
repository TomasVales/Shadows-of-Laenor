#include "Creditos.h"
#include <iostream>

Creditos::Creditos(float width, float height)
    : currentFrame(0), frameDuration(sf::seconds(0.10f)), active(false), windowWidth(width), windowHeight(height) {
    loadResources();
}

void Creditos::loadResources() {
    // Cargar las texturas de los créditos
    for (int i = 1; i <= 302; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("creditos/frames/" + std::to_string(i) + ".png")) {
            std::cout << "Error al cargar la imagen de crédito " << i << std::endl;
        } else {
            creditFrames.push_back(texture);
        }
    }

    // Cargar la música de los créditos
    if (!musicBuffer.loadFromFile("creditos/creditos.ogg")) {
        std::cout << "Error al cargar la música de los créditos" << std::endl;
    } else {
        music.setBuffer(musicBuffer);
        music.setLoop(true); // Hacer que la música se repita durante los créditos
    }
}

void Creditos::start() {
    active = true;
    currentFrame = 0;
    elapsedTime = sf::Time::Zero;
    music.play();
}

void Creditos::stop() {
    active = false;
    music.stop();
}

void Creditos::update(sf::Time deltaTime) {
    if (!active) return;

    elapsedTime += deltaTime;

    // Verificar si es hora de avanzar al siguiente frame
    if (elapsedTime >= frameDuration) {
        elapsedTime = sf::Time::Zero;
        currentFrame = (currentFrame + 1) % creditFrames.size();
        creditSprite.setTexture(creditFrames[currentFrame]);
    }
}

void Creditos::draw(sf::RenderWindow& window) {
    if (active) {
        creditSprite.setPosition(windowWidth / 2 - creditSprite.getGlobalBounds().width / 2,
        windowHeight / 2 - creditSprite.getGlobalBounds().height / 2);
        window.draw(creditSprite);
    }
}

bool Creditos::isActive() const {
    return active;
}
