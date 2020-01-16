#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

class PodstawowyObiekt
{
protected:
	string nazwa;
	sf::Vector2f pozycja_na_mapie;
	string sciezka_tekstury_lub_folderu;
	sf::Texture tekstura;
	sf::Sprite sprite;
	unsigned int predkosc = 0; //piksele na sekunde
	unsigned int przyspieszenie = 0; //zmiana predkosci na sekunde
	sf::Vector2f zadany_kierunek_ruchu = sf::Vector2f(0, 0);
	sf::Vector2f rzeczywisty_kierunek_ruchu = sf::Vector2f(0, 0);
	sf::FloatRect prostokat_kolizyjny;

public:
	PodstawowyObiekt(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, string s_sciezka_tekstury_lub_folderu, sf::Vector2f skala);

	string podajNazwe() { return nazwa; }
	sf::Vector2f podajPunktCentralnySpritea(); //odleglosc od lewego gornego rogu do srodka
	sf::Vector2f podajPozycjeNaMapie() { return pozycja_na_mapie; }
	sf::Vector2f podajPozycjeSprita() { return sprite.getPosition(); }
	sf::Sprite podajSprite() { return sprite; }
	sf::Vector2f podajRzeczywistyKierunekRuchu() { return rzeczywisty_kierunek_ruchu; }
	unsigned int podajPredkosc() { return predkosc; }
	unsigned int podajPrzyspieszenie() { return przyspieszenie; }
	string podajSciezkeTeksturyLubFolderu() { return sciezka_tekstury_lub_folderu; }

	void ustawPozycjeNaMapie(sf::Vector2f f_poz);
	sf::FloatRect podajProstokatKolizyjny(); //Z uwzglednieniem wspolrzednych na mapie

	virtual void aktualizujPolozenie(unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania) {}
	virtual void zmienKierunekRuchuWPionie(float y) {}
	virtual void zmienKierunekRuchuWPoziomie(float x) {}
	virtual void zmienRzeczywistyKierunekRuchuWPionie(float y) {}
	virtual void zmienRzeczywistyKierunekRuchuWPoziomie(float x) {}
};

class RuchomyObiekt : public PodstawowyObiekt
{
protected:
	string poprzednia_czynnosc = "stoj_dol";
	string aktualna_czynnosc = "stoj_dol";
	unsigned int nr_grafiki_animacji = 1;
	sf::Clock zegar_animacji;

public:
	RuchomyObiekt(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, unsigned int u_predkosc, unsigned int u_przyspieszenie, string s_sciezka_tekstury_lub_folderu, sf::Vector2f skala)
		: PodstawowyObiekt::PodstawowyObiekt(s_nazwa, u_pozycja_na_mapie, s_sciezka_tekstury_lub_folderu, skala)
	{
		predkosc = u_predkosc;
		przyspieszenie = u_przyspieszenie;
	}

	virtual void aktualizujPolozenie(unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl, float czas_do_pozycjonowania) override;
	virtual void zmienKierunekRuchuWPionie(float y) override;
	virtual void zmienKierunekRuchuWPoziomie(float x) override;
	virtual void zmienRzeczywistyKierunekRuchuWPionie(float y) override;
	virtual void zmienRzeczywistyKierunekRuchuWPoziomie(float x) override;
	void aktualizujRzeczywistyKierunekRuchu(float czas_do_pozycjonowania);

	void animujPoruszanie();
};

class Gracz : public RuchomyObiekt
{
public:
	Gracz(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, unsigned int u_predkosc, unsigned int u_przyspieszenie, string s_sciezka_tekstury_lub_folderu, sf::Vector2f skala)
		: RuchomyObiekt::RuchomyObiekt(s_nazwa, u_pozycja_na_mapie, u_predkosc, u_przyspieszenie, s_sciezka_tekstury_lub_folderu, skala)
	{}
};

class Krolik : public RuchomyObiekt
{
public:
	Krolik(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, unsigned int u_predkosc, unsigned int u_przyspieszenie, string s_sciezka_tekstury_lub_folderu, sf::Vector2f skala)
		: RuchomyObiekt::RuchomyObiekt(s_nazwa, u_pozycja_na_mapie, u_predkosc, u_przyspieszenie, s_sciezka_tekstury_lub_folderu, skala)
	{}
};

class Drzewo : public PodstawowyObiekt
{
public:
	Drzewo(string s_nazwa, sf::Vector2f u_pozycja_na_mapie, string tekstura_sciezka, sf::Vector2f skala)
		: PodstawowyObiekt::PodstawowyObiekt(s_nazwa, u_pozycja_na_mapie, tekstura_sciezka, skala)
	{
		sf::Vector2f rozmiar_spritea = sf::Vector2f(sprite.getTexture()->getSize().x * sprite.getScale().x, sprite.getTexture()->getSize().y * sprite.getScale().y);

		prostokat_kolizyjny = sf::FloatRect((rozmiar_spritea.x / 12) * 5, rozmiar_spritea.y - 70, rozmiar_spritea.x / 6, 15);
	}
};