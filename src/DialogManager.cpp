#include "DialogManager.h"
#include <iostream>

// Constructor del administrador de di�logos
DialogManager::DialogManager() : _currentFrame(0), _isActive(false)
{
    // Cargar la fuente de texto para el di�logo
    if (!_font.loadFromFile("UI/dialogo/fuente.ttf"))
    {
        std::cout << "Error al cargar la fuente." << std::endl;
    }

    // Configuraci�n del texto del di�logo
    _dialogText.setFont(_font);
    _dialogText.setCharacterSize(14); // Tama�o del texto
    _dialogText.setFillColor(sf::Color::White); // Color del texto
    _dialogText.setPosition(50, 200); // Posici�n inicial del di�logo en la pantalla

    // Cargar la imagen de la tecla "E"
    if (!letraEtextura.loadFromFile("resources/textures/letraE.png"))
    {
        std::cout << "Error al cargar el sprite." << std::endl;
    }

    // Configurar el sprite para la imagen de la tecla "E"
    letraEsprite.setTexture(letraEtextura);
    letraEsprite.setOrigin(letraEtextura.getSize().x / 2, letraEtextura.getSize().y / 2); // Centrar el origen
    letraEsprite.setScale(0.05f, 0.05f); // Reducir el tama�o

    // Cargar el sonido del di�logo
    if (!_bufferVoz.loadFromFile("1106.ogg"))
    {
        std::cout << "Error al cargar el sonido." << std::endl;
    }
    _sonidoVoz.setBuffer(_bufferVoz);
}

// Cargar el texto completo del di�logo, dividido en varias frases o "frames"
void DialogManager::loadDialog(const std::vector<std::string>& frames)
{
    _dialogFrames = frames; // Guardar las frases en un vector
}

// Iniciar el di�logo desde el primer "frame"
void DialogManager::startDialog()
{
    _currentFrame = 0; // Iniciar desde el primer frame de texto
    _isActive = true; // Activar el di�logo
    letraVisible = false; // Ocultar la tecla "E" al comenzar
    _dialogText.setString(_dialogFrames[_currentFrame]); // Mostrar el primer texto del di�logo
    _sonidoVoz.play();
}

// Avanzar al siguiente frame de di�logo
void DialogManager::nextFrame()
{
    if (_isActive && _currentFrame < _dialogFrames.size() - 1)
    {
        // Si el di�logo est� activo y no estamos en el �ltimo frame:
        _currentFrame++; // Pasar al siguiente frame
        _dialogText.setString(_dialogFrames[_currentFrame]); // Actualizar el texto mostrado
        _sonidoVoz.play();
    }
    else
    {
        // Si ya estamos en el �ltimo frame, desactivar el di�logo
        _isActive = false;
    }
}

// Dibujar el di�logo en la ventana de juego, centrado sobre el NPC
void DialogManager::draw(sf::RenderWindow& window, const sf::Vector2f& npcPosition)
{
    if (_isActive)   // Solo dibuja el di�logo si est� activo
    {
        // Actualizar el texto a mostrar
        _dialogText.setString(_dialogFrames[_currentFrame]);

        // Calcular la posici�n del texto
        float dialogX = npcPosition.x - _dialogText.getLocalBounds().width / 2 + 60;
        float dialogY = npcPosition.y - 36;

        // Posicionar el texto encima del NPC y dibujarlo en la ventana
        _dialogText.setPosition(dialogX, dialogY);
        window.draw(_dialogText);
    }
}

// Detener el di�logo
void DialogManager::stopDialog()
{
    _isActive = false; // Cambiar el estado a inactivo para ocultar el di�logo
}
