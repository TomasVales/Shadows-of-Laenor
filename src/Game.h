#pragma once
#include <SFML/Graphics.hpp>


extern bool paused; // le declaramos paused
void Begin(const sf::Window& window);
void Update(float deltaTime, sf::RenderWindow& window);
void Render(sf::RenderWindow& window , float deltaTime);
void guardarProgreso(int nivelActualIndex);
int cargarProgreso();
void cargarNuevoNivel(int nuevoNivelIndex, const std::string& nombreMapa, bool tieneJefe);
bool estaDentroDeRango(const sf::Vector2f& posicionJugador, const sf::Vector2f& posicionObjetivo, float rangeX, float rangeY);

