#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;


class MapaKafelkowa : public sf::Drawable, public sf::Transformable
{
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	unsigned int** level;
	unsigned int szerokosc_mapy_w_kafelkach, wysokosc_mapy_w_kafelkach;
	sf::Vector2u rozmiar_kafelka;

public:
	bool load(const std::string& tileset, sf::Vector2u tile_size, sf::Vector2u rozmiar_mapy);
	void aktualizujPozycje(sf::Vector2i pozycja_gracza, sf::Vector2i punkt_centralny_spritea_gracza, unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu);
	void generuj();
	void zapisz();
	bool wczytaj(string nazwa);

	string sciezka_wczytaj = "";
};
