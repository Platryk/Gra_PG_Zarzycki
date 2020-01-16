#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class MapaKafelkowa : public sf::Drawable, public sf::Transformable
{
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture tekstura_kafelkow;
	unsigned int** tablica_mapy;
	sf::Vector2u rozmiar_mapy_w_kafelkach;
	sf::Vector2u rozmiar_kafelka;
	string nazwa_pliku_mapy;
	string sciezka;
	vector <int> pula_generatora = {};


	bool wczytaj();
	void zaladuj();
	void generuj();

public:

	MapaKafelkowa(string s_nazwa_pliku_mapy);
	MapaKafelkowa(string s_nazwa_pliku_mapy, sf::Vector2u u_rozmiar_mapy_w_kafelkach, vector <int> v_pula_generatora);

	void usun();
	void zapisz();

	sf::Vector2u podajRozmiarKafelka() { return rozmiar_kafelka; }
	sf::Vector2u podajRozmiarMapyWKafelkach() { return rozmiar_mapy_w_kafelkach; }

};
