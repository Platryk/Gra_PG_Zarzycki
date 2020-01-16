#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "mapa_kafelkowa.h"

using namespace std;


MapaKafelkowa::MapaKafelkowa(string s_nazwa_pliku_mapy)
{
	nazwa_pliku_mapy = s_nazwa_pliku_mapy;
	sciezka = "mapy/" + nazwa_pliku_mapy + ".txt";

	this->wczytaj();

	this->zaladuj();
}

MapaKafelkowa::MapaKafelkowa(string s_nazwa_pliku_mapy, sf::Vector2u u_rozmiar_mapy_w_kafelkach, vector<int> v_pula_generatora)
{
	nazwa_pliku_mapy = s_nazwa_pliku_mapy;
	sciezka = "mapy/" + nazwa_pliku_mapy + ".txt";

	rozmiar_mapy_w_kafelkach = u_rozmiar_mapy_w_kafelkach;

	pula_generatora = v_pula_generatora;
	if(pula_generatora.size() == 0)
		pula_generatora = { 19, 20, 32, 33, 41 };

	this->generuj();
	this->zaladuj();
}

void MapaKafelkowa::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &tekstura_kafelkow;

	// draw the vertex array
	target.draw(m_vertices, states);
}

bool MapaKafelkowa::wczytaj()
{
	if (nazwa_pliku_mapy == "")
		return false;

	fstream plik;

	plik.open(sciezka, fstream::in);

	if (!plik.good()) {
		cout << "Problem z mapa: " + sciezka << endl;
		return false;
	}

	int liczba;
	string slowo;
	string linia;
	for (int i = 0; i < 4; i++) {

		do {
			getline(plik, linia);
		} while (linia.length() == 0);

		switch (i) {
		case 0:
			plik >> slowo;
			tekstura_kafelkow.loadFromFile("grafiki/" + slowo);
			break;
		case 1:
			plik >> rozmiar_mapy_w_kafelkach.x;
			plik >> rozmiar_mapy_w_kafelkach.y;
			break;
		case 2:
			plik >> rozmiar_kafelka.x;
			plik >> rozmiar_kafelka.y;
			break;
		case 3:
			tablica_mapy = new unsigned int*[rozmiar_mapy_w_kafelkach.y];
			for (int i = 0; i < rozmiar_mapy_w_kafelkach.y; i++)
				tablica_mapy[i] = new unsigned int[rozmiar_mapy_w_kafelkach.x];

			for (int k = 0; k < rozmiar_mapy_w_kafelkach.y; k++) {
				for (int w = 0; w < rozmiar_mapy_w_kafelkach.x; w++) {
					plik >> tablica_mapy[k][w];
				}
			}
			break;
		}
	}

	this->zaladuj();

	plik.close();
	return true;
}

void MapaKafelkowa::zaladuj()
{
	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(rozmiar_mapy_w_kafelkach.x * rozmiar_mapy_w_kafelkach.y * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int k = 0; k < rozmiar_mapy_w_kafelkach.x; ++k)
		for (unsigned int w = 0; w < rozmiar_mapy_w_kafelkach.y; ++w)
		{
			// get the current tile number
			int tileNumber = tablica_mapy[w][k];

			// find its position in the tileset texture
			int tu = tileNumber % (tekstura_kafelkow.getSize().x / rozmiar_kafelka.x);
			int tv = tileNumber / (tekstura_kafelkow.getSize().x / rozmiar_kafelka.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(k + w * rozmiar_mapy_w_kafelkach.y) * 4];

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
}

void MapaKafelkowa::generuj()
{
	tekstura_kafelkow.loadFromFile("grafiki/kafelki.jpg");

	rozmiar_kafelka = sf::Vector2u(48, 48);

	srand(time(NULL));

	tablica_mapy = new unsigned int*[rozmiar_mapy_w_kafelkach.y];
	for (int i = 0; i < rozmiar_mapy_w_kafelkach.y; i++)
		tablica_mapy[i] = new unsigned int[rozmiar_mapy_w_kafelkach.x];

	for (int k = 0; k < rozmiar_mapy_w_kafelkach.y; k++) {
		for (int w = 0; w < rozmiar_mapy_w_kafelkach.x; w++)
			tablica_mapy[k][w] = pula_generatora[rand() % pula_generatora.size()];
	}
}

void MapaKafelkowa::zapisz()
{
	fstream plik;

	plik.open(sciezka, fstream::out | fstream::trunc);

	plik << "#grafika kafelek\n";
	plik << "kafelki.jpg\n\n";
	plik << "#rozmiar mapy w kafelkach[x, y]\n";
	plik << rozmiar_mapy_w_kafelkach.x << ' ' << rozmiar_mapy_w_kafelkach.y << "\n\n";
	plik << "#rozmiar kafelka[x, y]\n";
	plik << rozmiar_kafelka.x << ' ' << rozmiar_kafelka.y << "\n\n";
	plik << "#mapa\n";

	for (int k = 0; k < rozmiar_mapy_w_kafelkach.y; k++) {
		for (int w = 0; w < rozmiar_mapy_w_kafelkach.x; w++) {
			plik << ' ' << tablica_mapy[k][w];
		}
		plik << endl;
	}

	plik.close();
}

void MapaKafelkowa::usun()
{
	fstream plik(nazwa_pliku_mapy);

	remove(sciezka.c_str());
}
