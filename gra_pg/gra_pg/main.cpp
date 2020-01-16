
/*------------------------------------------------------------

Temat projektu: Green World - symulacja �wiata 2D.

Zakres projektu:
1. Mapa generowana losowo.
2. Mo�liwo�� tworzenia i zapisywnia wielu �wiat�w.
3. Gracz porusza si� w 2-wymiarowej przestrzeni.
4. Sterowanie - WSAD 8 kierunk�w.
5. Zwierz�ta, ro�liny i gracz mog� wchodzi� w proste interakcje.
6. Celem gry jest przetrwanie i eksploracja �wiata oraz poznanie jego mechanik.

Zrealizowane:
1. Opisanie g��wnych za�o�e� i zakresu projektu.
2. Instalacja i konfiguracja biblioteki SFML w Visual Studio 2017.
3. Stworzenie podstawowych element�w szkieletu gry.
4. Wst�pne oprogamowanie systemu sterowania postaci�.
5. Zapoznanie si� z technikami manipulacji obiektami na ekranie.
6. Rozbudowa i polepszenie systemu sterowania.
7. Stworzenie systemu zapisu map (plik zawieraj�cy dane mapy).
8. Stworzenie systemu wczytywania map.
9. Zmiana punktu odniesienia (z mapy na posta�) w wy�wietlaniu mapy.
10. Zapoznanie z systemem dziedziczenia.
11. Wykorzystanie systemu dziedziczenia w programie.
12. Podzia� kodu na pliki zgodnie z podej�ciem obiektowym.
13. Post�p w pracy nad systemem wyswietlania.
14. Przebudowa systemu wyswietlania na oparty na sf::View.
15. Dodanie przykladowego obiektu (drzewo).
16. Oprogramowanie zmiany rozmiaru okna (window resized).
17. Wy�wietlanie gry na osobnym RenderWindow, a nast�pnie na g��wnym oknie.

Co zrealizowalem w biezacym tygodniu?
1. Dodanie wskaznika FPS.
2. Dodanie systemu animacji ruchu postaci.
3. Wzbogacenie systemu poruszania o wp�yw warto�ci przyspieszenia (charakterystycznej dla danej instancji).

Co planuje na kolejny tydzien?
1. Dalsze prace nad systemem sterowania, generowania map.
2. Dodanie jakiego� zwierz�cia/innych obiekt�w.
3. Dodanie systemu kolizji.


------------------------------------------------------------*/

/*------------------------------------------------------------

Program glowny

------------------------------------------------------------*/

#include <SFML/Graphics.hpp>
#include <iostream>

#include "menu.h"

using namespace std;


int main()
{
	unsigned int SZEROKOSC_OKNA = 1000;
	unsigned int WYSOKOSC_OKNA = 800;

	sf::RenderWindow okno_aplikacji(sf::VideoMode(SZEROKOSC_OKNA, WYSOKOSC_OKNA, 32), "Gra PG - Green World");
	okno_aplikacji.setVerticalSyncEnabled(true);
	okno_aplikacji.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - SZEROKOSC_OKNA / 2, sf::VideoMode::getDesktopMode().height / 2 - WYSOKOSC_OKNA / 2));

	Menu* menu = new Menu(&okno_aplikacji);

	delete menu;

	return 0;
}