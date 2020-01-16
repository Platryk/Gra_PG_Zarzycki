
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