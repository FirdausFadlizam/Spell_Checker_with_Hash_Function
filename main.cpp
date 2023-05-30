#include "SpellChecker.h"
#include "QuadraticProbing.h"
#include <iostream>
using namespace std;


int main(int argc, char* argv[]) {

	SpellChecker text;
	text.createDictionary(atoi(argv[1]));
	text.fillDictionary(string(argv[2]));
	text.readText(string(argv[3]));
	text.printWord();
	text.hashStats();
	

}