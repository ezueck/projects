/*
 * Project1.cpp
 * 
 * Name: Eduardo Zueck Garces
 * UTEID: ez2959
 * EE312 Spring 2016
 * SpellCheck
 */

#include <stdio.h> // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types

/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
 *
 * Many students find it helpful to declare global variables (often arrays). You are welcome to use
 * globals if you find them helfpul. Global variables are by no means necessary for this project.
 */

/* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
 * exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
 * You are expected to use reasonable programming style. I *insist* that you indent 
 * reasonably and consistently in your code. I strongly encourage you to avoid big functions
 * So, plan on implementing spellCheck by writing two or three other "support functions" that
 * help make the actual spell checking easier for you.
 * There are no explicit restictions on using functions from the C standard library. However,
 * for this project you should avoid using functionality from the C++ standard libary. You will
 * almost certainly find it easiest to just write everything you need from scratch!
 */
int checkIfLetter(char letter);
void wordPrinter(char article[], int first, int last);
int * wordSeparator(char article[], char dictionary[], int i);
void wordPrinter(char article[], int first, int last);
void checkForMatch(char article[], char dictionary[], int first, int last);
int letterMatching(char article, char dictionary);

void spellCheck(char article[], char dictionary[]) {

	int *wordPointer;		//Holds the information of the word being checked. See wordSeparator for more info
	int currentLetter = 0;  //Character currently being checked 
	int first = 0;			//First character of a word
	int last = 0;			//Last character of a word 

	while (article[currentLetter] != 0) {
		wordPointer = wordSeparator(article, dictionary, currentLetter);
		currentLetter = *(wordPointer + 2); 
		if (*(wordPointer + 3) == 1) {		//If it is a word, check for match 
			checkForMatch(article, dictionary, *(wordPointer), *(wordPointer + 1));
		}
	}
}


/*
*  This function identifies wheter something is a word or not
*  It returns an array of results via a pointer 
*  [0]: Position of first letter of the word
*  [1]: Position of last letter of the word 
*  [2]: Next position to check 
*  [3]: True/False for finding a word or not
*/
int * wordSeparator(char article[], char dictionary[], int i) {
	int resultArray[4] = { 0,0,0,0 };
	if (checkIfLetter(article[i]) && checkIfLetter(article[i + 1])) {
		resultArray[3] = 1;
		resultArray[0] = i;
		i = i + 2;
		while (checkIfLetter(article[i])) {
			i++;
		}
		resultArray[1] = i-1;
	}
	else {
		resultArray[3] = 0;
	}
	resultArray[2]=i+1;
	return resultArray;
}

/*
*  This function checks if a character is a letter or not 
*  Returns True or False 
*/
int checkIfLetter(char letter) {
	if ((letter > 64 && letter < 91) || (letter > 96 && letter < 123)) {
		return 1;
	}
	else {
		return 0;
	}
}

/*
*  This function searches a word from the array in the dictionary 
*  The inputs are both arrays and the position of the article word 
*/
void checkForMatch(char article[], char dictionary[], int first, int last) {
	int matches = 0; //Counter for letters that match between the article/dictionary words
	int dictionaryCounter = 0;

	while ( dictionary[dictionaryCounter]!=0) {
		if (letterMatching(article[first+matches], dictionary[dictionaryCounter])) { //Checks if letters match
			matches++;
			dictionaryCounter++;
		}
		else { // If letters don't match, jump to the next one
			while (dictionary[dictionaryCounter] != '\n') {
				dictionaryCounter++;
			}
		}
		if ((dictionary[dictionaryCounter] == '\n') && (matches < (last - first + 1))) { 
			matches = 0; //Reset the matches counter to check a new word 
			dictionaryCounter++;
		}
		else if (matches == (last - first + 1)) { //If the whole word matches FULLY, return 
			if ((dictionary[dictionaryCounter] == '\n') || (dictionary[dictionaryCounter] == 0)) {
				return;
			}
			matches = 0;
		}
	}
	wordPrinter(article, first, last); //There was no match, so print the word
}

/*
*  Crosscheckes if a letter in the article and dictionary are the same
*  Makes sure different capitalizations don't affect the matching 
*/
int letterMatching(char article, char dictionary) {
	if (article == dictionary) { 
		return 1; //They are the same character 
	}
	else if ((article == (dictionary + 32)) && ((dictionary + 32) < 123)) { 
		return 1; //Article = lowercase, Dictionary = uppercase
	}
	else if ((article == (dictionary - 32)) && ((dictionary - 32) > 64)) {
		return 1; //Article = uppercase, Dictionary = lowercase
	}
	else {
		return 0; //No match
	}
}

/*
*  The function prints a word from the article array
*/
void wordPrinter(char article[], int first, int last) {
	while (first <= last) {
		printf("%c", article[first]);
		first++;
	}
	printf("\n");
}