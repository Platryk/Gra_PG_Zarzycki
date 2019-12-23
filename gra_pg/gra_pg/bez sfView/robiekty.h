#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;


class RObiekt
{
protected:
	string nazwa;
	sf::Vector2f pozycja_na_mapie;
	unsigned int predkosc = 0; //piksele na sekunde
	sf::Vector2f kierunek;
	sf::Texture tekstura;
	sf::Sprite sprite;

public:
	RObiekt(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, unsigned int u_predkosc, string tekstura_sciezka, sf::Vector2f skala);

	string podajNazwe() { return nazwa; }
	sf::Vector2i podajPunktCentralnySpritea(); //odleglosc od lewego gornego rogu do srodka
	sf::Vector2f podajPozycjeNaMapie() { return pozycja_na_mapie; }
	sf::Vector2f podajPozycjeSprita() { return sprite.getPosition(); }
	sf::Sprite podajSprite() { return sprite; }
	virtual void aktualizujPolozenie(unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu, unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania);
	void zmienKierunekRuchuWPionie(float y);
	void zmienKierunekRuchuWPoziomie(float x);
};

class Gracz : public RObiekt
{
public:
	Gracz(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, unsigned int u_predkosc, string tekstura_sciezka, sf::Vector2f skala)
		: RObiekt::RObiekt(s_nazwa, u_pozycja_na_mapie, u_predkosc, tekstura_sciezka, skala)
	{}

	virtual void aktualizujPolozenie(unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu, unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania);
};

class Drzewo : public RObiekt
{
public:
	Drzewo(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, string tekstura_sciezka, sf::Vector2f skala)
		: RObiekt::RObiekt(s_nazwa, u_pozycja_na_mapie, 0, tekstura_sciezka, skala)
	{}
};