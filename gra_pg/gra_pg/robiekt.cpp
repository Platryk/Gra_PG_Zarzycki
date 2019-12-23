#include <SFML/Graphics.hpp>
#include <iostream>

#include "robiekty.h"

using namespace std;


PodstawowyObiekt::PodstawowyObiekt(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, string tekstura_sciezka, sf::Vector2f skala)
{
	nazwa = s_nazwa;
	pozycja_na_mapie = sf::Vector2f(u_pozycja_na_mapie.x, u_pozycja_na_mapie.y);

	tekstura.loadFromFile(tekstura_sciezka);
	sprite.setTexture(tekstura);
	sprite.setScale(skala);
}

sf::Vector2i PodstawowyObiekt::podajPunktCentralnySpritea()
{
	int x = sprite.getTexture()->getSize().x * sprite.getScale().x / 2;
	int y = sprite.getTexture()->getSize().y * sprite.getScale().y / 2;
	return sf::Vector2i(x, y);
}

void RuchomyObiekt::aktualizujPolozenie(unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania)
{
	pozycja_na_mapie.x += kierunek.x * predkosc * czas_do_pozycjonowania;
	pozycja_na_mapie.y += kierunek.y * predkosc * czas_do_pozycjonowania;

	//cout << "                  " << kierunek.x * predkosc * czas_do_pozycjonowania << endl;

	sf::Vector2i srodek_spritea(this->podajPunktCentralnySpritea());

	pozycja_na_mapie.x = pozycja_na_mapie.x < 0 ? 0 : pozycja_na_mapie.x;
	pozycja_na_mapie.x = pozycja_na_mapie.x > szerokosc_mapy_w_pxl - 2 * srodek_spritea.x ? szerokosc_mapy_w_pxl - 2 * srodek_spritea.x : pozycja_na_mapie.x;
	pozycja_na_mapie.y = pozycja_na_mapie.y < 0 ? 0 : pozycja_na_mapie.y;
	pozycja_na_mapie.y = pozycja_na_mapie.y > wysokosc_mapy_w_pxl - 2 * srodek_spritea.y ? wysokosc_mapy_w_pxl - 2 * srodek_spritea.y : pozycja_na_mapie.y;

	unsigned int x = pozycja_na_mapie.x;
	unsigned int y = pozycja_na_mapie.y;

	sprite.setPosition(x, y);
}

void RuchomyObiekt::zmienKierunekRuchuWPionie(float y)
{
	kierunek.y = y;
}

void RuchomyObiekt::zmienKierunekRuchuWPoziomie(float x)
{
	kierunek.x = x;
}
