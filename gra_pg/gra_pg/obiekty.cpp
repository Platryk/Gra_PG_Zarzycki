#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "obiekty.h"

using namespace std;


PodstawowyObiekt::PodstawowyObiekt(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, string s_sciezka_tekstury_lub_folderu, sf::Vector2f skala)
{
	nazwa = s_nazwa;
	pozycja_na_mapie = sf::Vector2f(u_pozycja_na_mapie.x, u_pozycja_na_mapie.y);

	sciezka_tekstury_lub_folderu = s_sciezka_tekstury_lub_folderu;
	if (s_sciezka_tekstury_lub_folderu[s_sciezka_tekstury_lub_folderu.length() - 1] != '/')
		tekstura.loadFromFile(s_sciezka_tekstury_lub_folderu);
	else {
		tekstura.loadFromFile(s_sciezka_tekstury_lub_folderu + "domyslny.png");
	}

	sprite.setTexture(tekstura);
	sprite.setScale(skala);
	sprite.setPosition(pozycja_na_mapie);

	prostokat_kolizyjny = sprite.getGlobalBounds();
	prostokat_kolizyjny.left = 0;
	prostokat_kolizyjny.top = 0;
}

sf::Vector2f PodstawowyObiekt::podajPunktCentralnySpritea()
{
	float x = sprite.getTexture()->getSize().x * sprite.getScale().x / 2;
	float y = sprite.getTexture()->getSize().y * sprite.getScale().y / 2;
	return sf::Vector2f(x, y);
}

void PodstawowyObiekt::ustawPozycjeNaMapie(sf::Vector2f f_poz)
{
	pozycja_na_mapie = f_poz;
}

sf::FloatRect PodstawowyObiekt::podajProstokatKolizyjny()
{
	sf::FloatRect prost_kol = sprite.getGlobalBounds();
	prost_kol.left += prostokat_kolizyjny.left;
	prost_kol.top += prostokat_kolizyjny.top;
	prost_kol.width = prostokat_kolizyjny.width;
	prost_kol.height = prostokat_kolizyjny.height;

	return prost_kol;
}

void RuchomyObiekt::aktualizujPolozenie(unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania)
{
	aktualizujRzeczywistyKierunekRuchu(czas_do_pozycjonowania);

	pozycja_na_mapie.x += rzeczywisty_kierunek_ruchu.x * predkosc * czas_do_pozycjonowania;
	pozycja_na_mapie.y += rzeczywisty_kierunek_ruchu.y * predkosc * czas_do_pozycjonowania;

	//cout << "                  " << zadany_kierunek_ruchu.x * predkosc * czas_do_pozycjonowania << endl;

	unsigned int x = pozycja_na_mapie.x;
	unsigned int y = pozycja_na_mapie.y;

	sprite.setPosition(x, y);

	animujPoruszanie();
}

void RuchomyObiekt::zmienKierunekRuchuWPionie(float y)
{
	zadany_kierunek_ruchu.y = y;
}

void RuchomyObiekt::zmienKierunekRuchuWPoziomie(float x)
{
	zadany_kierunek_ruchu.x = x;
}

void RuchomyObiekt::zmienRzeczywistyKierunekRuchuWPionie(float y)
{
	rzeczywisty_kierunek_ruchu.y = y;
}

void RuchomyObiekt::zmienRzeczywistyKierunekRuchuWPoziomie(float x)
{
	rzeczywisty_kierunek_ruchu.x = x;
}

void RuchomyObiekt::aktualizujRzeczywistyKierunekRuchu(float czas_do_pozycjonowania)
{
	if (rzeczywisty_kierunek_ruchu.x - zadany_kierunek_ruchu.x < 0) {
		rzeczywisty_kierunek_ruchu.x += przyspieszenie * czas_do_pozycjonowania;

		if (rzeczywisty_kierunek_ruchu.x > zadany_kierunek_ruchu.x)
			rzeczywisty_kierunek_ruchu.x = zadany_kierunek_ruchu.x;
	}
	else if (rzeczywisty_kierunek_ruchu.x - zadany_kierunek_ruchu.x > 0) {
		rzeczywisty_kierunek_ruchu.x -= przyspieszenie * czas_do_pozycjonowania;

		if (rzeczywisty_kierunek_ruchu.x < zadany_kierunek_ruchu.x)
			rzeczywisty_kierunek_ruchu.x = zadany_kierunek_ruchu.x;
	}

	if (rzeczywisty_kierunek_ruchu.y - zadany_kierunek_ruchu.y < 0) {
		rzeczywisty_kierunek_ruchu.y += przyspieszenie * czas_do_pozycjonowania;

		if (rzeczywisty_kierunek_ruchu.y > zadany_kierunek_ruchu.y)
			rzeczywisty_kierunek_ruchu.y = zadany_kierunek_ruchu.y;
	}
	else if (rzeczywisty_kierunek_ruchu.y - zadany_kierunek_ruchu.y > 0) {
		rzeczywisty_kierunek_ruchu.y -= przyspieszenie * czas_do_pozycjonowania;

		if (rzeczywisty_kierunek_ruchu.y < zadany_kierunek_ruchu.y)
			rzeczywisty_kierunek_ruchu.y = zadany_kierunek_ruchu.y;
	}

	if (abs(rzeczywisty_kierunek_ruchu.x) + abs(rzeczywisty_kierunek_ruchu.y) > sqrt(2)) {
		rzeczywisty_kierunek_ruchu.x *= sqrt(2) / (abs(rzeczywisty_kierunek_ruchu.x) + abs(rzeczywisty_kierunek_ruchu.y));
		rzeczywisty_kierunek_ruchu.y = rzeczywisty_kierunek_ruchu.y < 0 ? (sqrt(2) - abs(rzeczywisty_kierunek_ruchu.x)) * (-1) : sqrt(2) - abs(rzeczywisty_kierunek_ruchu.x);
	}
}

void RuchomyObiekt::animujPoruszanie()
{
	if (zadany_kierunek_ruchu.x > 0)
		aktualna_czynnosc = "bieg_prawo";
	else if (zadany_kierunek_ruchu.x < 0)
		aktualna_czynnosc = "bieg_lewo";

	if (zadany_kierunek_ruchu.y > 0)
		aktualna_czynnosc = "bieg_dol";
	else if (zadany_kierunek_ruchu.y < 0)
		aktualna_czynnosc = "bieg_gora";

	if (rzeczywisty_kierunek_ruchu.x == 0 && rzeczywisty_kierunek_ruchu.y == 0) {
		if (poprzednia_czynnosc == "bieg_prawo")
			aktualna_czynnosc = "stoj_prawo";
		else if (poprzednia_czynnosc == "bieg_lewo")
			aktualna_czynnosc = "stoj_lewo";
		else if (poprzednia_czynnosc == "bieg_dol")
			aktualna_czynnosc = "stoj_dol";
		else if (poprzednia_czynnosc == "bieg_gora")
			aktualna_czynnosc = "stoj_gora";
		else
			aktualna_czynnosc = poprzednia_czynnosc;
	}

	if (aktualna_czynnosc[0] == 'b') {
		bool test = false;
		unsigned int nr_grafiki = 1;
		if (poprzednia_czynnosc[0] == 's') {
			nr_grafiki_animacji = 1;
			zegar_animacji.restart();

			nr_grafiki = nr_grafiki_animacji;
			test = true;
		}
		else if (zegar_animacji.getElapsedTime().asSeconds() >= 25.f / (predkosc * (sqrt(powf(rzeczywisty_kierunek_ruchu.x, 2) + powf(rzeczywisty_kierunek_ruchu.y, 2))))) {
			nr_grafiki_animacji++;
			zegar_animacji.restart();

			if (nr_grafiki_animacji > 4)
				nr_grafiki_animacji = 1;

			nr_grafiki = nr_grafiki_animacji;

			if (nr_grafiki_animacji == 4)
				nr_grafiki = 2;

			test = true;
		}

		if (test) {
			tekstura.loadFromFile(sciezka_tekstury_lub_folderu + aktualna_czynnosc + '_' + (char)(nr_grafiki + 48) + ".png");
			sprite.setTexture(tekstura);
		}
	}
	else if (poprzednia_czynnosc[0] == 'b') {
		tekstura.loadFromFile(sciezka_tekstury_lub_folderu + aktualna_czynnosc + ".png");
		sprite.setTexture(tekstura);
	}

	poprzednia_czynnosc = aktualna_czynnosc;
}
