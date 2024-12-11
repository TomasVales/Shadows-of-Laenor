// DialogManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class DialogManager {
private:
    std::vector<std::string> _dialogFrames; // Almacena cada "página" del diálogo
    int _currentFrame;
    bool _isActive;
    sf::Font _font;
    sf::Text _dialogText;
    bool dialogActive; // Ejemplo de una variable para controlar si el diálogo está act
    sf::Sound _sonidoVoz;
    sf::SoundBuffer _bufferVoz;


public:
    DialogManager();
     sf::Sprite letraEsprite;
    bool letraVisible = false;
    sf::Texture letraEtextura;
    void loadDialog(const std::vector<std::string>& frames);
    void startDialog();
    void nextFrame();
    bool isActive() const { return _isActive; }
    void draw(sf::RenderWindow& window, const sf::Vector2f& npcPosition) ;
      void stopDialog();
};
