#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp> // Incluir la biblioteca grafica de SFML
#include <SFML/Audio.hpp>    // Incluir la biblioteca de audio de SFML
#include <vector>            // Incluir la biblioteca para usar vectores
#include <string>            // Incluir la biblioteca para usar cadenas

// Definicion de la clase Menu
class Menu {
public:
    bool isPlaySubmenuActive = false; // Indica si el submenu"JUGAR" esta activo

    int selectedOptionIndexPlaySubmenu = 0; // Opcion seleccionada en el submenu "JUGAR"
    // Constructor que inicializa el menu con el ancho y alto especificados
    Menu(float width, float height);
    // Destructor de la clase Menu
    ~Menu();
    void toggleMusic();  // Funcion para cambiar el estado de la musica
    bool getIsMusicOn() const;  // Obtener el estado actual de la musica
    // M�todo para dibujar el men� en la ventana
    void draw(sf::RenderWindow &window);
    // M�todo para mover la selecci�n hacia arriba
    void moveUp();
    // M�todo para mover la selecci�n hacia abajo
    void moveDown();
    // M�todo para obtener la opci�n seleccionada en el men� principal
    int getSelectedOption() const;
    // M�todo para obtener el �ndice de la opci�n seleccionada en la pantalla de opciones
    int getSelectedOptionIndexOptions() const;
    // M�todo para actualizar las opciones del men� con un nuevo conjunto de opciones
    void updateMenuOptions(const std::vector<std::string>& newOptions);
    // M�todo para actualizar la animaci�n del t�tulo basado en el tiempo transcurrido
    void updateTitleAnimation(sf::Time elapsedTime);
    // M�todo para dibujar las opciones en la ventana
    void drawOptions(sf::RenderWindow &window);
    // M�todo para activar la pantalla de opciones
    void activateOptionsScreen();
    // M�todo para desactivar la pantalla de opciones
    void deactivateOptionsScreen();
    // M�todo para manejar la selecci�n de opciones
    void handleOptionSelection();
    // M�todo para actualizar el estado de la m�sica (encendida/apagada)
    void updateMusicStatus();
    // M�todo para alternar el estado de la m�sica

    // M�todo para verificar si se est� en la pantalla de opciones
    bool getIsOptionsScreen() const;
    // M�todo para disminuir el volumen de la m�sica
    void decreaseVolume();
    // M�todo para aumentar el volumen de la m�sica
    void increaseVolume();
    // M�todos para acceder y modificar el volumen
    float getVolume() const; // Obtener el volumen actual
    void setVolume(float volume); // Establecer un nuevo volumen
    bool getIsCreditsPlaying() const { return isCreditsPlaying; }

      void stopMusic() {
        backgroundMusic.stop();  // Detiene la m�sica
    }
    // Sonido para la m�sica de fondo
    sf::Sound sound;
    // Texto que muestra el volumen de la m�sica
    sf::Text volumeText;

    //CREDITOS
   // void drawPlaySubmenu(sf::RenderWindow &window);
    void detenerMusica(); // M�todo para detener la m�s
    void startCredits();
    void stopCredits();
    void drawCredits(sf::RenderWindow &window);
    void loadCreditFrames();
    void handleInput(sf::Event &event, bool &isMenuActive, sf::RenderWindow &window);
    void moveUpPlaySubmenu();
    void moveDownPlaySubmenu();
    void handlePlaySubmenuSelection();
    bool isMenuActive;  // Nueva variable miembro
    bool getIsMenuActive() const { return isMenuActive; }
    bool getStartGameOptionSelected() const; // Declaraci�n del m�tod


private:
     int selectedItemIndex;
    bool startGameSelected = false;  // Variable que indica si "Continuar partida" fue seleccionado

       sf::Music backgroundMusic;
       std::vector<sf::Text> playSubmenuOptions;
    // Vector que almacena las opciones del men�
    std::vector<sf::Text> menuOptions;
    // Vector que almacena los frames de textura para la animaci�n del t�tulo
    std::vector<sf::Texture> titleFrames;
    // Sprite para el t�tulo del men�
    sf::Sprite titleSprite;
    // Sprite para el fondo del men�
    sf::Sprite backgroundSprite;
    // Fuente utilizada para el texto
    sf::Font font;
    // Buffer de sonido para la m�sica de fondo
    sf::SoundBuffer buffer;
    // Buffer de sonido para el movimiento entre opciones
    sf::SoundBuffer moveBuffer;
    // Sonido para el movimiento entre opciones
    sf::Sound moveSound;
    // Textura para el fondo del men�
    sf::Texture backgroundTexture;
    bool isMusicOn;  // Variable para controlar el estado de la m�sica
    // �ndice de la opci�n seleccionada en el men� principal
    int selectedOptionIndex;
    // �ndice de la opci�n seleccionada en la pantalla de opciones
    int selectedOptionIndexOptions;
    // Indicador de si se est� en la pantalla de opciones
    bool isOptionsScreen = false;
    // Indicador de si la m�sica est� sonando
    bool isMusicPlaying = true;


    // Texto que muestra el estado de la m�sica (encendida/apagada)
    sf::Text musicStatusText;
    // Texto que muestra el idioma actual
    sf::Text languageText;

    // Tiempo total transcurrido
    sf::Time totalElapsedTime;
    // Duraci�n de cada frame en la animaci�n
    sf::Time frameDuration;
    // Contador del frame actual de la animaci�n
    int currentFrame;

    // Otros miembros de la clase

    //CLASES
    void drawPlaySubmenu(sf::RenderWindow &window);  // Declaraci�n de la funci�n para dibujar el submen� "JUGAR"
    std::vector<sf::Texture> creditFrames;
    sf::Sprite creditsSprite;
    sf::Music creditsMusic;
    bool isCreditsPlaying = false;



};

#endif // MENU_H
