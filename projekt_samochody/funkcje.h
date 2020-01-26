#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "funkcje.h"
using namespace std;

struct s {
	string v;
};
struct rejestracje {
	string data;
	string numer;
	rejestracje* nast;
};
struct dane {
	string marka;
	string model;
	string rok;
	string pojemnosc;
	string nrSilnika;
	string nrNadwozia;
	string data;
};
struct listaPosiadaczy {
	string imie;
	string nazwisko;
	string data;
	listaPosiadaczy* nast;
};
struct wszystkieSamochody {
	dane dane;
	listaPosiadaczy* listaP;
	wszystkieSamochody* nast;
};
struct samochody {
	dane dane;
	string dataPosiadania;
	//rejestracje* numeryRej;
	samochody* nast;
};
struct wlasciciele {
	string imie;
	string nazwisko;
	string data;
	samochody* samochod;
	wlasciciele* nast;
};

s wytnijDane(string wiersz);
void wczytajPlik(ifstream& plikWej, wlasciciele*& listaWlascicieli, wszystkieSamochody*& listaSamochodow, rejestracje*& numeryRejestracyjne);
void dodajDoListy(listaPosiadaczy*& listaHead, string imie, string nazwisko, string data);
void dodajDoListy(wszystkieSamochody*& lista, dane d, listaPosiadaczy* listaP);
void dodajDoListy(rejestracje*& lista, string data, string numer);
void dodajDoListy(wlasciciele*& lista, string imie, string nazwisko, samochody samochod, string data);
bool sprawdzCzyIstnieje(wlasciciele* lista, string imie, string nazwisko);
wlasciciele* znajdzWlasciciela(wlasciciele* lista, string imie, string nazwisko);
listaPosiadaczy* znajdzPosiadacza(listaPosiadaczy* lista, string imie, string nazwisko);

listaPosiadaczy* znajdzSamochod(wszystkieSamochody* listaS, dane d);

void wypiszWlascicieli(ofstream& plikWyjscia, wlasciciele* lista);
void wypiszSamochody(ofstream& plikWyjscia, samochody* lista);

void usunListe(listaPosiadaczy*& pHead);
void usunListe(wlasciciele*& pHead);
void usunListe(samochody*& pHead);
void usunListe(wszystkieSamochody*& pHead);
void usunListe(rejestracje*& pHead);
//parametry
struct Parametry
{
	string plikWejsciowy;
	string plikWyjsciowy;
};

bool pliki(int argc, char** argv, Parametry& p);
int dzialaj(const Parametry& p);