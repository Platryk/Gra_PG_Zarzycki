
#include <time.h>
#include <string>
#include <cmath>
#include <fstream>

#include "gra.h"

Gra::Gra(sf::RenderWindow* okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc)
{
	okno_aplikacji = okno_aplikacji_in;
	zamknij_aplikacje = b_zamknij_aplikacje;
	czy_wyswietlic_pomoc = b_czy_wyswietlic_pomoc;
	pomoc = t_pomoc;

	SZEROKOSC_OKNA = okno_aplikacji->getSize().x;
	WYSOKOSC_OKNA = okno_aplikacji->getSize().y;

	SZEROKOSC_EKRANU_GRY = SZEROKOSC_OKNA - 100;
	WYSOKOSC_EKRANU_GRY = WYSOKOSC_OKNA - 100;

	courier_prime.loadFromFile("czcionki/CourierPrime-Regular.ttf");

	wskaznik_FPS.setFont(courier_prime);
	wskaznik_FPS.setCharacterSize(13);
	wskaznik_FPS.setFillColor(sf::Color(220, 220, 220));
	wskaznik_FPS.setPosition(sf::Vector2f(5, 5));

	ZEGAR_OD_ROZPOCZECIA_GRY.restart();
}

Gra::Gra(sf::RenderWindow * okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc, string zapis)
	: Gra::Gra(okno_aplikacji_in, b_zamknij_aplikacje, b_czy_wyswietlic_pomoc, t_pomoc)
{
	if (!this->wczytajZapis(zapis))
		cout << "Nie udalo sie wczytac zapisu" << endl;
	else
		this->start();
}

Gra::Gra(sf::RenderWindow * okno_aplikacji_in, bool * b_zamknij_aplikacje, bool * b_czy_wyswietlic_pomoc, sf::Text t_pomoc, sf::Vector2u rozmiar_nowej_mapy, vector <int> v_pula_generatora)
	: Gra::Gra(okno_aplikacji_in, b_zamknij_aplikacje, b_czy_wyswietlic_pomoc, t_pomoc)
{
	fstream plik;

	int nr_zapisu = 1;
	do {
		sciezka_do_zapisu = "zapisy/zapis" + to_string(nr_zapisu) + ".txt";
		plik.open(sciezka_do_zapisu);
		nr_zapisu++;
	} while (plik.good());
	plik.close();

	this->nowaGra(rozmiar_nowej_mapy, v_pula_generatora);
	this->start();
}

void Gra::start()
{
	pauza = false;
	this->glownaPetla();
}

bool Gra::zapiszGre()
{
	cout << "Zapisywanie gry..." << endl;

	mapa_gry->zapisz();

	fstream plik;
	plik.open(sciezka_do_zapisu, fstream::out);
	if (!plik.good())
		return false;

	plik << "#mapa\n";
	plik << nazwa_mapy + "\n\n";

	plik << "#obiekty\n";
	string klasa, nazwa, sciezka_tekstury_lub_folderu;
	sf::Vector2f pozycja_na_mapie, skala;
	unsigned int predkosc, przyspieszenie;
	for (int i = 0; i < vObiekty.size(); i++) {
		nazwa = vObiekty[i]->podajNazwe();
		if (nazwa.substr(0, 5) == "gracz") {
			klasa = "Gracz";
			pozycja_na_mapie = vObiekty[i]->podajPozycjeNaMapie();
			predkosc = vObiekty[i]->podajPredkosc();
			przyspieszenie = vObiekty[i]->podajPrzyspieszenie();
			sciezka_tekstury_lub_folderu = vObiekty[i]->podajSciezkeTeksturyLubFolderu();
			skala = vObiekty[i]->podajSprite().getScale();
			plik << klasa << ' ' << nazwa << ' ' << pozycja_na_mapie.x << ' ' << pozycja_na_mapie.y << ' ' << predkosc << ' ' << przyspieszenie << ' ' << sciezka_tekstury_lub_folderu << ' ' << skala.x << ' ' << skala.y << '\n';
		}
		else if (nazwa.substr(0, 6) == "drzewo") {
			klasa = "Drzewo";
			pozycja_na_mapie = vObiekty[i]->podajPozycjeNaMapie();
			sciezka_tekstury_lub_folderu = vObiekty[i]->podajSciezkeTeksturyLubFolderu();
			skala = vObiekty[i]->podajSprite().getScale();
			plik << klasa << ' ' << nazwa << ' ' << pozycja_na_mapie.x << ' ' << pozycja_na_mapie.y << ' ' << sciezka_tekstury_lub_folderu << ' ' << skala.x << ' ' << skala.y << '\n';
		}
	}
	plik.close();

	return true;
}

bool Gra::wczytajZapis(string zapis)
{
	if (zapis == "")
		return false;

	sciezka_do_zapisu = "zapisy/" + zapis + ".txt";

	fstream plik;

	plik.open(sciezka_do_zapisu, fstream::in);

	if (!plik.good()) {
		cout << "Problem z zapisem: " + sciezka_do_zapisu << endl;
		return false;
	}

	int liczba;
	string slowo;
	string linia;
	for (int i = 0; i < 2; i++) {

		do {
			getline(plik, linia);
		} while (linia.length() == 0);

		switch (i) {
		case 0:
			plik >> nazwa_mapy;
			break;
		case 1:
			string klasa, nazwa, sciezka_tekstury_lub_folderu;
			sf::Vector2f pozycja_na_mapie, skala;
			unsigned int predkosc, przyspieszenie;

			while (plik >> klasa) {
				if (klasa == "Gracz") {
					plik >> nazwa >> pozycja_na_mapie.x >> pozycja_na_mapie.y >> predkosc >> przyspieszenie >> sciezka_tekstury_lub_folderu >> skala.x >> skala.y;
					vObiekty.push_back(new Gracz(nazwa, sf::Vector2f(pozycja_na_mapie.x, pozycja_na_mapie.y), predkosc, przyspieszenie, sciezka_tekstury_lub_folderu, sf::Vector2f(skala.x, skala.y)));
				}
				else if (klasa == "Drzewo") {
					plik >> nazwa >> pozycja_na_mapie.x >> pozycja_na_mapie.y >> sciezka_tekstury_lub_folderu >> skala.x >> skala.y;
					vObiekty.push_back(new Drzewo(nazwa, sf::Vector2f(pozycja_na_mapie.x, pozycja_na_mapie.y), sciezka_tekstury_lub_folderu, sf::Vector2f(skala.x, skala.y)));
				}
			}
			break;
		}
	}

	plik.close();


	mapa_gry = new MapaKafelkowa(nazwa_mapy);

	SZEROKOSC_MAPY_W_PXL = mapa_gry->podajRozmiarMapyWKafelkach().x * mapa_gry->podajRozmiarKafelka().x;
	WYSOKOSC_MAPY_W_PXL = mapa_gry->podajRozmiarMapyWKafelkach().y * mapa_gry->podajRozmiarKafelka().y;

	ekran_gry.create(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL);

	widok_ekranu_gry.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY));

	widok_ekranu_gry.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_GRY / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_GRY / WYSOKOSC_MAPY_W_PXL));

	return true;
}

void Gra::nowaGra(sf::Vector2u rozmiar_nowej_mapy, vector <int> v_pula_generatora)
{
	fstream plik;

	int nr_mapy = 1;
	do {
		nazwa_mapy = "mapa" + to_string(nr_mapy);
		plik.open("mapy/" + nazwa_mapy + ".txt");
		nr_mapy++;
	} while (plik.good());
	plik.close();


	mapa_gry = new MapaKafelkowa(nazwa_mapy, rozmiar_nowej_mapy, v_pula_generatora);

	SZEROKOSC_MAPY_W_PXL = mapa_gry->podajRozmiarMapyWKafelkach().x * mapa_gry->podajRozmiarKafelka().x;
	WYSOKOSC_MAPY_W_PXL = mapa_gry->podajRozmiarMapyWKafelkach().y * mapa_gry->podajRozmiarKafelka().y;

	vObiekty.push_back(new Gracz("gracz", sf::Vector2f(rand() % (SZEROKOSC_MAPY_W_PXL - 100), rand() % (WYSOKOSC_MAPY_W_PXL - 100)), 200, 3, "grafiki/postac2/", sf::Vector2f(0.15, 0.15)));

	ekran_gry.create(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL);

	widok_ekranu_gry.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY));

	widok_ekranu_gry.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_GRY / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_GRY / WYSOKOSC_MAPY_W_PXL));
}

void Gra::glownaPetla()
{
	//restart zegarow
	ZEGAR_DO_OBLICZANIA_POZYCJI.restart();
	ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS.restart();

	while (!pauza && !*zamknij_aplikacje)
	{
		//obs³uga zdarzeñ
		while (okno_aplikacji->pollEvent(zdarzenie))
		{
			switch (zdarzenie.type) {
			case sf::Event::Closed:
			{
				okno_aplikacji->close();
				*zamknij_aplikacje = true;
				break;
			}

			case sf::Event::Resized:
			{
				sf::Vector2u rozmiarOkna = okno_aplikacji->getSize();

				SZEROKOSC_EKRANU_GRY -= SZEROKOSC_OKNA - rozmiarOkna.x;
				WYSOKOSC_EKRANU_GRY -= WYSOKOSC_OKNA - rozmiarOkna.y;

				SZEROKOSC_OKNA = rozmiarOkna.x;
				WYSOKOSC_OKNA = rozmiarOkna.y;

				okno_aplikacji->setView(sf::View(sf::FloatRect(0, 0, SZEROKOSC_OKNA, WYSOKOSC_OKNA)));

				widok_ekranu_gry.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_GRY, WYSOKOSC_EKRANU_GRY));
				widok_ekranu_gry.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_GRY / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_GRY / WYSOKOSC_MAPY_W_PXL));

				break;
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
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
					*czy_wyswietlic_pomoc = *czy_wyswietlic_pomoc ? false : true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					pauza = true;

				break;
			}
		}

		pojedynczaKlatka();
	}
}

void Gra::pojedynczaKlatka()
{
	//aktualizacja pozycji obiektow z vektora vObiekty
	czas_do_pozycjonowania_w_sekundach = ZEGAR_DO_OBLICZANIA_POZYCJI.getElapsedTime().asSeconds();

	for (int i = 0; i < vObiekty.size(); i++) {
		vObiekty[i]->aktualizujPolozenie(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL, czas_do_pozycjonowania_w_sekundach);
	}


	//sprawdzanie i obsluga kolizji
	wykrywanieKolizji();


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

		wskaznik_FPS.setString(fps + " FPS");

		ZEGAR_DO_ODSWIEZANIA_WSKAZNIKA_FPS.restart();
	}


	//restart
	ZEGAR_DO_OBLICZANIA_POZYCJI.restart();


	//aktualizacja i wyczyszczenie tekstury ekranu gry
	ekran_gry.setView(widok_ekranu_gry);
	ekran_gry.clear(sf::Color(50, 50, 50));
	ekran_gry.draw(*mapa_gry);


	//gdy zmieni sie rozmiar wektora vObiekty
	if (kolejnosc_wyswietlania_obiektow_z_vObiekty.size() != vObiekty.size()) {
		kolejnosc_wyswietlania_obiektow_z_vObiekty.resize(vObiekty.size());

		for (int i = 0; i < vObiekty.size(); i++)
			kolejnosc_wyswietlania_obiektow_z_vObiekty[i] = i;
	}


	//sortowanie obiektow wedlug polozenia w pionie (rysowanie od gory)
	for (int i = 0; i < vObiekty.size() - 1; i++) {
		if (vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[i]]->podajPredkosc() != 0 || vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[i + 1]]->podajPredkosc() != 0 || pierwsze_sortowanie) {
			for (int ii = i + 1; ii < vObiekty.size(); ii++) {
				if (vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[i]]->podajPozycjeNaMapie().y + vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[i]]->podajSprite().getGlobalBounds().height
			> vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[ii]]->podajPozycjeNaMapie().y + vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[ii]]->podajSprite().getGlobalBounds().height) {
					unsigned int tmp = kolejnosc_wyswietlania_obiektow_z_vObiekty[i];
					kolejnosc_wyswietlania_obiektow_z_vObiekty[i] = kolejnosc_wyswietlania_obiektow_z_vObiekty[ii];
					kolejnosc_wyswietlania_obiektow_z_vObiekty[ii] = tmp;
				}
			}
		}
	}
	pierwsze_sortowanie = false;


	//rysowanie na ekranie obiektow z vektora vObiekty
	for (int i = 0; i < vObiekty.size(); i++) {
		ekran_gry.draw(vObiekty[kolejnosc_wyswietlania_obiektow_z_vObiekty[i]]->podajSprite());
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


	okno_aplikacji->clear(sf::Color(50, 50, 50));

	okno_aplikacji->draw(wskaznik_FPS);
	okno_aplikacji->draw(sprite_ekranu_gry);

	if (*czy_wyswietlic_pomoc)
		okno_aplikacji->draw(pomoc);

	okno_aplikacji->display();
}

sf::Vector2i Gra::v2ftoi(sf::Vector2f v2f)
{
	return sf::Vector2i((int)v2f.x, (int)v2f.y);
}

sf::Vector2f Gra::pozycjaWidokuGlownego(sf::Vector2f poz_gracza, sf::Vector2f srodek_spritea, sf::Vector2u rozmiar_mapy, sf::Vector2u rozmiar_ekranu)
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

void Gra::wykrywanieKolizji()
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

		/*sf::RectangleShape rect1(sf::Vector2f(prostokat1.width, prostokat1.height));
		rect1.setPosition(prostokat1.left, prostokat1.top);
		ekran_gry.draw(rect1);*/

		if (rzecz_kierunek1 != sf::Vector2f(0, 0)) {
			sf::Vector2f srodek_spritea(vObiekty[nr_ob1]->podajPunktCentralnySpritea());

			if (poz_map1.x < 0) {
				poz_map1.x = 0;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
			}
			else if (poz_map1.x > SZEROKOSC_MAPY_W_PXL - 2 * srodek_spritea.x) {
				poz_map1.x = SZEROKOSC_MAPY_W_PXL - 2 * srodek_spritea.x;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPoziomie(0);
			}

			if (poz_map1.y < 0) {
				poz_map1.y = 0;
				vObiekty[nr_ob1]->zmienRzeczywistyKierunekRuchuWPionie(0);
			}
			else if (poz_map1.y > WYSOKOSC_MAPY_W_PXL - 2 * srodek_spritea.y) {
				poz_map1.y = WYSOKOSC_MAPY_W_PXL - 2 * srodek_spritea.y;
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
