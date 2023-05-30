#pragma once
#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <iostream>
#include "QuadraticProbing.h"
using namespace std;

// SpellChecker class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// void createDictionary  --> Create hash table
// void fillDictionary(string x) --> Fill in hash table 
// void readText(string x) --> Read text into program
// void increaseWordList(); --> Increase word list
// incorrectWord* spellingCheck(string x) --> Return all possible corrections
// void printWord(); --> Print word list
// void hashStats(); --> Print stats

struct incorrectWord {
	string word;
	int line;
	incorrectWord* correction;

};

class SpellChecker {

private: 
	HashTable<string>dictionary;
	struct incorrectWord* wordSaved;
	int count;
	int maxWords;

public:

	SpellChecker();
	void createDictionary(int totalWords);
	void fillDictionary(string fileName);
	void readText(string fileName);
	void increaseWordList();
	incorrectWord* spellingCheck(string word);
	void printWord();
	void hashStats();
	~SpellChecker();
	
};







#endif