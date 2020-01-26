#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "funkcje.h"
//#include "mem-leak-detect.h"
using namespace std;

int main(int argc, char* argv[])
{
	Parametry p;
	if (!pliki(argc, argv, p))
	{
		cerr << "Wrong parameters.\n";
		cerr << "Usage " << argv[0] << " -i <plik_wejsciowy> -o <plik_wyjsciowy>\n";
		return EXIT_FAILURE;
	}

	return dzialaj(p);
}