
/*------------------------------------------------------------

Temat projektu: Green World - symulacja œwiata 2D.

Zakres projektu:
1. Mapa generowana losowo.
2. Mo¿liwoœæ tworzenia i zapisywnia wielu œwiatów.
3. Gracz porusza siê w 2-wymiarowej przestrzeni.
4. Sterowanie - WSAD 8 kierunków.
5. Zwierzêta, roœliny i gracz mog¹ wchodziæ w proste interakcje.
6. Celem gry jest przetrwanie i eksploracja œwiata oraz poznanie jego mechanik.

Zrealizowane:
1. Opisanie g³ównych za³o¿eñ i zakresu projektu.
2. Instalacja i konfiguracja biblioteki SFML w Visual Studio 2017.
3. Stworzenie podstawowych elementów szkieletu gry.
4. Wstêpne oprogamowanie systemu sterowania postaci¹.
5. Zapoznanie siê z technikami manipulacji obiektami na ekranie.
6. Rozbudowa i polepszenie systemu sterowania.
7. Stworzenie systemu zapisu map (plik zawieraj¹cy dane mapy).
8. Stworzenie systemu wczytywania map.
9. Zmiana punktu odniesienia (z mapy na postaæ) w wyœwietlaniu mapy.
10. Zapoznanie z systemem dziedziczenia.
11. Wykorzystanie systemu dziedziczenia w programie.

Co zrealizowalem w biezacym tygodniu?
1. Podzia³ kodu na pliki zgodnie z podejœciem obiektowym.
2. Postêp w pracy nad systemem wyswietlania.

Co planuje na kolejny tydzien?
1. Dalsze prace nad systemem sterowania, generowania map.
2. Dodanie jakiegoœ zwierzêcia/innych obiektów.
3. Dodanie animacji ruchu postaci.


------------------------------------------------------------*/

/*------------------------------------------------------------

Program glowny

------------------------------------------------------------*/


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>

#include "mapa_kafelkowa.h"
#include "robiekty.h"

using namespace std;


sf::Vector2i v2ftoi(sf::Vector2f v2f) {
	return sf::Vector2i((int)v2f.x, (int)v2f.y);
}

void pozycjonowanieEkranu(sf::Sprite& sprite_ekranu, unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu, 
	sf::Vector2i punkt_centralny_spritea_gracza, sf::Vector2i pozycja_gracza_na_mapie, sf::Vector2u rozmiar_kafelka, sf::Vector2u rozmiar_mapy_w_kafelkach);


int main()
{
	//deklaracja zmiennych
	sf::Clock GLOWNY_ZEGAR;
	sf::Clock ZEGAR_OD_ROZPOCZECIA_GRY;
	sf::Clock ZEGAR_DO_OBLICZANIA_POZYCJI;
	float czas_do_pozycjonowania_w_sekundach;

	unsigned int SZEROKOSC_OKNA = 1024;
	unsigned int WYSOKOSC_OKNA = 800;

	unsigned int SZEROKOSC_EKRANU = 0;
	unsigned int WYSOKOSC_EKRANU = 0;

	unsigned int SZEROKOSC_MAPY_W_PXL = 0;
	unsigned int WYSOKOSC_MAPY_W_PXL = 0;

	sf::RenderWindow oknoAplikacji(sf::VideoMode(SZEROKOSC_OKNA, WYSOKOSC_OKNA, 32), "Gra PG - Green World");

	MapaKafelkowa mapa;
	mapa.sciezka_wczytaj = "zapis1";
	sf::Vector2u rozmiar_kafelka(48, 48);
	sf::Vector2u rozmiar_mapy_w_kafelkach(40, 40);
	if (!mapa.load("grafiki/kafelki.jpg", rozmiar_kafelka, rozmiar_mapy_w_kafelkach))
		return -1;

	SZEROKOSC_EKRANU = SZEROKOSC_OKNA + 2 * rozmiar_kafelka.x;
	WYSOKOSC_EKRANU = WYSOKOSC_OKNA + 2 * rozmiar_kafelka.y;

	sf::RenderTexture ekran;
	ekran.create(SZEROKOSC_EKRANU, WYSOKOSC_EKRANU);

	SZEROKOSC_MAPY_W_PXL = rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x;
	WYSOKOSC_MAPY_W_PXL = rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y;

	typedef vector < RObiekt* > VObiektyT;
	VObiektyT vObiekty;

	vObiekty.push_back(new Gracz("gracz", sf::Vector2i(20, 20), 200, "grafiki/postac1.png", sf::Vector2f(0.3, 0.3)));
	vObiekty.push_back(new Drzewo("drzewo1", sf::Vector2i(500, 500), "grafiki/drzewo1.png", sf::Vector2f(0.4, 0.4)));

	unsigned int nr_gracza_w_vObiekty = 0;;

	//restart zegarow
	ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
	GLOWNY_ZEGAR.restart();
	ZEGAR_OD_ROZPOCZECIA_GRY.restart();

	//g³ówna pêtla gry
	while (oknoAplikacji.isOpen()) 
	{
		//restart glownego zegara
		GLOWNY_ZEGAR.restart();

		//obs³uga zdarzeñ
		sf::Event zdarzenie;
		while (oknoAplikacji.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type) {
			case sf::Event::Closed:
				oknoAplikacji.close();
				break;

			case sf::Event::KeyReleased:
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
						vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPionie(0);
					else
						vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPionie(-1);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPionie(1);
				else
					vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPionie(0);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
						vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPoziomie(0);
					else
						vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPoziomie(-1);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPoziomie(1);
				else
					vObiekty[nr_gracza_w_vObiekty]->zmienKierunekRuchuWPoziomie(0);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					oknoAplikacji.close();

				break;
			}
		}


		//wyszukanie obiektu o nazwie "gracz" i zapisanie jego pozycji w wektorze do zmiennej
		for (int i = 0; i < vObiekty.size(); i++) {
			if (vObiekty[i]->podajNazwe() == "gracz") {
				nr_gracza_w_vObiekty = i;
				//cout << "                  |" << gracz_nr_w_vObiekty << endl;
				break;
			}
		}

		ekran.clear(sf::Color(0,100,50));

		//aktualizacja pozycji obiektow z vektora vObiekty
		czas_do_pozycjonowania_w_sekundach = ZEGAR_DO_OBLICZANIA_POZYCJI.getElapsedTime().asSeconds();

		for (int i = 0; i < vObiekty.size(); i++) {
			vObiekty[i]->aktualizujPolozenie(SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL, czas_do_pozycjonowania_w_sekundach);
		}

		ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
		
		//okreslenie fragmentu mapy do wyswietlenia w oparciu o pozycje gracza
		sf::Vector2i punkt_centralny_spritea_gracza = vObiekty[nr_gracza_w_vObiekty]->podajPunktCentralnySpritea();
		sf::Vector2i pozycja_gracza_na_mapie = v2ftoi(vObiekty[nr_gracza_w_vObiekty]->podajPozycjeNaMapie());
		mapa.aktualizujPozycje(pozycja_gracza_na_mapie, punkt_centralny_spritea_gracza, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU);
	cout << pozycja_gracza_na_mapie.x << ' ' << pozycja_gracza_na_mapie.y << endl;
	//cout << "        " << ZEGAR_OD_ROZPOCZECIA_GRY.getElapsedTime().asSeconds() << endl;

		ekran.draw(mapa);

		//rysowanie na ekranie obiektow z vektora vObiekty
		for (int i = 0; i < vObiekty.size(); i++) {

			//cout << vObiekty[i]->podajPozycjeSprita().x << ' ' << vObiekty[i]->podajPozycjeSprita().y << endl;

			if (vObiekty[i]->podajPozycjeSprita().x < -100 || vObiekty[i]->podajPozycjeSprita().x > SZEROKOSC_EKRANU + 100);
			else if (vObiekty[i]->podajPozycjeSprita().y < -100 || vObiekty[i]->podajPozycjeSprita().y > WYSOKOSC_EKRANU + 100);
			else {
				ekran.draw(vObiekty[i]->podajSprite());
			}
		}

		ekran.display();

		const sf::Texture& tekstura_ekranu = ekran.getTexture();
		sf::Sprite sprite_ekranu(tekstura_ekranu);

		pozycjonowanieEkranu(sprite_ekranu, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, punkt_centralny_spritea_gracza, pozycja_gracza_na_mapie, rozmiar_kafelka, rozmiar_mapy_w_kafelkach);

		//cout << sprite_ekranu.getPosition().x << ' ' << sprite_ekranu.getPosition().y << endl;
		oknoAplikacji.draw(sprite_ekranu);


		oknoAplikacji.display();
	}
	return 0;
}

//////////////////////////////////////////////////////////

void pozycjonowanieEkranu(sf::Sprite& sprite_ekranu, unsigned int szerokosc_ekranu, unsigned int wysokosc_ekranu, 
	sf::Vector2i punkt_centralny_spritea_gracza, sf::Vector2i pozycja_gracza_na_mapie, sf::Vector2u rozmiar_kafelka, sf::Vector2u rozmiar_mapy_w_kafelkach)
{
	//ustawienie pozycji ekranu na oknie aplikacji - autorski sposób na p³ynne wyœwietlanie œciêtych kafelków na krañcach mapy
	unsigned int srodek_okna_x = szerokosc_ekranu / 2 - punkt_centralny_spritea_gracza.x;
	unsigned int srodek_okna_y = wysokosc_ekranu / 2 - punkt_centralny_spritea_gracza.y;

	sf::Vector2f pozycja_ekranu;
	if (pozycja_gracza_na_mapie.x < srodek_okna_x)
		pozycja_ekranu.x = rozmiar_kafelka.x * -1.0 + srodek_okna_x - pozycja_gracza_na_mapie.x > 0 ? 0 : rozmiar_kafelka.x * -1.0 + srodek_okna_x - pozycja_gracza_na_mapie.x;
	else if (pozycja_gracza_na_mapie.x + 2 * punkt_centralny_spritea_gracza.x > rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x)
		if (rozmiar_kafelka.x * -1.0 + (rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x) - (pozycja_gracza_na_mapie.x + 2 * punkt_centralny_spritea_gracza.x) < -2.0 * rozmiar_kafelka.x)
			pozycja_ekranu.x = -2.0 * rozmiar_kafelka.x;
		else
			pozycja_ekranu.x = rozmiar_kafelka.x * -1.0 + (rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x) - (pozycja_gracza_na_mapie.x + 2 * punkt_centralny_spritea_gracza.x);
	else
		pozycja_ekranu.x = rozmiar_kafelka.x * -1.0;

	if (pozycja_gracza_na_mapie.y < srodek_okna_y)
		pozycja_ekranu.y = rozmiar_kafelka.y * -1.0 + srodek_okna_y - pozycja_gracza_na_mapie.y > 0 ? 0 : rozmiar_kafelka.y * -1.0 + srodek_okna_y - pozycja_gracza_na_mapie.y;
	else if (pozycja_gracza_na_mapie.y + 2 * punkt_centralny_spritea_gracza.y > rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y)
		if (rozmiar_kafelka.y * -1.0 + (rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y) - (pozycja_gracza_na_mapie.y + 2 * punkt_centralny_spritea_gracza.y) < -2.0 * rozmiar_kafelka.y)
			pozycja_ekranu.y = -2.0 * rozmiar_kafelka.y;
		else
			pozycja_ekranu.y = rozmiar_kafelka.y * -1.0 + (rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y) - (pozycja_gracza_na_mapie.y + 2 * punkt_centralny_spritea_gracza.y);
	else
		pozycja_ekranu.y = rozmiar_kafelka.y * -1.0;

	sprite_ekranu.setPosition(pozycja_ekranu);
}
