#pragma once
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "gra.h"

using namespace std;


class Menu
{
private:

	bool zamknij_aplikacje = false;

	sf::Font fruktur;
	sf::Font montserrat;
	sf::Text* opcje_wyboru;
	sf::Text tytul;
	sf::Text pomoc;
	string* akcje_opcji_wyboru;
	int podswietlona_opcja = 0;
	int liczba_opcji_wyboru = 0;
	bool czy_wyswietlic_pomoc = false;

	sf::Color kolor_opcji_niewybranej;
	sf::Color kolor_opcji_wybranej;
	unsigned int rozmiar_czcionki;
	float separacja_opcji_w_pionie;
	float grubosc_obrysu_tekstu;

	sf::RenderWindow* okno_aplikacji;

	sf::RenderTexture ekran_menu;
	sf::View widok_ekranu_menu;
	sf::Vector2f pozycja_widoku_tla_menu;
	sf::Vector2f kierunek_przesuwania_tla_menu;
	unsigned int predkosc_przesuwania_tla_menu;

	unsigned int SZEROKOSC_OKNA;
	unsigned int WYSOKOSC_OKNA;

	unsigned int SZEROKOSC_EKRANU_MENU;
	unsigned int WYSOKOSC_EKRANU_MENU;

	unsigned int SZEROKOSC_MAPY_W_PXL;
	unsigned int WYSOKOSC_MAPY_W_PXL;

	sf::Event zdarzenie;

	MapaKafelkowa* mapa_tla_menu;

	Gra* gra;

	void glownaPetla();
	void opcjaWGore();
	void opcjaWDol();
	void wyswietlMenu();
	void aktualizujMenu();
	void aktualizujWidokTlaMenu();

	void wybrano();
	void menuGlowne();
	void nowaGra();
	void wczytajGre();
	void pauzaWGrze();
	void czyNaPewnoPrzerwacRozgrywke();
	void czyNaPewnoWyjscZGry();
	void menuZapisano();
	void menuNieZapisano();

public:

	Menu(sf::RenderWindow* okno_aplikacji_in);

	~Menu();
};
