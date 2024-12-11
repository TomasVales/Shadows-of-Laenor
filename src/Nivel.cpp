
#include "Nivel.h"
#include "Creditos.h"
#include <iostream>


//Inicializa el nivel, carga la textura y define si un nivel tiene jefe

Nivel::Nivel(const std::string& fondoPath, bool tieneJefe)
    : tieneJefe(tieneJefe), jefeDerrotado(false), klosterian(nullptr) {


    if (!fondoTextura.loadFromFile(fondoPath)) {
        std::cerr << "Error al cargar la textura del fondo!" << std::endl;
    }

    spriteFondo.setTexture(fondoTextura); // Asigna la textura cargada al sprite del fondo
}

Creditos creditos(1360, 768); // Ajusta el tamaño a las dimensiones de tu ventana

//Configura el contenido del nivel actual, agregando las plataformas/jefes.

void Nivel::cargarNivel(int nivel) {

        nivelactual = nivel;



   /* for (auto plataforma : plataformas) {
        Physics::world.DestroyBody(plataforma);
    }

    //Limpia la coleccion de plataformas despues de destruir el cuerpo fisico.
    plataformas.clear();

    */


    switch (nivel) {
        case 1:
            klosterian = new Klosterian(); // Inicializa Klosterian solo en el primer nivel
            klosterian->Begin();
            addPlatform(100, 1026, 1500); // Agrega una plataforma para el nivel 1
            std::cerr << "Primer mapa" << std::endl;
            break;
        case 2:
            addPlatform(100, 1026, 1500); // Agrega una plataforma para el nivel 2

            if (tieneJefe)
            {
                maliketh.Begin(); // Inicializa el jefe Maliketh si está presente
                tieneJefeMaliketh = true; // Marca que Maliketh está presente
            }
            std::cerr << "Segundo mapa" << std::endl;
            break;
        case 3:
            addPlatform(100, 1026, 1500); // Agrega una plataforma para el nivel 3
            break;
        case 4:
            addPlatform(100, 1026, 1500); // Agrega una plataforma para el nivel 4
            if (tieneJefe) {
                kaldrax.Begin(); // Inicializa el jefe Kaldrax si está presente
                tieneJefeKaldrax = true; // Marca que Kaldrax está presente
            }
            break;
        case 5:
            addPlatform(100, 1026, 1500);

            break;
        case 6:
             addPlatform(100, 1026, 1500);
                if (tieneJefe) {
                zorathar.Begin();
                tieneJefeZorathar = true;
            }
            break;
        case 7:
            addPlatform(100 , 1026 , 1500);
            break;
        case 8:
            addPlatform(100 , 1026 , 1500);
            if(tieneJefe)
            {
                radahn.Begin();
                tienejefeRadahn = true;
            }
            break;

        default:
            std::cout << "Nivel no definido" << std::endl; // Mensaje de error si el nivel no está definido
            break;
    }
}

//Actualiza la logica del nivel (Fisica, HUD, Teletransporte, EstadoJefes, Items).

void Nivel::actualizar(float deltaTime, Jugador& jugador , sf::View& view) {

    //Actualiza la fisica del juego
    Physics::Update(deltaTime);
    //Actualiza el estado del hud.
    hud.update(deltaTime , view , jugador);

    float xMinLimit; // Límite izquierdo para cada nivel

    sf::Vector2f teleportPosition;

    //Evita que el personaje salga del mapa teletransportandolo a una posicion especifica.

    switch (nivelactual) {
        case 1:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 2:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 3:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 4:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 5:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 6:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 7:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        case 8:
            xMinLimit = 10.0f;
            teleportPosition = sf::Vector2f(20.0f, jugador.getPosition().y);
            break;
        default:
            break;
    }

    // Activación de teletransporte si el jugador trata de pasar al límite izquierdo
    if (jugador.getPosition().x <= xMinLimit) {
        jugador.setPosition(teleportPosition); // Teletransporta al jugador al lado derecho del mapa
    }


    //Para cada jefe se actualiza su estado y se ajusta su barra de salud.

   if (tieneJefe && tieneJefeMaliketh) {
        maliketh.update(deltaTime, jugador);
        healthBarJefe.setHealth(maliketh.getHealth());

        if (!maliketh.isAlive() && !jefeDerrotado) {
            // Maliketh ha sido derrotado, dropea el ítem
            jefeDerrotado = true;
            sf::Vector2f itemPos = maliketh.getPosition();  // Asegúrate de que Maliketh tenga getPosition()
            items.push_back(new Item(ItemType::ARMOR, itemPos));  // Crear y almacenar el ítem
        }
    }
    if (tieneJefe && tieneJefeKaldrax) {
        kaldrax.update(deltaTime, jugador);
        healthBarKaldrax.setHealth(kaldrax.getHealth());

        //Drop Item
        if (!kaldrax.isAlive() && !jefeDerrotado) {
            jefeDerrotado = true;
            sf::Vector2f itemPos = {kaldrax.getPosition().x, kaldrax.getPosition().y + 120.0f};
            items.push_back(new Item(ItemType::SWORD, itemPos));
        }
    }
    if (tieneJefe && tieneJefeZorathar) {
        zorathar.update(deltaTime, jugador);
        healthBarZorathar.setHealth(zorathar.getHealth());

        //Drop Item
        if (!zorathar.isAlive() && !jefeDerrotado) {
            jefeDerrotado = true;
            sf::Vector2f itemPos = zorathar.getPosition();
            items.push_back(new Item(ItemType::HELMET, itemPos));
        }
    }

        if (tieneJefe && tienejefeRadahn) {
            radahn.update(deltaTime, jugador);
            healthBarRadahn.setHealth(radahn.getHealth());


            if(!radahn.isAlive() && !jefeDerrotado){
            jefeDerrotado = true;
    }
}
    //Manejo de items recogidos

    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->isRecogido()) {
            // Eliminar el cuerpo del ítem del mundo de física
            Physics::world.DestroyBody((*it)->getBody());

            // Liberar la memoria del ítem y eliminarlo de la lista
            delete *it;
            it = items.erase(it);
        } else {
            ++it;
        }
    }

    if (klosterian) {
        klosterian->update(deltaTime); // Actualiza Klosterian
    }
}

//Renderizado del fondo, jefes y hud.

void Nivel::renderizar(sf::RenderWindow& window , sf::View& view) {


    spriteFondo.setScale(static_cast<float>(window.getSize().x) / fondoTextura.getSize().x,
                          static_cast<float>(window.getSize().y) / fondoTextura.getSize().y);

    window.draw(spriteFondo);

    for (auto plataforma : plataformas) {
        b2Vec2 pos = plataforma->GetPosition();
        sf::RectangleShape rect(sf::Vector2f(30 * 1500, 30 * 10));
        rect.setPosition(pos.x * 30.0f, pos.y * 30.0f);
        rect.setFillColor(sf::Color::Green);
        window.draw(rect);
    }

    hud.render(window , view);


    if (tieneJefe && tieneJefeMaliketh) {

        healthBarJefe.draw(window , view);
        window.draw(maliketh);
    }
    if(tieneJefe && tieneJefeKaldrax){
        healthBarKaldrax.draw(window , view);
        window.draw(kaldrax);

    }

    if(tieneJefe && tieneJefeZorathar){
        healthBarZorathar.draw(window , view);
        window.draw(zorathar);

    }


    if (tieneJefe && tienejefeRadahn) {
       healthBarRadahn.draw(window , view);
       window.draw(radahn);
    }

    if (klosterian) {
        window.draw(*klosterian); // Asegúrate de que Klosterian herede de Drawable
    }
    for (auto item : items) {
        window.draw(*item);
    }


}

//Si el jefe esta derrotado se toma como el nivel completo, accediendo al otro mapa.

bool Nivel::nivelCompleto() const {
    return tieneJefe ? jefeDerrotado : true;
}

//Añade plataformas al nivel.

void Nivel::addPlatform(size_t x, size_t y, size_t width) {


    b2BodyDef definicionPiso;
    definicionPiso.type = b2_staticBody;
    definicionPiso.position.Set(x / 30.0f, y / 30.0f);

    b2Body* body = Physics::world.CreateBody(&definicionPiso);
    b2PolygonShape caja;
    caja.SetAsBox(width / 30.0f, 10);
    b2FixtureDef fixturePiso;
    fixturePiso.shape = &caja;
    fixturePiso.density = 1.0f;
    fixturePiso.friction = 0.5f;
    fixturePiso.userData.pointer = reinterpret_cast<uintptr_t>(&sueloData);

    body->CreateFixture(&fixturePiso);
    plataformas.push_back(body);
}

Nivel::~Nivel() {
    delete klosterian; // Libera la memoria de Klosterian
}


