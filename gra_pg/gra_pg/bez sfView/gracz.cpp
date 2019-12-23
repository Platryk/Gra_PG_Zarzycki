#include <SFML/Graphics.hpp>
#include <iostream>

#include "robiekty.h"

using namespace std;


void Gracz::aktualizujPolozenie(unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu, unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania)
{
	RObiekt::aktualizujPolozenie(szerokosc_ekranu, wysokosc_ekranu, szerokosc_mapy_w_pxl, wysokosc_mapy_w_pxl, czas_do_pozycjonowania);

	sf::Vector2i srodek_spritea(this->podajPunktCentralnySpritea());
	sf::Vector2i pozycja_srodka_spritea_na_mapie(pozycja_na_mapie.x + srodek_spritea.x, pozycja_na_mapie.y + srodek_spritea.y);

	unsigned int x = szerokosc_ekranu / 2 - srodek_spritea.x;
	unsigned int y = wysokosc_ekranu / 2 - srodek_spritea.y;

	if (pozycja_srodka_spritea_na_mapie.x < szerokosc_ekranu / 2)
		x = pozycja_na_mapie.x;
	else if (pozycja_srodka_spritea_na_mapie.x > szerokosc_mapy_w_pxl - szerokosc_ekranu / 2)
		x = pozycja_srodka_spritea_na_mapie.x - (szerokosc_mapy_w_pxl - szerokosc_ekranu) - srodek_spritea.x;

	if (pozycja_srodka_spritea_na_mapie.y < wysokosc_ekranu / 2)
		y = pozycja_na_mapie.y;
	else if (pozycja_srodka_spritea_na_mapie.y > wysokosc_mapy_w_pxl - wysokosc_ekranu / 2)
		y = pozycja_srodka_spritea_na_mapie.y - (wysokosc_mapy_w_pxl - wysokosc_ekranu) - srodek_spritea.y;

	sprite.setPosition(x, y);
}
