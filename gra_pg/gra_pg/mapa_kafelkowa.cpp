#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "mapa_kafelkowa.h"

using namespace std;


void MapaKafelkowa::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

bool MapaKafelkowa::load(const std::string & tileset, sf::Vector2u tile_size, sf::Vector2u rozmiar_mapy)
{
	szerokosc_mapy_w_kafelkach = rozmiar_mapy.x;
	wysokosc_mapy_w_kafelkach = rozmiar_mapy.y;
	rozmiar_kafelka = tile_size;

	level = new unsigned int*[wysokosc_mapy_w_kafelkach];
	for (int i = 0; i < wysokosc_mapy_w_kafelkach; i++)
		level[i] = new unsigned int[szerokosc_mapy_w_kafelkach];

	if (!this->wczytaj(sciezka_wczytaj)) {
		this->generuj();
		this->zapisz();
	}

	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(szerokosc_mapy_w_kafelkach * wysokosc_mapy_w_kafelkach * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int k = 0; k < szerokosc_mapy_w_kafelkach; ++k)
		for (unsigned int w = 0; w < wysokosc_mapy_w_kafelkach; ++w)
		{
			// get the current tile number
			int tileNumber = level[w][k];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / rozmiar_kafelka.x);
			int tv = tileNumber / (m_tileset.getSize().x / rozmiar_kafelka.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(k + w * szerokosc_mapy_w_kafelkach) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(k * rozmiar_kafelka.x, w * rozmiar_kafelka.y);
			quad[1].position = sf::Vector2f((k + 1) * rozmiar_kafelka.x, w * rozmiar_kafelka.y);
			quad[2].position = sf::Vector2f((k + 1) * rozmiar_kafelka.x, (w + 1) * rozmiar_kafelka.y);
			quad[3].position = sf::Vector2f(k * rozmiar_kafelka.x, (w + 1) * rozmiar_kafelka.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * rozmiar_kafelka.x, tv * rozmiar_kafelka.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * rozmiar_kafelka.x, tv * rozmiar_kafelka.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * rozmiar_kafelka.x, (tv + 1) * rozmiar_kafelka.y);
			quad[3].texCoords = sf::Vector2f(tu * rozmiar_kafelka.x, (tv + 1) * rozmiar_kafelka.y);
		}

	return true;
}

void MapaKafelkowa::generuj()
{
	srand(time(NULL));

	int pula[] = { 4,15,18 };

	for (int k = 0; k < wysokosc_mapy_w_kafelkach; k++) {
		for (int w = 0; w < szerokosc_mapy_w_kafelkach; w++)
			level[k][w] = pula[rand() % (sizeof(pula) / sizeof(*pula))];
	}
}

void MapaKafelkowa::zapisz()
{
	fstream plik;
	string sciezka;

	int i = 1;
	do {
		plik.close();
		sciezka = "zapisy/zapis" + to_string(i) + ".txt";

		plik.open(sciezka);

		i++;
	} while (plik && (i < 5));

	plik.open(sciezka, fstream::out | fstream::in | fstream::trunc);

	for (int k = 0; k < wysokosc_mapy_w_kafelkach; k++) {
		for (int w = 0; w < szerokosc_mapy_w_kafelkach; w++) {
			plik << ' ' << level[k][w];
			cout << ' ' << level[k][w];
		}
		cout << endl;
		plik << endl;
	}

	plik.close();
}

bool MapaKafelkowa::wczytaj(string nazwa)
{
	if (nazwa == "")
		return false;

	fstream plik;
	string sciezka;

	sciezka = "zapisy/" + nazwa + ".txt";

	plik.open(sciezka, fstream::in);
	if (!plik.good())
		return false;

	for (int k = 0; k < wysokosc_mapy_w_kafelkach; k++) {
		for (int w = 0; w < szerokosc_mapy_w_kafelkach; w++) {
			plik >> level[k][w];
			cout << ' ' << level[k][w];
		}
		cout << endl;
	}

	plik.close();
	return true;
}
