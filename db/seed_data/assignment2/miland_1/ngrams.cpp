/**
 * Name: Milan Doshi
 * Assignment: ngramsr.cpp
 * TA: Jimmy Lee
 *
 * This program generates random text based on a file inputed and the number of grams inputed for
 * which the user wants the text to be manipulated with.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "hashmap.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

void printMessage();
void readFile();
void generateRandomText();
int checkNumberOfWords();

HashMap<Vector<string>, Vector<string> > ngrams;
Vector<string>fileWords;
Vector<string>textKey;
int numberOfGrams;

int main() {
    printMessage();
    readFile();
    generateRandomText();
    return 0;
}

/**
 * method: printMessage()
 * return: void
 * This method prints out the initial message the user will see when they start the program.
 */
void printMessage() {
    setConsoleSize(750, 450);
    setConsoleFont("Courier New-16");
    setConsoleEcho(true);

    cout << "Welcome to CS 106B Random Writer (aka 'N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll generate random text for you." << endl;
}

/**
 * method: readFile()
 * return: void
 * This method reads in the file which will be used to generate random text. The file is read in based
 * on the N (number of grams) value for the number of N-Grams. The N value is the number used to determine
 * how many words will be allocated in each key of the collection. This method also checks for certain
 * user input errors.
 */
void readFile() {

    string word;
    string nextWord;
    Vector<string> temp;

    //this segment of code takes in a valid filename
    string filename = getLine("Input file name?");
    while(fileExists(filename) == false) {
        cout << "Please enter a valid filename/type." << endl;
        filename = getLine("Input file name?");
    }

    //this segment of code determines the N value which is used through the entirety of the program
    numberOfGrams = getInteger("Value of N?");
    while(numberOfGrams < 2) {
        cout << "Please pick a value for N greater than 1." << endl;
        numberOfGrams = getInteger("Value of N?");
    }

    //all the words in the file are read into a single vector
    ifstream input;
    input.open(filename.c_str());
    while(!input.eof()) {
        input >> word;
        fileWords += word;
    }

    //the hashmap is created for ngrams based on the single vector into which all the words
    //in the file were stored
    for(int i = 0; i < fileWords.size(); i++) {
        if (temp.isEmpty()) {
            for(int j = 0; j < numberOfGrams-1; j++) {
                word = fileWords[j];
                temp += word;
            }
            Vector<string> firstkey;
            nextWord = fileWords[(i+numberOfGrams-1)%fileWords.size()];
            firstkey += nextWord;
            ngrams[temp] = firstkey;
            temp.add(nextWord);
        } else {
            temp.remove(0);
            nextWord = fileWords[(i+numberOfGrams-1)%fileWords.size()];
            if (ngrams.containsKey(temp)) {
                ngrams[temp] += nextWord;
            } else {
                Vector<string> keys;
                keys += nextWord;
                ngrams[temp] = keys;
            }
            temp += nextWord;
        }
    }
}

/**
 * method: generateRandomText()
 * return: void
 * This method generates random text based on the N value which was entered earlier in the program. Based
 * on the number of words the user would like to generate, this method uses the N value in combination
 * with the number of words the user wants to generate to print out the final message, completely random.
 */
void generateRandomText() {
    string randomText;
    int numberOfWords = checkNumberOfWords();
    int randomPrefix = randomInteger(0, ngrams.size() - 1);
    Vector<Vector<string> > allKeys = ngrams.keys();
    while(numberOfWords != 0) {
        textKey = allKeys[randomPrefix];
        for(int i = 0; i < numberOfWords; i++) {
            randomText += textKey.get(0) + " ";
            string nextKey = ngrams[textKey].get(randomInteger(0, ngrams[textKey].size() - 1));
            textKey += nextKey;
            textKey.remove(0);
        }
        cout << "... " << randomText << "..." << endl;
        randomText = "";
        numberOfWords = checkNumberOfWords();
        randomPrefix = randomInteger(0, ngrams.size() - 1);
    }
}

/**
 * method: checkNumberOfWords()
 * return: int
 * This method asks the user for the number of words they want to generate. However, they must enter
 * a number at least the size of N (the number of grams), unless the input is 0. If the user enters 0,
 * the program will exit.
 */
int checkNumberOfWords() {
    int numberOfWords = getInteger("# of random words to generate(0 to quit)?");
    if (numberOfWords == 0) {
        cout << "Exiting." << endl;
    } else {
        while (numberOfWords < numberOfGrams && numberOfWords != 0) {
            cout << "Please enter a number of words to be generated larger than or equal to N." << endl;
            numberOfWords = getInteger("# of random words to generate(0 to quit)?");
            if (numberOfWords == 0) {
                cout << "Exiting." << endl;
            }
        }
    }
    return numberOfWords;
}




