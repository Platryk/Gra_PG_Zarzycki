
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

#include "menu.h"


Menu::Menu(sf::RenderWindow * okno_aplikacji_in)
{
	okno_aplikacji = okno_aplikacji_in;

	fruktur.loadFromFile("czcionki/Fruktur-Regular.ttf");
	montserrat.loadFromFile("czcionki/Montserrat-Regular.ttf");

	kolor_opcji_niewybranej = sf::Color(11, 164, 0);
	kolor_opcji_wybranej = sf::Color(97, 255, 85);
	rozmiar_czcionki = 40;
	separacja_opcji_w_pionie = 80;
	grubosc_obrysu_tekstu = 9;

	tytul.setFont(fruktur);
	tytul.setStyle(sf::Text::Regular);
	tytul.setFillColor(kolor_opcji_niewybranej);
	tytul.setOutlineThickness(grubosc_obrysu_tekstu);
	tytul.setCharacterSize(rozmiar_czcionki);

	pomoc.setFont(montserrat);
	pomoc.setStyle(sf::Text::Bold);
	pomoc.setFillColor(sf::Color(9, 23, 12));
	pomoc.setOutlineColor(sf::Color(40, 145, 31));
	pomoc.setOutlineThickness(2);
	pomoc.setCharacterSize(25);
	pomoc.setString("Menu: W - gora, S - dol, Enter - zatwierdz\nSterowanie: WSAD (8 kierunkow)");
	pomoc.setPosition(sf::Vector2f(55, 55));

	SZEROKOSC_OKNA = okno_aplikacji->getSize().x;
	WYSOKOSC_OKNA = okno_aplikacji->getSize().y;

	SZEROKOSC_EKRANU_MENU = SZEROKOSC_OKNA - 100;
	WYSOKOSC_EKRANU_MENU = WYSOKOSC_OKNA - 100;

	mapa_tla_menu = new MapaKafelkowa("mapa_tla_menu", sf::Vector2u(50, 50), { 19, 20, 32, 33, 41 });

	SZEROKOSC_MAPY_W_PXL = mapa_tla_menu->podajRozmiarMapyWKafelkach().x * mapa_tla_menu->podajRozmiarKafelka().x;
	WYSOKOSC_MAPY_W_PXL = mapa_tla_menu->podajRozmiarMapyWKafelkach().y * mapa_tla_menu->podajRozmiarKafelka().y;

	ekran_menu.create(SZEROKOSC_MAPY_W_PXL, WYSOKOSC_MAPY_W_PXL);

	widok_ekranu_menu.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_MENU, WYSOKOSC_EKRANU_MENU));
	widok_ekranu_menu.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_MENU / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_MENU / WYSOKOSC_MAPY_W_PXL));

	pozycja_widoku_tla_menu = sf::Vector2f(SZEROKOSC_MAPY_W_PXL / 2, WYSOKOSC_MAPY_W_PXL / 2);

	predkosc_przesuwania_tla_menu = 1;
	srand(time(NULL));
	kierunek_przesuwania_tla_menu = sf::Vector2f((rand() % 30 + 70) / 100.0, (rand() % 30 + 70) / 100.0);
	if (rand() % 2 == 1)
		kierunek_przesuwania_tla_menu.x *= -1;
	if (rand() % 2 == 1)
		kierunek_przesuwania_tla_menu.y *= -1;

	this->menuGlowne();
	this->glownaPetla();
}

void Menu::glownaPetla()
{
	while (!zamknij_aplikacje)
	{
		//obs³uga zdarzeñ
		while (okno_aplikacji->pollEvent(zdarzenie))
		{
			switch (zdarzenie.type) {
			case sf::Event::Closed:
			{
				okno_aplikacji->close();
				zamknij_aplikacje = true;
				break;
			}

			case sf::Event::Resized:
			{
				sf::Vector2u rozmiarOkna = okno_aplikacji->getSize();

				SZEROKOSC_EKRANU_MENU -= SZEROKOSC_OKNA - rozmiarOkna.x;
				WYSOKOSC_EKRANU_MENU -= WYSOKOSC_OKNA - rozmiarOkna.y;

				SZEROKOSC_OKNA = rozmiarOkna.x;
				WYSOKOSC_OKNA = rozmiarOkna.y;

				okno_aplikacji->setView(sf::View(sf::FloatRect(0, 0, SZEROKOSC_OKNA, WYSOKOSC_OKNA)));

				widok_ekranu_menu.reset(sf::FloatRect(0.f, 0.f, SZEROKOSC_EKRANU_MENU, WYSOKOSC_EKRANU_MENU));
				widok_ekranu_menu.setViewport(sf::FloatRect(0.f, 0.f, (float)SZEROKOSC_EKRANU_MENU / SZEROKOSC_MAPY_W_PXL, (float)WYSOKOSC_EKRANU_MENU / WYSOKOSC_MAPY_W_PXL));

				this->aktualizujMenu();

				break;
			}
			case sf::Event::KeyReleased:
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					this->opcjaWGore();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					this->opcjaWDol();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					this->wybrano();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
					czy_wyswietlic_pomoc = czy_wyswietlic_pomoc ? false : true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					if (akcje_opcji_wyboru[liczba_opcji_wyboru] != "") {
						podswietlona_opcja = liczba_opcji_wyboru;
						this->wybrano();
					}
					else if (podswietlona_opcja != liczba_opcji_wyboru - 1)
						podswietlona_opcja = liczba_opcji_wyboru - 1;
					else
						this->wybrano();

					this->aktualizujMenu();
				}

				break;
			}
		}

		this->aktualizujWidokTlaMenu();
		this->wyswietlMenu();
	}
}

void Menu::opcjaWGore()
{
	if (podswietlona_opcja > 0)
		podswietlona_opcja--;
	else
		podswietlona_opcja = liczba_opcji_wyboru - 1;

	this->aktualizujMenu();
}

void Menu::opcjaWDol()
{
	if (podswietlona_opcja < liczba_opcji_wyboru - 1)
		podswietlona_opcja++;
	else
		podswietlona_opcja = 0;

	this->aktualizujMenu();
}

void Menu::wyswietlMenu()
{
	ekran_menu.setView(widok_ekranu_menu);
	ekran_menu.clear(sf::Color(50, 50, 50));
	ekran_menu.draw(*mapa_tla_menu);

	ekran_menu.display();

	const sf::Texture& tekstura_ekranu = ekran_menu.getTexture();
	sf::Sprite sprite_ekranu_menu(tekstura_ekranu);
	sprite_ekranu_menu.setPosition(sf::Vector2f(50, 50));

	okno_aplikacji->clear(sf::Color(50, 50, 50));
	okno_aplikacji->draw(sprite_ekranu_menu);

	okno_aplikacji->draw(tytul);
	for (int i = 0; i < liczba_opcji_wyboru; i++)
	{
		okno_aplikacji->draw(opcje_wyboru[i]);
	}
	if (czy_wyswietlic_pomoc)
		okno_aplikacji->draw(pomoc);

	okno_aplikacji->display();
}

void Menu::aktualizujMenu()
{
	for (int i = 0; i < liczba_opcji_wyboru; i++)
	{
		opcje_wyboru[i].setFont(fruktur);
		if (i == podswietlona_opcja) {
			opcje_wyboru[i].setStyle(sf::Text::Bold);
			opcje_wyboru[i].setFillColor(kolor_opcji_wybranej);
		}
		else {
			opcje_wyboru[i].setStyle(sf::Text::Regular);
			opcje_wyboru[i].setFillColor(kolor_opcji_niewybranej);
		}

		opcje_wyboru[i].setOutlineThickness(grubosc_obrysu_tekstu);
		opcje_wyboru[i].setCharacterSize(rozmiar_czcionki);

		opcje_wyboru[i].setPosition(sf::Vector2f(SZEROKOSC_OKNA / 2 - opcje_wyboru[i].getGlobalBounds().width / 2, opcje_wyboru[i].getPosition().y));
	}

	float pozycja_y = WYSOKOSC_OKNA / 2 - separacja_opcji_w_pionie * liczba_opcji_wyboru / 2;
	for (int i = 0; i < liczba_opcji_wyboru; i++) {
		opcje_wyboru[i].setPosition(sf::Vector2f(SZEROKOSC_OKNA / 2 - opcje_wyboru[i].getGlobalBounds().width / 2, pozycja_y));

		pozycja_y += separacja_opcji_w_pionie;
	}

	if (tytul.getString() != "")
		tytul.setPosition(sf::Vector2f(SZEROKOSC_OKNA / 2 - tytul.getGlobalBounds().width / 2, opcje_wyboru[0].getPosition().y - separacja_opcji_w_pionie));
}

void Menu::aktualizujWidokTlaMenu()
{
	pozycja_widoku_tla_menu.x += kierunek_przesuwania_tla_menu.x * predkosc_przesuwania_tla_menu;
	pozycja_widoku_tla_menu.y += kierunek_przesuwania_tla_menu.y * predkosc_przesuwania_tla_menu;

	if (pozycja_widoku_tla_menu.x < SZEROKOSC_EKRANU_MENU / 2) {
		pozycja_widoku_tla_menu.x = SZEROKOSC_EKRANU_MENU / 2;
		kierunek_przesuwania_tla_menu.x *= -1;
	}
	else if (pozycja_widoku_tla_menu.x > SZEROKOSC_MAPY_W_PXL - SZEROKOSC_EKRANU_MENU / 2) {
		pozycja_widoku_tla_menu.x = SZEROKOSC_MAPY_W_PXL - SZEROKOSC_EKRANU_MENU / 2;
		kierunek_przesuwania_tla_menu.x *= -1;
	}

	if (pozycja_widoku_tla_menu.y < WYSOKOSC_EKRANU_MENU / 2) {
		pozycja_widoku_tla_menu.y = WYSOKOSC_EKRANU_MENU / 2;
		kierunek_przesuwania_tla_menu.y *= -1;
	}
	else if (pozycja_widoku_tla_menu.y > WYSOKOSC_MAPY_W_PXL - WYSOKOSC_EKRANU_MENU / 2) {
		pozycja_widoku_tla_menu.y = WYSOKOSC_MAPY_W_PXL - WYSOKOSC_EKRANU_MENU / 2;
		kierunek_przesuwania_tla_menu.y *= -1;
	}

	widok_ekranu_menu.setCenter(pozycja_widoku_tla_menu);
	ekran_menu.setView(widok_ekranu_menu);
}

void Menu::wybrano()
{
	tytul.setString("");

	string wybrana_opcja = akcje_opcji_wyboru[podswietlona_opcja];

	if (wybrana_opcja == "wyjscie") {
		okno_aplikacji->close();
		zamknij_aplikacje = true;
	}
	else if (wybrana_opcja == "nowa_gra") {
		this->nowaGra();
	}
	else if (wybrana_opcja == "mala_mapa") {
		this->pauzaWGrze();
		gra = new Gra(okno_aplikacji, &zamknij_aplikacje, &czy_wyswietlic_pomoc, pomoc, sf::Vector2u(20, 20), {0,9,18,27,36,45,54});
	}
	else if (wybrana_opcja == "srednia_mapa") {
		this->pauzaWGrze();
		gra = new Gra(okno_aplikacji, &zamknij_aplikacje, &czy_wyswietlic_pomoc, pomoc, sf::Vector2u(25, 25), {10,28,37,46});
	}
	else if (wybrana_opcja == "duza_mapa") {
		this->pauzaWGrze();
		gra = new Gra(okno_aplikacji, &zamknij_aplikacje, &czy_wyswietlic_pomoc, pomoc, sf::Vector2u(35, 35), {38,39,40,48,49});
	}
	else if (wybrana_opcja == "wczytaj_gre") {
		this->wczytajGre();
	}
	else if (wybrana_opcja == "menu_glowne") {
		this->menuGlowne();
	}
	else if (wybrana_opcja == "zapisz_gre") {
		if (gra->zapiszGre())
			this->menuZapisano();
		else
			this->menuNieZapisano();
	}
	else if (wybrana_opcja.substr(0, 5) == "zapis"){
		this->pauzaWGrze();
		gra = new Gra(okno_aplikacji, &zamknij_aplikacje, &czy_wyswietlic_pomoc, pomoc, wybrana_opcja);
	}
	else if (wybrana_opcja == "wznow_gre") {
		gra->start();
	}
	else if (wybrana_opcja == "wyjdz_z_gry") {
		delete gra;
		this->menuGlowne();
	}
	else if (wybrana_opcja == "czy_na_pewno_przerwac_rozgrywke") {
		this->czyNaPewnoPrzerwacRozgrywke();
	}
	else if (wybrana_opcja == "czy_na_pewno_wyjsc_z_gry") {
		this->czyNaPewnoWyjscZGry();
	}
	else if (wybrana_opcja == "pauza_w_grze") {
		this->pauzaWGrze();
	}
}

void Menu::menuGlowne()
{
	liczba_opcji_wyboru = 3;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];
	
	int n = 0;
	opcje_wyboru[n].setString("Nowa gra");
	akcje_opcji_wyboru[n] = "nowa_gra";

	n++;
	opcje_wyboru[n].setString("Wczytaj gre");
	akcje_opcji_wyboru[n] = "wczytaj_gre";

	n++;
	opcje_wyboru[n].setString("Wyjscie");
	akcje_opcji_wyboru[n] = "czy_na_pewno_wyjsc_z_gry";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::nowaGra()
{
	liczba_opcji_wyboru = 4;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("Mala mapa");
	akcje_opcji_wyboru[n] = "mala_mapa";

	n++;
	opcje_wyboru[n].setString("Srednia mapa");
	akcje_opcji_wyboru[n] = "srednia_mapa";

	n++;
	opcje_wyboru[n].setString("Duza mapa");
	akcje_opcji_wyboru[n] = "duza_mapa";

	n++;
	opcje_wyboru[n].setString("Cofnij");
	akcje_opcji_wyboru[n] = "menu_glowne";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::wczytajGre()
{
	fstream plik;

	vector <string> nazwy_zapisow;
	for (int i = 1; i < 10; i++) {
		plik.open("zapisy/zapis" + to_string(i) + ".txt", fstream::in);

		if (plik.good()) {
			nazwy_zapisow.push_back("zapis" + to_string(i));
			plik.close();
		}
		else
			continue;
	}

	liczba_opcji_wyboru = nazwy_zapisow.size() + 1;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;

	for (int i = 0; i < nazwy_zapisow.size(); i++) {
		opcje_wyboru[n].setString(nazwy_zapisow[i]);
		akcje_opcji_wyboru[n] = nazwy_zapisow[i];

		n++;
	}

	opcje_wyboru[n].setString("Cofnij");
	akcje_opcji_wyboru[n] = "menu_glowne";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::pauzaWGrze()
{
	liczba_opcji_wyboru = 3;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("Wznow gre");
	akcje_opcji_wyboru[n] = "wznow_gre";

	n++;
	opcje_wyboru[n].setString("Zapisz gre");
	akcje_opcji_wyboru[n] = "zapisz_gre";

	n++;
	opcje_wyboru[n].setString("Wyjdz do menu glownego");
	akcje_opcji_wyboru[n] = "czy_na_pewno_przerwac_rozgrywke";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::czyNaPewnoPrzerwacRozgrywke()
{
	tytul.setString("Czy na pewno chcesz przerwac gre?");

	liczba_opcji_wyboru = 2;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("Tak");
	akcje_opcji_wyboru[n] = "wyjdz_z_gry";

	n++;
	opcje_wyboru[n].setString("Nie");
	akcje_opcji_wyboru[n] = "pauza_w_grze";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::czyNaPewnoWyjscZGry()
{
	tytul.setString("Czy na pewno chcesz wyjsc z gry?");

	liczba_opcji_wyboru = 2;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("Tak");
	akcje_opcji_wyboru[n] = "wyjscie";

	n++;
	opcje_wyboru[n].setString("Nie");
	akcje_opcji_wyboru[n] = "menu_glowne";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::menuZapisano()
{
	tytul.setString("Zapisano gre!");

	liczba_opcji_wyboru = 1;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("OK :D");
	akcje_opcji_wyboru[n] = "pauza_w_grze";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

void Menu::menuNieZapisano()
{
	tytul.setString("Nie udalo sie zapisac gry :/");

	liczba_opcji_wyboru = 1;
	podswietlona_opcja = 0;

	opcje_wyboru = new sf::Text[liczba_opcji_wyboru];
	akcje_opcji_wyboru = new string[liczba_opcji_wyboru + 1];

	int n = 0;
	opcje_wyboru[n].setString("Trudno...");
	akcje_opcji_wyboru[n] = "pauza_w_grze";

	n++;
	akcje_opcji_wyboru[n] = "";

	this->aktualizujMenu();
}

Menu::~Menu()
{
	mapa_tla_menu->usun();
}
