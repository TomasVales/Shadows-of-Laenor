#include "Hud.h"

Hud::Hud() : healthBarJugador("UI/sprite_00.png") {
    // Cargar las texturas (asegúrate de que los archivos existan)
    if (!texturaArmadura.loadFromFile("UI/Armadura.png")) {
        std::cerr << "Error al cargar la textura de armadura." << std::endl;
    }
    _spriteArmadura.setTexture(texturaArmadura);
    _spriteArmadura.setScale(0.25f, 0.25f);

    if (!texturaEspada.loadFromFile("UI/espada5.png")) {
        std::cerr << "Error al cargar la textura de espada." << std::endl;
    }
    _spriteEspada.setTexture(texturaEspada);
    _spriteEspada.setScale(0.045f, 0.045f);

    if (!texturaCasco.loadFromFile("UI/casco.png")) {
        std::cerr << "Error al cargar la textura de casco." << std::endl;
    }
    _spriteCasco.setTexture(texturaCasco);
    _spriteCasco.setScale(0.25f, 0.25f);
}

void Hud::update(float deltaTime, sf::View& view, Jugador& player) {

    // Actualiza la posición y la salud de la barra de vida del jugador
    healthBarJugador.setHealth(player.getHealth());
    healthBarJugador.updatePosition(view);

    // Actualizar estado de los ítems
    armadura = player.getArmadura();
    casco = player.getCasco();
    espada = player.getEspada();
}

void Hud::render(sf::RenderWindow& window, sf::View& view) {
    healthBarJugador.draw(window, view);

    // Dibujar ítems recogidos en el HUD
    if (armadura) {
        _spriteArmadura.setPosition(view.getCenter().x - view.getSize().x / 2 + 48.0f,
                                    view.getCenter().y + view.getSize().y / 2 - _spriteArmadura.getGlobalBounds().height - 353.0f);
        window.draw(_spriteArmadura);
    }

    if (espada) {
        _spriteEspada.setPosition(view.getCenter().x - view.getSize().x / 2 + 80.0f,
                                  view.getCenter().y + view.getSize().y / 2 - _spriteEspada.getGlobalBounds().height - 353.0f);
        window.draw(_spriteEspada);
    }

    if (casco) {
        _spriteCasco.setPosition(view.getCenter().x - view.getSize().x / 2 + 112.0f,
                                 view.getCenter().y + view.getSize().y / 2 - _spriteCasco.getGlobalBounds().height - 353.0f);
        window.draw(_spriteCasco);
    }
}
