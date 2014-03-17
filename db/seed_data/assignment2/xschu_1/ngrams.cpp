/*
 * Xinsheng Chu
 * This program generates random text from a input txt file.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "console.h"
#include "random.h"
#include "simpio.h"
#include "filelib.h"
#include "map.h"
#include "vector.h"
#include "strlib.h"
#include "queue.h"
#include "hashmap.h"

using namespace std;

/*
 * This function displays the initial message on the console.
 */
void displayMessage() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

/*
 * This function prompts the user to input the file name and return the valid txt name.
 * If the file name is not valid, it re-prompts the user for input.
 */
string getFileName() {
    string name = getLine("Input file name? ");
    while (!fileExists(name)) {
        cout << "Unable to open that file. Try again." << endl;
        name = getLine("Input file name? ");
    }
    return name;
}

/*
 * This function prompts user to input the value of N, and return the valid number of N.
 * If user input is less than 2, it re-prompts the user to enter the number.
 */
int getN() {
    int number = getInteger("Value of N? ");
    while (number < 2) {
        cout << "N must be 2 or greater." << endl;
        number = getInteger("Value of N? ");
    }
    return number;
}

/*
 * This function prompts user to input the number of words to n, and return the number.
 * It uses the number of N as parameter.
 * If user input is less than 0 or less than N - 1, it re-prompts the user to enter the number.
 */
int getWordNum(int num) {
    int wordNumber = getInteger("# of random words to generate (0 to quit)? ");
    while ((wordNumber > 0 && wordNumber < num - 1) || wordNumber < 0) {
        cout << "Must be at least " << num - 1 << " words." << endl;
        cout << "" << endl;
        wordNumber = getInteger("# of random words to generate (0 to quit)? ");
    }
    return wordNumber;
}

/*
 * This function takes in the input file and reads in all the words in that txt file.
 * It stores all the words in a vector of string, and passes it by reference.
 * It is called by method buildMap.
 */
void buildWordVector(ifstream& input, Vector<string>& wordVector) {
    string word;
    while (input >> word) {
        wordVector.add(word);
    }
}

/*
 * This function generates the first window.
 * It takes in the vector of string of all the words, and N.
 * It adds N words to the vector of string and passes it by reference.
 * It is called by method buildMap.
 */
void initialWindow(Vector<string>& vectorOfWord, int number, Vector<string>& firstWindow) {
    for (int i = 0; i < number - 1; i++) {
        firstWindow.add(vectorOfWord[i]);
    }
}

/*
 * This method builds the hashmap, which uses the vector of strings as keys, and another vector of strings as values.
 * It also takes in the input file and integer N.
 * It loops over the vector of strings of total word to get N - 1 word and put those into vector of strings of windows.
 * It also wraps around.
 * If same windows are found, both values are added to the values.
 */
void buildMap(HashMap<Vector<string>, Vector<string> >& wordMap, ifstream& input, int num) {
    Vector<string> totalWord;
    buildWordVector(input, totalWord);
    Vector<string> window;
    initialWindow(totalWord, num, window);
    Vector<string> nextRandomWord;

    for (int i = num; i <= totalWord.size(); i++) {
        if (wordMap.containsKey(window)) {
            nextRandomWord = wordMap.get(window);
        } else {
            nextRandomWord.clear();
        }
        nextRandomWord.add(totalWord[i-1]);
        wordMap.put(window, nextRandomWord);
        window.add(totalWord[i-1]);
        window.remove(0);
    }

    // wrap around
    for (int i = 1; i < num; i++) {
        if (wordMap.containsKey(window)) {
            nextRandomWord = wordMap.get(window);
        } else {
            nextRandomWord.clear();
        }
        nextRandomWord.add(totalWord[i-1]);
        wordMap.put(window, nextRandomWord);
        window.add(totalWord[i-1]);
        window.remove(0);
    }
}

/*
 * This function generates the random starting window of text.
 * It takes in the map and vector of string of starting text.
 * It get all the keys in the map, stores them in a vector and get a random index to get the random starting window.
 */

void generateStartingText(HashMap<Vector<string>, Vector<string> >& ngramMap, Vector<string>& startingText ) {
    Vector<Vector<string> > allKeys = ngramMap.keys();
    //cout << allKeys.toString() << endl;
    int r = randomInteger(0, allKeys.size()-1);
    startingText = allKeys.get(r);
}

/*
 * This function generates N prefix words from the existing random text.
 * It takes vector of strings of random text, and N as input.
 * For example, if the random text is "to be or not to be", and N = 3, it generates the prefix to be "to be", which can be used as the key to search for suffix in next iteration.
 */
void getPrefix(Vector<string>& randomText, int n, Vector<string>& prefix) {
    for (int i = 0; i < n - 1; i++) {
        prefix.add(randomText[randomText.size() - n + 1 + i]);
    }
}

/*
 * This function generates the random text.
 * It calls the generateStartingText and getPrefix function.
 * It takes in the map, N and number of words to generate, and random text as input.
 * By gettting the starting prefix, the suffix is get and added to random text.
 * It does that by iteration to get the desired number of words.
 */
void generateText(HashMap<Vector<string>, Vector<string> >& ngramMap, int n, int wordNum, Vector<string>& randomText) {
    Vector<string> startingText;
    generateStartingText(ngramMap, startingText);
    randomText = startingText;
    Vector<string> suffixVector = ngramMap.get(startingText);
    int r = randomInteger(0, suffixVector.size()-1);
    string suffix = suffixVector.get(r);
    randomText.add(suffix);
    for (int i = n; i < wordNum; i++) {
        Vector<string> prefix;
        getPrefix(randomText, n, prefix);
        suffixVector = ngramMap.get(prefix);
        r = randomInteger(0, suffixVector.size()-1);
        suffix = suffixVector.get(r);
        randomText.add(suffix);
    }
}

/*
 * This method takes in the vector of strings of random text generated and prints out the text.
 * It add ... to both beginning and end of the text.
 */
void displayRandomText(Vector<string>& randomText) {
    cout << "... ";
    for (int i = 0; i < randomText.size(); i++) {
        cout << randomText[i] << " ";
    }
    cout << "..." << endl;
    cout << endl;
}

/*
 * This main function asks user to input the file name, and open the file
 * and calls all functions including buildMap, generateText, displayRandomText.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    displayMessage();
    ifstream inputFile;
    string fileName = getFileName();
    openFile(inputFile, fileName);

    int n = getN();
    int wordNum = getWordNum(n);
    while (wordNum != 0) {
         HashMap<Vector<string>, Vector<string> > ngramMap;
         buildMap(ngramMap, inputFile, n);
         Vector<string> randomText;
         generateText(ngramMap, n, wordNum, randomText);
         displayRandomText(randomText);
         wordNum = getWordNum(n);
    }
    cout << "Exiting." << endl;
    return 0;
}
