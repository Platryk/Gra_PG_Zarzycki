
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
12. Podzia³ kodu na pliki zgodnie z podejœciem obiektowym.
13. Postêp w pracy nad systemem wyswietlania.
14. Przebudowa systemu wyswietlania na oparty na sf::View.
15. Dodanie przykladowego obiektu (drzewo).
16. Oprogramowanie zmiany rozmiaru okna (window resized).
17. Wyœwietlanie gry na osobnym RenderWindow, a nastêpnie na g³ównym oknie.

Co zrealizowalem w biezacym tygodniu?
1. Dodanie wskaznika FPS.
2. Dodanie systemu animacji ruchu postaci.
3. Wzbogacenie systemu poruszania o wp³yw wartoœci przyspieszenia (charakterystycznej dla danej instancji).

Co planuje na kolejny tydzien?
1. Dalsze prace nad systemem sterowania, generowania map.
2. Dodanie jakiegoœ zwierzêcia/innych obiektów.
3. Dodanie systemu kolizji.


------------------------------------------------------------*/

/*------------------------------------------------------------

Program glowny

------------------------------------------------------------*/


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <cmath>

#include "mapa_kafelkowa.h"
#include "obiekty.h"

using namespace std;


sf::Vector2i v2ftoi(sf::Vector2f v2f) {
	return sf::Vector2i((int)v2f.x, (int)v2f.y);
}

sf::Vector2f pozycjaWidokuGlownego(sf::Vector2f, sf::Vector2f, sf::Vector2u, sf::Vector2u);

//void wykrywanieKolizji(vector < PodstawowyObiekt* >& vObiekty);
void wykrywanieKolizji(vector < PodstawowyObiekt* >& vObiekty, sf::RenderTexture &ekran_gry, unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl);

int main()
{
	//deklaracja zmiennych
	sf::Clock GLOWNY_ZEGAR;
	sf::Clock ZEGAR_OD_ROZPOCZECIA_GRY;
	sf::Clock ZEGAR_DO_OBLICZANIA_POZYCJI;
	sf::Clock ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS;
	float czas_do_pozycjonowania_w_sekundach;

	unsigned int SZEROKOSC_OKNA = 1000;
	unsigned int WYSOKOSC_OKNA = 800;

	unsigned int SZEROKOSC_EKRANU_GRY = SZEROKOSC_OKNA - 100;
	unsigned int WYSOKOSC_EKRANU_GRY = WYSOKOSC_OKNA - 100;

	unsigned int SZEROKOSC_MAPY_W_PXL = 0;
	unsigned int WYSOKOSC_MAPY_W_PXL = 0;

	//stworzenie czcionki i wskaznika FPS
	sf::Font courier_prime;
	courier_prime.loadFromFile("czcionki/CourierPrime-Regular.ttf");
	sf::Text wskaznik_FPS;
	wskaznik_FPS.setFont(courier_prime);
	wskaznik_FPS.setCharacterSize(13);
	wskaznik_FPS.setFillColor(sf::Color(220, 220, 220));
	wskaznik_FPS.setPosition(sf::Vector2f(5, 5));


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


	typedef vector < PodstawowyObiekt* > VObiektyT;
	VObiektyT vObiekty;

	vObiekty.push_back(new Gracz("gracz", sf::Vector2f(100, 200), 250, 2, "grafiki/postac2/", sf::Vector2f(0.15, 0.15)));
	vObiekty.push_back(new Drzewo("drzewo1", sf::Vector2f(300, 300), "grafiki/drzewo1.png", sf::Vector2f(0.4, 0.4)));


	unsigned int nr_gracza_w_vObiekty = 0;
	sf::Event zdarzenie;


	//restart zegarow
	ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
	ZEGAR_OD_ROZPOCZECIA_GRY.restart();
	ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS.restart();


	//g³ówna pêtla gry
	while (okno_aplikacji.isOpen()) 
	{
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


		//aktualizacja pozycji obiektow z vektora vObiekty
		czas_do_pozycjonowania_w_sekundach = ZEGAR_DO_OBLICZANIA_POZYCJI.getElapsedTime().asSeconds();

		for (int i = 0; i < vObiekty.size(); i++) {
			vObiekty[i]->aktualizujPolozenie(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL, czas_do_pozycjonowania_w_sekundach);
		}


		//sprawdzanie i obsluga kolizji
		wykrywanieKolizji(vObiekty, ekran_gry, SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL);


		//sortowanie obiektow wedlug polozenia w pionie (rysowanie od gory)
		for (int i = 0; i < vObiekty.size(); i++) {
			if (vObiekty[i]->podajPredkosc() != 0) {
				for (int ii = i + 1; ii < vObiekty.size(); ii++) {
					cout << vObiekty[i] << endl << endl;
					cout << vObiekty[ii] << endl;
					if (vObiekty[i]->podajPozycjeNaMapie().y + vObiekty[i]->podajSprite().getGlobalBounds().height > vObiekty[ii]->podajPozycjeNaMapie().y + vObiekty[ii]->podajSprite().getGlobalBounds().height) {
						//PodstawowyObiekt* tmp = vObiekty[i];
						//vObiekty[i] = vObiekty[ii];
						//vObiekty[ii] = tmp;

						cout << "swap" << endl;
					}
				}
			}
		}


		//wyszukanie obiektu o nazwie "gracz" i zapisanie jego pozycji w wektorze do zmiennej
		if (vObiekty[nr_gracza_w_vObiekty]->podajNazwe() != "gracz") {
			for (int i = 0; i < vObiekty.size(); i++) {
				if (vObiekty[i]->podajNazwe() == "gracz") {
					nr_gracza_w_vObiekty = i;
					break;
				}
			}
		}
		

		//prowadzenie wartosci wskaznika FPS
		if (ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS.getElapsedTime().asSeconds() >= 0.2f) {
			string fps = to_string((int)(1 / ZEGAR_DO_OBLICZANIA_POZYCJI.getElapsedTime().asSeconds()));
			if (fps.length() == 1)
				fps = "  " + fps;
			else if (fps.length() == 2)
				fps = ' ' + fps;

			wskaznik_FPS.setString( fps + " FPS");

			ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS.restart();
		}


		//restart
		ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
		

		//aktualizacja i wyczyszczenie tekstury ekranu gry
		ekran_gry.setView(widok_ekranu_gry);
		ekran_gry.clear(sf::Color(50, 50, 50));
		ekran_gry.draw(mapa);


		//rysowanie na ekranie obiektow z vektora vObiekty
		for (int i = 0; i < vObiekty.size(); i++) {

			ekran_gry.draw(vObiekty[i]->podajSprite());
		}


		//okreslenie fragmentu mapy do wyswietlenia w oparciu o pozycje gracza
		sf::Vector2f punkt_centralny_spritea_gracza = vObiekty[nr_gracza_w_vObiekty]->podajPunktCentralnySpritea();
		sf::Vector2f pozycja_gracza_na_mapie = vObiekty[nr_gracza_w_vObiekty]->podajPozycjeNaMapie();

		widok_ekranu_gry.setCenter(pozycjaWidokuGlownego(
			pozycja_gracza_na_mapie, punkt_centralny_spritea_gracza, sf::Vector2u(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL), sf::Vector2u(SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY)));


		ekran_gry.display();

		const sf::Texture& tekstura_ekranu = ekran_gry.getTexture();
		sf::Sprite sprite_ekranu_gry(tekstura_ekranu);
		sprite_ekranu_gry.setPosition(sf::Vector2f(50, 50));


		okno_aplikacji.clear(sf::Color(50, 50, 50));

		okno_aplikacji.draw(wskaznik_FPS);
		okno_aplikacji.draw(sprite_ekranu_gry);

		okno_aplikacji.display();
	}
	return 0;
}

//////////////////////////////////////////////////////////

sf::Vector2f pozycjaWidokuGlownego(sf::Vector2f poz_gracza, sf::Vector2f srodek_spritea, sf::Vector2u rozmiar_mapy, sf::Vector2u rozmiar_ekranu)
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

	return sf::Vector2f(x, y);
}

void wykrywanieKolizji(vector<PodstawowyObiekt*>& vObiekty, sf::RenderTexture &ekran_gry, unsigned int szerokosc_mapy_w_pxl, unsigned int wysokosc_mapy_w_pxl)
{
	sf::Vector2f poz_map1, poz_map2;
	sf::Vector2f rzecz_kierunek1, rzecz_kierunek2;
	unsigned int predkosc1, predkosc2;
	sf::FloatRect prostokat1, prostokat2;
	sf::Vector2f czesc_wspolna;

	for (int nr_ob1 = 0; nr_ob1 < vObiekty.size(); nr_ob1++) {
		poz_map1 = vObiekty[nr_ob1]->podajPozycjeNaMapie();
		rzecz_kierunek1 = vObiekty[nr_ob1]->podajRzeczywistyKierunekRuchu();
		prostokat1 = vObiekty[nr_ob1]->podajProstokatKolizyjny();

		sf::RectangleShape rect1(sf::Vector2f(prostokat1.width, prostokat1.height));
		rect1.setPosition(prostokat1.left, prostokat1.top);
		ekran_gry.draw(rect1);

		if (rzecz_kierunek1 != sf::Vector2f(0, 0)) {
			sf::Vector2f srodek_spritea(vObiekty[nr_ob1]->podajPunktCentralnySpritea());

			if (poz_map1.x < 0) {
				poz_map1.x = 0;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
			}
			else if (poz_map1.x > szerokosc_mapy_w_pxl - 2 * srodek_spritea.x) {
				poz_map1.x = szerokosc_mapy_w_pxl - 2 * srodek_spritea.x;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
			}

			if (poz_map1.y < 0) {
				poz_map1.y = 0;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPionie(0);
			}
			else if (poz_map1.y > wysokosc_mapy_w_pxl - 2 * srodek_spritea.y) {
				poz_map1.y = wysokosc_mapy_w_pxl - 2 * srodek_spritea.y;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPionie(0);
			}

			vObiekty[nr_ob1]->ustawPozycjeNaMapie(poz_map1);
		}

		for (int nr_ob2 = nr_ob1 + 1; nr_ob2 < vObiekty.size(); nr_ob2++) {

			prostokat2 = vObiekty[nr_ob2]->podajProstokatKolizyjny();
			if (prostokat1.intersects(prostokat2)) {

				if (prostokat1.left + (prostokat1.width / 2) <= prostokat2.left + (prostokat2.width / 2))
					czesc_wspolna.x = (prostokat1.left + prostokat1.width) - prostokat2.left;
				else
					czesc_wspolna.x = (prostokat2.left + prostokat2.width) - prostokat1.left;
				if (prostokat1.top + (prostokat1.height / 2) <= prostokat2.top + (prostokat2.height / 2))
					czesc_wspolna.y = (prostokat1.top + prostokat1.height) - prostokat2.top;
				else
					czesc_wspolna.y = (prostokat2.top + prostokat2.height) - prostokat1.top;

				if (czesc_wspolna.x <= czesc_wspolna.y) {

					if (rzecz_kierunek1.x != 0) {
						vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
						if (prostokat1.left + (prostokat1.width / 2) <= prostokat2.left + (prostokat2.width / 2)) {
							poz_map1 = sf::Vector2f(prostokat2.left - prostokat1.width + (poz_map1.x - prostokat1.left), poz_map1.y);
							vObiekty[nr_ob1]->ustawPozycjeNaMapie(poz_map1);
						}
						else {
							poz_map1 = sf::Vector2f(prostokat2.left + prostokat2.width + (poz_map1.x - prostokat1.left), poz_map1.y);
							vObiekty[nr_ob1]->ustawPozycjeNaMapie(poz_map1);
						}
					}
					if (rzecz_kierunek2.x != 0) {
						vObiekty[nr_ob2]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
					}

					//cout << "Zderzenie po x" << endl;
				}
				else {
					
					if (rzecz_kierunek1.y != 0) {
						vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPionie(0);
						if (prostokat1.top + (prostokat1.height / 2) <= prostokat2.top + (prostokat2.height / 2)) {
							poz_map1 = sf::Vector2f(poz_map1.x, prostokat2.top - prostokat1.height + (poz_map1.y - prostokat1.top));
							vObiekty[nr_ob1]->ustawPozycjeNaMapie(poz_map1);
						}
						else {
							poz_map1 = sf::Vector2f(poz_map1.x, prostokat2.top + prostokat2.height + (poz_map1.y - prostokat1.top));
							vObiekty[nr_ob1]->ustawPozycjeNaMapie(poz_map1);
						}
					}
					if (rzecz_kierunek2.y != 0) {
						vObiekty[nr_ob2]->zmienRzeczywistyKierunekRuchuWPionie(0);
					}

					//cout << "Zderzenie po y" << endl;
				}
			}
		}
	}
}

/* archiwum cout-ow itp

	cout << rand() << endl;

	cout << "Glowny widok:" << x - (srodek_spritea.x / 2) << "   " << y - srodek_spritea.y <<endl;

	cout << widok_ekranu_gry.getCenter().x << "  " << widok_ekranu_gry.getCenter().y << endl;

	cout << pozycja_gracza_na_mapie.x << ' ' << pozycja_gracza_na_mapie.y << endl;

	cout << "        " << ZEGAR_OD_ROZPOCZECIA_GRY.getElapsedTime().asSeconds() << endl;

	cout << "                  |" << gracz_nr_w_vObiekty << endl;

	cout << vObiekty[1]->podajSprite().getPosition().x << ' ' << vObiekty[1]->podajPozycjeNaMapie().y << endl;

	cout << widok_ekranu_gry.getViewport().left << "  " << widok_ekranu_gry.getViewport().top << "  " << "  " << widok_ekranu_gry.getViewport().width << "  " << widok_ekranu_gry.getViewport().height << endl;

	cout << czesc_wspolna.x << ' ' << czesc_wspolna.y << endl;


*/