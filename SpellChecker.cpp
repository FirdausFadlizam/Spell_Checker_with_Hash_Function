#include "SpellChecker.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

SpellChecker::SpellChecker(){

	//initialize array pointers
	wordSaved = new incorrectWord[1000];
	for (int i = 0; i < 1000; i++) {
		wordSaved[i].word = "EMPTY";
		wordSaved[i].correction = NULL;
	}
	maxWords = 1000;
	count = 0;
}

void SpellChecker::createDictionary(int words) {
	
	//initialize hashtable
	dictionary = HashTable<string>(2 * words);
	

}

void SpellChecker::fillDictionary(string fileName) {

	
	string word;
	ifstream input;

	input.open(fileName);

	while (input >> word) {
		dictionary.insert(word);
		
	}

	input.close();

}

void SpellChecker::readText(string fileName) {
	
	ifstream input;
	string lineStr;
	string tempWord;
	int lineNumber = 0;

	input.open(fileName);

	while (getline(input, lineStr)) {

		++lineNumber;
		//remove punctuations
		for (int i = 0; i < lineStr.length(); i++) {
			if (!isalpha(lineStr[i]))
				lineStr[i] = ' ';
		}

		//copy each word from a line
	
		istringstream extractWord(lineStr);

		while (extractWord >> tempWord) {
		
			if (!dictionary.contains(tempWord)){

				//Change upper-case to lower-case letter
				for (int i = 0; i < tempWord.size(); i++) {
					tempWord[i] = tolower(tempWord[i]);
				}

				if (!dictionary.contains(tempWord)) {

					if (count == maxWords) {
						increaseWordList();
					}
					
						++count;
						wordSaved[count - 1].word = tempWord;
						wordSaved[count - 1].line = lineNumber;
						wordSaved[count - 1].correction = spellingCheck(tempWord);

					
				}
			
				
			}
		}


	}

	input.close();
}

void SpellChecker::increaseWordList() {

	//temporarily holds array elements
	incorrectWord* wordTemp = new incorrectWord [maxWords];
	for (int i = 0; i < maxWords; i++) {
		wordTemp[i] = wordSaved[i];
	}

	//change array size
	wordSaved = new incorrectWord[maxWords * 2];

	//return elements to the original variable
	for (int i = 0; i < maxWords; i++) {
		wordSaved[i] = wordTemp[i];
	}


}

incorrectWord* SpellChecker::spellingCheck(string word) {

	//head of pointer - to be returned
	incorrectWord* head = NULL;
	//delete a letter 
	for (int i = 0; i < word.length(); i++) {

		//remove a letter by copying the word without it 
		string wordCheck = word.substr(0, i) + word.substr(i + 1);

		if (dictionary.contains(wordCheck)) { 
			//assign to head if NULL
			
			if (head == NULL) {
				head = new incorrectWord;
				head->word = wordCheck;
				head->correction = NULL;
			}

			//assign to the next pointer in the list
			else {
				incorrectWord* temp = head;

				while (temp->correction != NULL) {
					temp = temp->correction;
				}

				incorrectWord* tempNew = new incorrectWord;
				tempNew->word = wordCheck;
				tempNew->correction = NULL;
				temp->correction = tempNew;
			}
		}
	}

	//insert one letter
	for (int i = 0; i < word.length() + 1; i++) {
		for (char j = 'a'; j <= 'z'; j++) {

			//add a letter in a specific position
			string wordCheck = word.substr(0, i) + j + word.substr(i);
			
			if (dictionary.contains(wordCheck)) {

				if (head == NULL) {
					head = new incorrectWord;
					head->word = wordCheck;
					head->correction = NULL;
				}

				else {
					incorrectWord* temp = head;
					while (temp->correction != NULL) {
						temp = temp->correction;
					}

					incorrectWord* tempNew = new incorrectWord;
					tempNew->word = wordCheck;
					tempNew->correction = NULL;
					temp->correction = tempNew;
				}
			}
		}
	}
	
	//swap characters
	for (int i = 1 ; i < word.length() + 1; i++) {

			string wordCheck = word;

			//swap a letter with the previous letter
			swap(wordCheck[i - 1], wordCheck[i]);

			if (dictionary.contains(wordCheck)) {

				if (head == NULL) {
					head = new incorrectWord;
					head->word = wordCheck;
					head->correction = NULL;
				}

				else {
					incorrectWord* temp = head;
					while (temp->correction != NULL) {
						temp = temp->correction;
					}

					incorrectWord* tempNew = new incorrectWord;
					tempNew->word = wordCheck;
					tempNew->correction = NULL;
					temp->correction = tempNew;
				}
			}
		
	}

	//replace letter

	for (int i = 0; i < word.length(); i++) {
		for (char j = 'a'; j <= 'z'; j++) {
			string wordCheck = word;
			wordCheck[i] = j;
			if (dictionary.contains(wordCheck)) {

				if (head == NULL) {
					head = new incorrectWord;
					head->word = wordCheck;
					head->correction = NULL;
				}

				else {
					incorrectWord* temp = head;
					while (temp->correction != NULL) {
						temp = temp->correction;
					}

					incorrectWord* tempNew = new incorrectWord;
					tempNew->word = wordCheck;
					tempNew->correction = NULL;
					temp->correction = tempNew;
				}
			}
		}
	}

	return head;
}

void SpellChecker::printWord() {

	for (int i = 0; i < count; i++) {
		cout << wordSaved[i].word << "("<< wordSaved[i].line << "): ";
		
		incorrectWord* nextWord = wordSaved[i].correction;
		while ( nextWord != NULL) {
			cout << nextWord->word << " ";
			nextWord = nextWord->correction;
		}

		cout << endl;

	}
}

void SpellChecker::hashStats() {
	
	    cerr << "Number of words " << dictionary.getTotalObj()<< ", Table size " << dictionary.getTableSize() << ", Load factor " << dictionary.getLoadFactor() << endl;
		cerr << "Collisions " << dictionary.getTotalCollision() << ", Average chain length " << dictionary.getAverageChain() << ", Longest chain length " << dictionary.getLongestCollision() <<  endl;
}

SpellChecker::~SpellChecker() {
	
	for (int i = 0; i < count; i++) {
		incorrectWord* temp = wordSaved[i].correction;
		incorrectWord* deleteWord;
		while (temp != NULL) {
			deleteWord = temp;
			temp = temp->correction;
			delete deleteWord;
		}
	}

}