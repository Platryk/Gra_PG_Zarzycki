
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
3. Przebudowa systemu wyswietlania na oparty na sf::View.
4. Dodanie przykladowego obiektu (drzewo).
5. Oprogramowanie zmiany rozmiaru okna (window resized).
6. Wyœwietlanie gry na osobnym RenderWindow, a nastêpnie na g³ównym oknie.

Co planuje na kolejny tydzien?
1. Dalsze prace nad systemem sterowania, generowania map.
2. Dodanie jakiegoœ zwierzêcia/innych obiektów.
3. Dodanie animacji ruchu postaci.
4. Dodanie systemu kolizji.


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

sf::Vector2f pozycja_widoku_glownego(sf::Vector2i, sf::Vector2i, sf::Vector2u, sf::Vector2u);


int main()
{
	//deklaracja zmiennych
	sf::Clock GLOWNY_ZEGAR;
	sf::Clock ZEGAR_OD_ROZPOCZECIA_GRY;
	sf::Clock ZEGAR_DO_OBLICZANIA_POZYCJI;
	float czas_do_pozycjonowania_w_sekundach;

	unsigned int SZEROKOSC_OKNA = 1000;
	unsigned int WYSOKOSC_OKNA = 800;

	unsigned int SZEROKOSC_EKRANU_GRY = SZEROKOSC_OKNA - 100;
	unsigned int WYSOKOSC_EKRANU_GRY = WYSOKOSC_OKNA - 100;

	unsigned int SZEROKOSC_MAPY_W_PXL = 0;
	unsigned int WYSOKOSC_MAPY_W_PXL = 0;


	sf::RenderWindow okno_aplikacji(sf::VideoMode(SZEROKOSC_OKNA, WYSOKOSC_OKNA, 32), "Gra PG - Green World"); 
	okno_aplikacji.setVerticalSyncEnabled(true);
	okno_aplikacji.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - SZEROKOSC_OKNA / 2, sf::VideoMode::getDesktopMode().height / 2 - WYSOKOSC_OKNA / 2));

	MapaKafelkowa mapa;
	mapa.sciezka_wczytaj = "zapis1";
	sf::Vector2u rozmiar_kafelka(48, 48);
	sf::Vector2u rozmiar_mapy_w_kafelkach(40, 40);
	if (!mapa.load("grafiki/kafelki.jpg", rozmiar_kafelka, rozmiar_mapy_w_kafelkach))
		return -1;

	SZEROKOSC_MAPY_W_PXL = rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x;
	WYSOKOSC_MAPY_W_PXL = rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y;

	sf::RenderTexture ekran_gry;
	ekran_gry.create(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL);

	sf::View widok_ekranu_gry(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY));

	widok_ekranu_gry.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_GRY / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_GRY / WYSOKOSC_MAPY_W_PXL));

	//cout << widok_ekranu_gry.getViewport().left << "  " << widok_ekranu_gry.getViewport().top << "  " << "  " << widok_ekranu_gry.getViewport().width << "  " << widok_ekranu_gry.getViewport().height << endl;


	typedef vector < PodstawowyObiekt* > VObiektyT;
	VObiektyT vObiekty;

	vObiekty.push_back(new Gracz("gracz", sf::Vector2i(400, 400), 200, "grafiki/postac1.png", sf::Vector2f(0.3, 0.3)));
	vObiekty.push_back(new Drzewo("drzewo1", sf::Vector2i(500, 500), "grafiki/drzewo1.png", sf::Vector2f(0.4, 0.4)));

	cout << vObiekty[1]->podajSprite().getPosition().x << ' ' << vObiekty[1]->podajPozycjeNaMapie().y << endl;


	unsigned int nr_gracza_w_vObiekty;
	sf::Event zdarzenie;


	//restart zegarow
	ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
	GLOWNY_ZEGAR.restart();
	ZEGAR_OD_ROZPOCZECIA_GRY.restart();


	//g³ówna pêtla gry
	while (okno_aplikacji.isOpen()) 
	{
		//restart glownego zegara
		GLOWNY_ZEGAR.restart();


		//wyszukanie obiektu o nazwie "gracz" i zapisanie jego pozycji w wektorze do zmiennej
		for (int i = 0; i < vObiekty.size(); i++) {
			if (vObiekty[i]->podajNazwe() == "gracz") {
				nr_gracza_w_vObiekty = i;
				//cout << "                  |" << gracz_nr_w_vObiekty << endl;
				break;
			}
		}


		//obs³uga zdarzeñ
		while (okno_aplikacji.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type) {
			case sf::Event::Closed:
				okno_aplikacji.close();
				break;

			case sf::Event::Resized:
				{
					sf::Vector2u rozmiarOkna = okno_aplikacji.getSize();

					SZEROKOSC_EKRANU_GRY -= SZEROKOSC_OKNA - rozmiarOkna.x;
					WYSOKOSC_EKRANU_GRY -= WYSOKOSC_OKNA - rozmiarOkna.y;

					SZEROKOSC_OKNA = rozmiarOkna.x;
					WYSOKOSC_OKNA = rozmiarOkna.y;

					okno_aplikacji.setView(sf::View(sf::FloatRect(0, 0, SZEROKOSC_OKNA, WYSOKOSC_OKNA)));

					widok_ekranu_gry.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY));
					widok_ekranu_gry.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_GRY / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_GRY / WYSOKOSC_MAPY_W_PXL));
				}
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
					okno_aplikacji.close();

				break;
			}
		}


		ekran_gry.clear(sf::Color(50, 50, 50));

		ekran_gry.draw(mapa);


		//aktualizacja pozycji obiektow z vektora vObiekty
		czas_do_pozycjonowania_w_sekundach = ZEGAR_DO_OBLICZANIA_POZYCJI.getElapsedTime().asSeconds();

		for (int i = 0; i < vObiekty.size(); i++) {
			vObiekty[i]->aktualizujPolozenie(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL, czas_do_pozycjonowania_w_sekundach);
		}

		ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
		

		//rysowanie na ekranie obiektow z vektora vObiekty
		for (int i = 0; i < vObiekty.size(); i++) {

			//cout << vObiekty[i]->podajPozycjeSprita().x << ' ' << vObiekty[i]->podajPozycjeSprita().y << endl;

			if (vObiekty[i]->podajPozycjeSprita().x < -100 || vObiekty[i]->podajPozycjeSprita().x > 2000);
			else if (vObiekty[i]->podajPozycjeSprita().y < -100 || vObiekty[i]->podajPozycjeSprita().y > 2000);
			else {
				ekran_gry.draw(vObiekty[i]->podajSprite());
			}
		}


		//okreslenie fragmentu mapy do wyswietlenia w oparciu o pozycje gracza
		sf::Vector2i punkt_centralny_spritea_gracza = vObiekty[nr_gracza_w_vObiekty]->podajPunktCentralnySpritea();
		sf::Vector2i pozycja_gracza_na_mapie = v2ftoi(vObiekty[nr_gracza_w_vObiekty]->podajPozycjeNaMapie());

		//cout << pozycja_gracza_na_mapie.x << ' ' << pozycja_gracza_na_mapie.y << endl;
		//cout << "        " << ZEGAR_OD_ROZPOCZECIA_GRY.getElapsedTime().asSeconds() << endl;

		widok_ekranu_gry.setCenter(pozycja_widoku_glownego(
			pozycja_gracza_na_mapie, punkt_centralny_spritea_gracza, sf::Vector2u(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL), sf::Vector2u(SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY)));

		//cout << widok_ekranu_gry.getCenter().x << "  " << widok_ekranu_gry.getCenter().y << endl;


		ekran_gry.setView(widok_ekranu_gry);

		ekran_gry.display();

		const sf::Texture& tekstura_ekranu = ekran_gry.getTexture();
		sf::Sprite sprite_ekranu_gry(tekstura_ekranu);
		sprite_ekranu_gry.setPosition(sf::Vector2f(50, 50));


		okno_aplikacji.clear(sf::Color(50, 50, 50));

		okno_aplikacji.draw(sprite_ekranu_gry);

		okno_aplikacji.display();
	}
	return 0;
}

//////////////////////////////////////////////////////////

sf::Vector2f pozycja_widoku_glownego(sf::Vector2i poz_gracza, sf::Vector2i srodek_spritea, sf::Vector2u rozmiar_mapy, sf::Vector2u rozmiar_ekranu)
{
	float x = poz_gracza.x + (srodek_spritea.x / 2);
	float y = poz_gracza.y + srodek_spritea.y;

	if (x < rozmiar_ekranu.x / 2)
		x = rozmiar_ekranu.x / 2;
	else if (x > rozmiar_mapy.x - rozmiar_ekranu.x / 2)
		x = rozmiar_mapy.x - rozmiar_ekranu.x / 2;

	if (y < rozmiar_ekranu.y / 2)
		y = rozmiar_ekranu.y / 2;
	else if (y > rozmiar_mapy.y - rozmiar_ekranu.y / 2)
		y = rozmiar_mapy.y - rozmiar_ekranu.y / 2;

	//cout << "Glowny widok:" << x - (srodek_spritea.x / 2) << "   " << y - srodek_spritea.y <<endl;
	return sf::Vector2f(x, y);
}
