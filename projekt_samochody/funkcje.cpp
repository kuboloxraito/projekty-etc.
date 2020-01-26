#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include "funkcje.h"
using namespace std;

s wytnijDane(string wiersz) {
	s n;
	n.v = wiersz.substr(wiersz.find(": ") + 2);
	return n;
}
void wczytajPlik(ifstream& plikWej, wlasciciele*& listaWlascicieli, wszystkieSamochody*& listaSamochodow, rejestracje*& numerRej) {
	int flaga = 3; // 0 - samochod, 1 - rejestracja, 2 - wlasciciele, 3 - nic
	samochody samochod;
	//samochod.numeryRej = nullptr;
	wlasciciele wlasciciel;
	string wiersz;
	stringstream tmp;
	while (getline(plikWej, wiersz)) {
		if (wiersz == "--- samochod ---") { //wprowadz nowy samochod
			flaga = 0;
		}
		else if (wiersz == "numery rejestracyjne:") { //wprowadz nowe rejestracje
			flaga = 1;
		}
		else if (wiersz == "wlasciciele:") { //wprowadz nowych wlascicieli
			flaga = 2;
		}
		else {
			if (flaga == 0) {
				if (wiersz == "") { //tmp wypelniony, gotowy do dodania jako samochod
					tmp >> samochod.dane.marka >> samochod.dane.model >> samochod.dane.rok >> samochod.dane.pojemnosc >> samochod.dane.nrSilnika >> samochod.dane.nrNadwozia >> samochod.dane.data;
					dodajDoListy(listaSamochodow, samochod.dane, nullptr);
					stringstream().swap(tmp);
				}
				else {
					tmp << wytnijDane(wiersz).v << ' ';
				}
			}
			else if (flaga == 1) { //dodaj do listy z numerami
				if (wiersz == "") continue;
				else {
					rejestracje nr;
					tmp << wiersz;
					tmp >> nr.data >> nr.numer;
					dodajDoListy(numerRej, nr.data, nr.numer);
					stringstream().swap(tmp);
				}
			}
			else if (flaga == 2) { //dodaj do listy wlascicieli
				if (wiersz == "") continue;
				else {
					if (wiersz.find(',') != numeric_limits<size_t>::max()) { //jesli >1 wlasciciel przy dacie
						tmp << wiersz.substr(0, wiersz.find(',')); //data i pierwszy wlasciciel
						tmp >> wlasciciel.data >> wlasciciel.imie >> wlasciciel.nazwisko;
						dodajDoListy(listaWlascicieli, wlasciciel.imie, wlasciciel.nazwisko, samochod, wlasciciel.data);
						dodajDoListy(listaSamochodow->listaP, wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data);
						//listaSamochodow->listaP = new listaPosiadaczy{ wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data, listaSamochodow->listaP };
						stringstream().swap(tmp);

						while (wiersz.find(',') != numeric_limits<size_t>::max()) {
							wiersz = wiersz.substr(wiersz.find(',') + 2); //reszta wlascicieli z tej daty
							tmp << wiersz.substr(0, wiersz.find(','));
							tmp >> wlasciciel.imie >> wlasciciel.nazwisko;
							dodajDoListy(listaWlascicieli, wlasciciel.imie, wlasciciel.nazwisko, samochod, wlasciciel.data);
							dodajDoListy(listaSamochodow->listaP, wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data);
							//listaSamochodow->listaP = new listaPosiadaczy{ wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data, listaSamochodow->listaP };
							stringstream().swap(tmp);
						}
					}
					else {
						tmp << wiersz;
						tmp >> wlasciciel.data >> wlasciciel.imie >> wlasciciel.nazwisko;
						dodajDoListy(listaWlascicieli, wlasciciel.imie, wlasciciel.nazwisko, samochod, wlasciciel.data);
						dodajDoListy(listaSamochodow->listaP, wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data);
						//listaSamochodow->listaP = new listaPosiadaczy{ wlasciciel.imie, wlasciciel.nazwisko, wlasciciel.data, listaSamochodow->listaP };
						stringstream().swap(tmp);
					}
				}
			}
			else return;
		}
	}
}

void dodajDoListy(listaPosiadaczy*& listaHead, string imie, string nazwisko, string data) {	// lista posiadaczy samochodu
	if (not listaHead) // lista pusta
		listaHead = new listaPosiadaczy{ imie, nazwisko, data, nullptr };
	else
	{
		auto p = listaHead;

		while (p->nast)
			p = p->nast;

		// w tym miejsce p wskazuje na ostatnie element listy
		p->nast = new listaPosiadaczy{ imie, nazwisko, data, nullptr };
	}
}
void dodajDoListy(wszystkieSamochody*& lista, dane d, listaPosiadaczy* listaP) {	//lista wszystkich samochodow
	lista = new wszystkieSamochody{ d, listaP, lista };
}
void dodajDoListy(rejestracje*& lista, string data, string numer) {	//lista numerow rejestracyjnych
	lista = new rejestracje{ data, numer, lista };
}
void dodajDoListy(wlasciciele*& lista, string imie, string nazwisko, samochody samochod, string data) {	//lista wszystkich wlascicieli
	/*
	if (!sprawdzCzyIstnieje(lista, imie, nazwisko)) {
		samochody* s = new samochody{ samochod.dane, "", samochod.numeryRej, nullptr };
		lista = new wlasciciele{ imie, nazwisko, data, s, lista };
	}
	else {
		wlasciciele* p = znajdzWlasciciela(lista, imie, nazwisko);
		p->samochod = new samochody{ samochod.dane, "", samochod.numeryRej, p->samochod };
	}
	*/
	if (!sprawdzCzyIstnieje(lista, imie, nazwisko)) {
		if (not lista) // lista pusta
		{
			samochody* s = new samochody{ samochod.dane, "", nullptr };
			lista = new wlasciciele{ imie, nazwisko, data, s, nullptr };
		}
		else // lista niepusta
		{
			auto p = lista;
			while (p->nast)
				p = p->nast;
			// w tym miejsce p wskazuje na ostatnie element listy
			samochody* s = new samochody{ samochod.dane, "", nullptr };
			p->nast = new wlasciciele{ imie, nazwisko, data, s, nullptr };
		}
	}
	else {
		wlasciciele* p = znajdzWlasciciela(lista, imie, nazwisko);
		p->samochod = new samochody{ samochod.dane, "", p->samochod };
	}

}
bool sprawdzCzyIstnieje(wlasciciele* lista, string imie, string nazwisko) {
	if (not lista)
		return false;
	if (lista->nazwisko == nazwisko && lista->imie == imie)
		return true;
	return sprawdzCzyIstnieje(lista->nast, imie, nazwisko);
}
wlasciciele* znajdzWlasciciela(wlasciciele* lista, string imie, string nazwisko) {
	if (not lista)
		return nullptr;
	if (lista->nazwisko == nazwisko && lista->imie == imie)
		return lista;
	return znajdzWlasciciela(lista->nast, imie, nazwisko);
}

void dodajDatePosiadania(wlasciciele*& listaW, wszystkieSamochody* listaS) {
	wlasciciele* sprawdzanyWlasciciel = listaW;
	while (sprawdzanyWlasciciel) {
		samochody* sprawdzanySamochod = sprawdzanyWlasciciel->samochod;
		while (sprawdzanySamochod) {
			listaPosiadaczy* aktualnySamochod = znajdzSamochod(listaS, sprawdzanySamochod->dane);

			//string nowaData;
			listaPosiadaczy* sprawdzanyPosiadacz = znajdzPosiadacza(aktualnySamochod, sprawdzanyWlasciciel->imie, sprawdzanyWlasciciel->nazwisko);	//sprawdzac ludzi od konca?, najlepiej zrobic liste dwukierunkowa
			if (!sprawdzanyPosiadacz->nast) {			//dodac funkcje z porownywaniem daty, na podstawie tego okreslic posiadanie samochodu
				sprawdzanySamochod->dataPosiadania = "aktualny wlasciciel";	//jesli jego data jest wieksza od nastepnej to 
			}
			else if (sprawdzanyPosiadacz->nast->data != sprawdzanyPosiadacz->data) {
				sprawdzanySamochod->dataPosiadania = sprawdzanyPosiadacz->data + " do " + sprawdzanyPosiadacz->nast->data;
			}
			else {
				listaPosiadaczy* nastWlasc = sprawdzanyPosiadacz->nast;
				while (nastWlasc && nastWlasc->data == sprawdzanyPosiadacz->data) { //szukamy nastepnego wlasciciela (data musi byc wieksza)
					nastWlasc = nastWlasc->nast;
				}
				if (!nastWlasc) sprawdzanySamochod->dataPosiadania = "aktualny wlasciciel";
				else sprawdzanySamochod->dataPosiadania = sprawdzanyPosiadacz->data + " do " + nastWlasc->data;
			}
			sprawdzanySamochod = sprawdzanySamochod->nast;
		}
		sprawdzanyWlasciciel = sprawdzanyWlasciciel->nast;
	}

}

listaPosiadaczy* znajdzSamochod(wszystkieSamochody* listaS, dane d) { //znajduje samochod z listy wszystkich samochodow i zwraca liste posiadaczy tego samochodu
	while (listaS->dane.nrNadwozia != d.nrNadwozia) {
		listaS = listaS->nast;
	}
	return listaS->listaP;
}
listaPosiadaczy* znajdzPosiadacza(listaPosiadaczy* lista, string imie, string nazwisko) { //znajduje aktualnego wlasciciela w liscie posiadaczy aktualnego samochodu
	if (not lista)
		return nullptr;
	if (lista->imie == imie && lista->nazwisko == nazwisko)
		return lista;
	return znajdzPosiadacza(lista->nast, imie, nazwisko);
}

void wypiszSamochody(ofstream& plikWyjscia, samochody* lista) {
	while (lista) {
		plikWyjscia << "--- samochod ---" << endl;
		plikWyjscia << "marka: " << lista->dane.marka << endl;
		plikWyjscia << "model: " << lista->dane.model << endl;
		plikWyjscia << "rok produkcji: " << lista->dane.rok << endl;
		plikWyjscia << "pojemnosc silnika: " << lista->dane.pojemnosc << endl;
		plikWyjscia << "nr silnika: " << lista->dane.nrSilnika << endl;
		plikWyjscia << "nr nadwozia: " << lista->dane.nrNadwozia << endl;
		plikWyjscia << "data pierwszej rejestracji: " << lista->dane.data << endl << endl;

		plikWyjscia << "okres posiadania: " << lista->dataPosiadania << endl << endl;

		//ewentualnie lista numerow rejestracyjnych (do dodania)

		lista = lista->nast;
	}
}
void wypiszWlascicieli(ofstream& plikWyjscia, wlasciciele* lista) {
	while (lista)
	{
		plikWyjscia << lista->imie << ' ' << lista->nazwisko << ' ' << endl;
		plikWyjscia << "Posiadane samochody: " << endl;
		wypiszSamochody(plikWyjscia, lista->samochod);

		lista = lista->nast;
	}
}

void usunListe(wlasciciele*& pHead)
{
	auto p = pHead;

	while (pHead)
	{
		usunListe(pHead->samochod);
		p = pHead->nast;
		delete pHead;
		pHead = p;
	}
}
void usunListe(samochody*& pHead)
{
	auto p = pHead;

	while (pHead)
	{
		p = pHead->nast;
		delete pHead;
		pHead = p;
	}
}
void usunListe(rejestracje*& pHead)
{
	auto p = pHead;

	while (pHead)
	{
		p = pHead->nast;
		delete pHead;
		pHead = p;
	}
}

void usunListe(wszystkieSamochody*& pHead)
{
	auto p = pHead;

	while (pHead)
	{
		usunListe(pHead->listaP);
		p = pHead->nast;
		delete pHead;
		pHead = p;
	}
}
void usunListe(listaPosiadaczy*& pHead)
{
	auto p = pHead;

	while (pHead)
	{
		p = pHead->nast;
		delete pHead;
		pHead = p;
	}
}
//parametry
bool pliki(int argc, char** argv, Parametry& p) {
	for (int i = 0; i < argc - 1; ++i) {
		string param = argv[i];
		if (param == "-i")
			p.plikWejsciowy = argv[++i];
		else if (param == "-o")
			p.plikWyjsciowy = argv[++i];

	}
	return p.plikWejsciowy != "" && p.plikWyjsciowy != "";
}

int dzialaj(const Parametry& p) {
	ifstream plikWejscia;
	plikWejscia.open(p.plikWejsciowy);
	if (!plikWejscia)
	{
		cerr << "Nie udalo sie odczytac pliku: " << p.plikWejsciowy << endl;
		return EXIT_FAILURE;
	}

	ofstream plikWyjscia;
	plikWyjscia.open(p.plikWyjsciowy);
	if (!plikWyjscia)
	{
		cerr << "Nie udalo sie odczytac pliku: " << p.plikWyjsciowy << endl;
		return EXIT_FAILURE;
	}

	//funkcje
	wszystkieSamochody* listaSamochodow = nullptr;
	wlasciciele* listaWlascicieli = nullptr;
	rejestracje* numerRej = nullptr;
	wczytajPlik(plikWejscia, listaWlascicieli, listaSamochodow, numerRej);
	dodajDatePosiadania(listaWlascicieli, listaSamochodow);
	wypiszWlascicieli(plikWyjscia, listaWlascicieli);

	usunListe(listaSamochodow);
	usunListe(listaWlascicieli);
	usunListe(numerRej);

	return EXIT_SUCCESS;
}