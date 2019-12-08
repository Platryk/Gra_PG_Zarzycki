
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

Co zrealizowalem w biezacym tygodniu?
1. Zapoznanie z systemem dziedziczenia.
2. Wykorzystanie systemu dziedziczenia w programie.

Co planuje na kolejny tydzien?
1. Dalsze prace nad systemem sterowania, generowania map oraz wyœwietlania obiektów.


------------------------------------------------------------*/

/*------------------------------------------------------------

Program glowny

------------------------------------------------------------*/


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;


unsigned int SZEROKOSC_OKNA = 1024;
unsigned int WYSOKOSC_OKNA = 800;

unsigned int SZEROKOSC_EKRANU = 0;
unsigned int WYSOKOSC_EKRANU = 0;

unsigned int SZEROKOSC_MAPY = 0;
unsigned int WYSOKOSC_MAPY = 0;

class MapaKafelkowa : public sf::Drawable, public sf::Transformable
{
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	unsigned int** level;
	unsigned int szerokosc_mapy_w_kafelkach, wysokosc_mapy_w_kafelkach;
	sf::Vector2u rozmiar_kafelka;

public:
	bool load(const std::string& tileset, sf::Vector2u tile_size, sf::Vector2u rozmiar_mapy);
	void aktualizujPozycje(sf::Vector2i pozycja_gracza, sf::Sprite sprite_gracza);
	void generuj();
	void zapisz();
	bool wczytaj(string nazwa);

	string sciezka_wczytaj = "";
};

class RObiekt 
{
protected:
	string nazwa;
	sf::Vector2i pozycja_na_mapie;
	double predkosc = 4;
	sf::Vector2f kierunek;
	sf::Texture tekstura;
	sf::Sprite sprite;

public:
	RObiekt(string s_nazwa, sf::Vector2i u_pos, string tekstura_sciezka, sf::Vector2f skala);

	string podajNazwe() { return nazwa; }
	sf::Vector2i podajPozycjeNaMapie() { return pozycja_na_mapie; }
	sf::Vector2f podajPozycjeSprita() { return sprite.getPosition(); }
	sf::Sprite podajSprite() { return sprite; }
	virtual void aktualizujPolozenie();
	void zmienKierunekRuchuWPionie(float y);
	void zmienKierunekRuchuWPoziomie(float x);
};

class Gracz : public RObiekt
{
public:
	Gracz(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, string tekstura_sciezka, sf::Vector2f skala)
		: RObiekt::RObiekt(s_nazwa, u_pozycja_na_mapie, tekstura_sciezka, skala)
	{}

	virtual void aktualizujPolozenie();
};

int main()
{
	sf::RenderWindow oknoAplikacji(sf::VideoMode(SZEROKOSC_OKNA, WYSOKOSC_OKNA, 32), "Gra PG - Green World");

	sf::Clock glowny_zegar;

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

	SZEROKOSC_MAPY = rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x;
	WYSOKOSC_MAPY = rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y;

	typedef vector < RObiekt* > VObiektyT;
	VObiektyT vObiekty;

	vObiekty.push_back(new Gracz("gracz", sf::Vector2i(20, 20), "grafiki/char.png", sf::Vector2f(0.3, 0.3)));

	unsigned int nr_gracza_w_vObiekty = 0;;
	
	//g³ówna pêtla gry
	while (oknoAplikacji.isOpen()) 
	{
		//reset glownego zegara
		glowny_zegar.restart();

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
		for (int i = 0; i < vObiekty.size(); i++) {
			vObiekty[i]->aktualizujPolozenie();
		}
		
		//okreslenie fragmentu mapy do wyswietlenia w oparciu o pozycje gracza
		sf::Sprite sprite_gracza = vObiekty[nr_gracza_w_vObiekty]->podajSprite();
		sf::Vector2i pozycja_gracza_na_mapie = vObiekty[nr_gracza_w_vObiekty]->podajPozycjeNaMapie();
		mapa.aktualizujPozycje(pozycja_gracza_na_mapie, sprite_gracza);
	cout << pozycja_gracza_na_mapie.x << ' ' << pozycja_gracza_na_mapie.y << endl;

		ekran.draw(mapa);

		//wyswietlanie obiektow z vektora vObiekty
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

		unsigned int srodek_okna_x = SZEROKOSC_EKRANU / 2 - (sprite_gracza.getTexture()->getSize().x * sprite_gracza.getScale().x / 2);
		unsigned int srodek_okna_y = WYSOKOSC_EKRANU / 2 - (sprite_gracza.getTexture()->getSize().y * sprite_gracza.getScale().y / 2);

		//ustawienie pozycji ekranu na oknie aplikacji - autorski sposób na p³ynne wyœwietlanie œciêtych kafelków na krañcach mapy
		sf::Vector2f pozycja_ekranu;
		if (pozycja_gracza_na_mapie.x < srodek_okna_x)
			pozycja_ekranu.x = rozmiar_kafelka.x * -1.0 + srodek_okna_x - pozycja_gracza_na_mapie.x > 0 ? 0 : rozmiar_kafelka.x * -1.0 + srodek_okna_x - pozycja_gracza_na_mapie.x;
		else if (pozycja_gracza_na_mapie.x > rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x)
			pozycja_ekranu.x = rozmiar_kafelka.x * -1.0 + (rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x) - pozycja_gracza_na_mapie.x < -2.0 * rozmiar_kafelka.x ? -2.0 * rozmiar_kafelka.x : rozmiar_kafelka.x * -1.0 + (rozmiar_mapy_w_kafelkach.x * rozmiar_kafelka.x - srodek_okna_x) - pozycja_gracza_na_mapie.x;
		else
			pozycja_ekranu.x = rozmiar_kafelka.x * -1.0;

		if (pozycja_gracza_na_mapie.y < srodek_okna_y)
			pozycja_ekranu.y = rozmiar_kafelka.y * -1.0 + srodek_okna_y - pozycja_gracza_na_mapie.y > 0 ? 0 : rozmiar_kafelka.y * -1.0 + srodek_okna_y - pozycja_gracza_na_mapie.y;
		else if (pozycja_gracza_na_mapie.y > rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y)
			pozycja_ekranu.y = rozmiar_kafelka.y * -1.0 + (rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y) - pozycja_gracza_na_mapie.y < -2.0 * rozmiar_kafelka.y ? -2.0 * rozmiar_kafelka.y : rozmiar_kafelka.y * -1.0 + (rozmiar_mapy_w_kafelkach.y * rozmiar_kafelka.y - srodek_okna_y) - pozycja_gracza_na_mapie.y;
		else
			pozycja_ekranu.y = rozmiar_kafelka.y * -1.0;

		sprite_ekranu.setPosition(pozycja_ekranu);
		cout << sprite_ekranu.getPosition().x << ' ' << sprite_ekranu.getPosition().y << endl;
		oknoAplikacji.draw(sprite_ekranu);

		while (1)
			if (glowny_zegar.getElapsedTime().asMilliseconds() > 1000 / 60.0)
				break;

		oknoAplikacji.display();
	}
	return 0;
}

//////////////////////////////////////////////////////////


void MapaKafelkowa::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

bool MapaKafelkowa::load(const std::string & tileset, sf::Vector2u tile_size, sf::Vector2u rozmiar_mapy)
{
	szerokosc_mapy_w_kafelkach = rozmiar_mapy.x;
	wysokosc_mapy_w_kafelkach = rozmiar_mapy.y;
	rozmiar_kafelka = tile_size;

	level = new unsigned int*[wysokosc_mapy_w_kafelkach];
	for (int i = 0; i < wysokosc_mapy_w_kafelkach; i++)
		level[i] = new unsigned int[szerokosc_mapy_w_kafelkach];

	if (!this->wczytaj(sciezka_wczytaj)) {
		this->generuj();
		this->zapisz();
	}

	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(szerokosc_mapy_w_kafelkach * wysokosc_mapy_w_kafelkach * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < szerokosc_mapy_w_kafelkach; ++i)
		for (unsigned int j = 0; j < wysokosc_mapy_w_kafelkach; ++j)
		{
			// get the current tile number
			int tileNumber = level[j][i];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / rozmiar_kafelka.x);
			int tv = tileNumber / (m_tileset.getSize().x / rozmiar_kafelka.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * szerokosc_mapy_w_kafelkach) * 4];

			//// define its 4 corners
			//quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			//quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			//quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			//quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * rozmiar_kafelka.x, tv * rozmiar_kafelka.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * rozmiar_kafelka.x, tv * rozmiar_kafelka.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * rozmiar_kafelka.x, (tv + 1) * rozmiar_kafelka.y);
			quad[3].texCoords = sf::Vector2f(tu * rozmiar_kafelka.x, (tv + 1) * rozmiar_kafelka.y);
		}

	return true;
}

void MapaKafelkowa::aktualizujPozycje(sf::Vector2i pozycja_gracza_na_mapie, sf::Sprite sprite_gracza)
{
	for (unsigned int i = 0; i < szerokosc_mapy_w_kafelkach; ++i)
		for (unsigned int j = 0; j < wysokosc_mapy_w_kafelkach; ++j) {
			sf::Vertex* quad = &m_vertices[(i + j * szerokosc_mapy_w_kafelkach) * 4];

			unsigned int srodek_okna_x = SZEROKOSC_EKRANU / 2 - (sprite_gracza.getTexture()->getSize().x * sprite_gracza.getScale().x / 2);
			if (pozycja_gracza_na_mapie.x < srodek_okna_x)
				pozycja_gracza_na_mapie.x = srodek_okna_x;
			else if (pozycja_gracza_na_mapie.x > szerokosc_mapy_w_kafelkach * rozmiar_kafelka.x - srodek_okna_x)
				pozycja_gracza_na_mapie.x = szerokosc_mapy_w_kafelkach * rozmiar_kafelka.x - srodek_okna_x;

			unsigned int srodek_okna_y = WYSOKOSC_EKRANU / 2 - (sprite_gracza.getTexture()->getSize().y * sprite_gracza.getScale().y / 2);
			if (pozycja_gracza_na_mapie.y < srodek_okna_y)
				pozycja_gracza_na_mapie.y = srodek_okna_y;
			else if (pozycja_gracza_na_mapie.y > wysokosc_mapy_w_kafelkach * rozmiar_kafelka.y - srodek_okna_y)
				pozycja_gracza_na_mapie.y = wysokosc_mapy_w_kafelkach * rozmiar_kafelka.y - srodek_okna_y;

			unsigned int pozycja_x = srodek_okna_x - pozycja_gracza_na_mapie.x;
			unsigned int pozycja_y = srodek_okna_y - pozycja_gracza_na_mapie.y;

			unsigned int x = i * rozmiar_kafelka.x + pozycja_x;
			unsigned int y = j * rozmiar_kafelka.y + pozycja_y;
			quad[0].position = sf::Vector2f(x, y);

			x = (i + 1) * rozmiar_kafelka.x + pozycja_x;
			quad[1].position = sf::Vector2f(x, y);

			y = (j + 1) * rozmiar_kafelka.y + pozycja_y;
			quad[2].position = sf::Vector2f(x, y);

			x = i * rozmiar_kafelka.x + pozycja_x;
			quad[3].position = sf::Vector2f(x, y);
		}
}

void MapaKafelkowa::generuj()
{
	srand(time(NULL));

	int pula[] = {4,15,18};

	for (int y = 0; y < wysokosc_mapy_w_kafelkach; y++) {
		for (int x = 0; x < szerokosc_mapy_w_kafelkach; x++)
			level[y][x] = pula[rand() % (sizeof(pula)/sizeof(*pula))];
	}
}

void MapaKafelkowa::zapisz()
{
	fstream plik;
	string sciezka;

	int i = 1;
	do {
		plik.close();
		sciezka = "zapisy/zapis" + to_string(i) + ".txt";

		plik.open(sciezka);

		i++;
	} while (plik && (i < 5));

	plik.open(sciezka, fstream::out | fstream::in | fstream::trunc);
	
	for (int y = 0; y < wysokosc_mapy_w_kafelkach; y++) {
		for (int x = 0; x < szerokosc_mapy_w_kafelkach; x++) {
			plik << ' ' << level[y][x];
			cout << ' ' << level[y][x];
		}
		cout << endl;
		plik << endl;
	}

	plik.close();
}

bool MapaKafelkowa::wczytaj(string nazwa)
{
	if (nazwa == "")
		return false;

	fstream plik;
	string sciezka;

	sciezka = "zapisy/" + nazwa + ".txt";

	plik.open(sciezka, fstream::in);
	if (!plik.good())
		return false;

	for (int y = 0; y < wysokosc_mapy_w_kafelkach; y++) {
		for (int x = 0; x < szerokosc_mapy_w_kafelkach; x++) {
			plik >> level[y][x];
			cout << ' ' << level[y][x];
		}
		cout << endl;
	}

	plik.close();
	return true;
}

RObiekt::RObiekt(string s_nazwa, sf::Vector2i u_pozycja_na_mapie, string tekstura_sciezka, sf::Vector2f skala)
{
	nazwa = s_nazwa;
	pozycja_na_mapie = u_pozycja_na_mapie;
	tekstura.loadFromFile(tekstura_sciezka);

	sprite.setTexture(tekstura);
	sprite.setScale(skala);
}

void RObiekt::aktualizujPolozenie()
{
	pozycja_na_mapie.x += kierunek.x * predkosc;
	pozycja_na_mapie.y += kierunek.y * predkosc;

	pozycja_na_mapie.x = pozycja_na_mapie.x < 0 ? 0 : pozycja_na_mapie.x;
	pozycja_na_mapie.x = pozycja_na_mapie.x > SZEROKOSC_MAPY ? SZEROKOSC_MAPY : pozycja_na_mapie.x;
	pozycja_na_mapie.y = pozycja_na_mapie.y < 0 ? 0 : pozycja_na_mapie.y;
	pozycja_na_mapie.y = pozycja_na_mapie.y > WYSOKOSC_MAPY ? WYSOKOSC_MAPY : pozycja_na_mapie.y;

	unsigned int x = pozycja_na_mapie.x;
	unsigned int y = pozycja_na_mapie.y;

	sprite.setPosition(x, y);
}

void RObiekt::zmienKierunekRuchuWPionie(float y)
{
	kierunek.y = y;
}

void RObiekt::zmienKierunekRuchuWPoziomie(float x)
{
	kierunek.x = x;
}

void Gracz::aktualizujPolozenie()
{
	RObiekt::aktualizujPolozenie();

	unsigned int x = SZEROKOSC_EKRANU / 2 - (tekstura.getSize().x * sprite.getScale().x / 2);
	unsigned int y = WYSOKOSC_EKRANU / 2 - (tekstura.getSize().y * sprite.getScale().y / 2);

	if (pozycja_na_mapie.x < SZEROKOSC_EKRANU / 2 - (tekstura.getSize().x * sprite.getScale().x / 2)
		|| pozycja_na_mapie.x > SZEROKOSC_MAPY - (SZEROKOSC_EKRANU / 2 - (tekstura.getSize().x * sprite.getScale().x / 2)))
		x = pozycja_na_mapie.x;

	if (pozycja_na_mapie.y < WYSOKOSC_EKRANU / 2 - (tekstura.getSize().y * sprite.getScale().y / 2)
		|| pozycja_na_mapie.y > WYSOKOSC_MAPY - (WYSOKOSC_EKRANU / 2 - (tekstura.getSize().y * sprite.getScale().y / 2)))
		y = pozycja_na_mapie.y;

	sprite.setPosition(x, y);
}
