#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "mapa_kafelkowa.h"
#include "obiekty.h"


typedef vector < PodstawowyObiekt* > VObiektyT;


using namespace std;


class Gra
{
private:

	//deklaracja zmiennych
	sf::Clock GLOWNY_ZEGAR;
	sf::Clock ZEGAR_OD_ROZPOCZECIA_GRY;
	sf::Clock ZEGAR_DO_OBLICZANIA_POZYCJI;
	sf::Clock ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS;
	float czas_do_pozycjonowania_w_sekundach;

	unsigned int SZEROKOSC_OKNA;
	unsigned int WYSOKOSC_OKNA;

	unsigned int SZEROKOSC_EKRANU_GRY;
	unsigned int WYSOKOSC_EKRANU_GRY;

	unsigned int SZEROKOSC_MAPY_W_PXL;
	unsigned int WYSOKOSC_MAPY_W_PXL;

	vector <unsigned int> kolejnosc_wyswietlania_obiektow_z_vObiekty;

	sf::Font courier_prime;
	sf::Text wskaznik_FPS;

	sf::RenderWindow* okno_aplikacji;

	MapaKafelkowa* mapa_gry;

	sf::RenderTexture ekran_gry;
	sf::View widok_ekranu_gry;

	VObiektyT vObiekty;
	unsigned int nr_gracza_w_vObiekty = 0;

	sf::Event zdarzenie;
	
	bool pierwsze_sortowanie = true;
	bool pauza = false;
	bool* zamknij_aplikacje;
	bool* czy_wyswietlic_pomoc;
	sf::Text pomoc;

	string sciezka_do_zapisu = "";
	string nazwa_mapy = "";

	bool wczytajZapis(string zapis);
	void nowaGra(sf::Vector2u rozmiar_nowej_mapy, vector <int> v_pula_generatora);
	void glownaPetla();
	void pojedynczaKlatka();

	sf::Vector2i v2ftoi(sf::Vector2f v2f);
	sf::Vector2f pozycjaWidokuGlownego(sf::Vector2f, sf::Vector2f, sf::Vector2u, sf::Vector2u);
	void wykrywanieKolizji();


	Gra(sf::RenderWindow* okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc);

public:

	Gra(sf::RenderWindow* okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc, string zapis);
	Gra(sf::RenderWindow* okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc, sf::Vector2u rozmiar_nowej_mapy, vector <int> v_pula_generatora);

	void start();
	bool zapiszGre();

	~Gra() {}
};


