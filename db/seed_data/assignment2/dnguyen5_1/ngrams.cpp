// This program makes random text based on a document. After the user selects
// a file, an 'N' value for groups of words, and a number of words to randomly
// generate, it creates random text.


#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <string>
#include "console.h"
#include "hashset.h"
#include "set.h"
#include "random.h"
#include "queue.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"



using namespace std;

void introStatement();
HashMap<Queue<string>, Vector<string>> createMap(string fileName, int nValue);
void randomlyGenerate(int nValue, int numWords, HashMap<Queue<string>, Vector<string>> &textMap);
string getFileName();
int getRandomWordCount(int nValue);
int getNValue();

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    introStatement();
    string fileName = getFileName();
    int nValue = getNValue();
    HashMap<Queue<string>, Vector<string>> textMap = createMap(fileName, nValue);
    while (true) {
        int numRandomWords = getRandomWordCount(nValue);
        if (numRandomWords == 0) break;
        randomlyGenerate(nValue, numRandomWords, textMap);
    }
    cout << "Exiting." << endl;
    return 0;
}

/*Prompts user for a filename. Prompts user if file is unopenable. Returns
filename as string*/
string getFileName() {
    while (true) {
        ifstream input;
        string fileName = getLine("Input file name? ");
        if (fileExists(fileName)) {
            return fileName;
            break;
        }
        if (!fileExists(fileName)) {
            cout << "Unable to open that file.  Try again." << endl;
        }
    }
}

/*Prompts user for an N-value. Prompts user if N-value entered is less
 * than 2. Returns int*/
int getNValue() {
    while (true) {
        int nValue = getInteger("Value of N? ");
        if (nValue < 2) {
            cout << "N must be 2 or greater." << endl;
        }
        else {
            cout << endl;
            return nValue;
            break;
        }
    }
}

/*Prompts user for how many words to be generated. Reprompts user if random value
 * is less than N-value. returns int*/
int getRandomWordCount(int nValue) {
    while (true) {
        int numRandomWords = getInteger("# of random words to generate (0 to quit)? ");
        if (numRandomWords >= nValue || numRandomWords ==0) {
            return numRandomWords;
            break;
        }
        else {
            cout << "Must be at least " << nValue << " words." << endl;
            cout << endl;
        }
    }
}

/*Creates a hashmap of a file (taken in as parameter) from prefixes to suffixes. Takes in nValue
 * as a parameter*/
HashMap<Queue<string>, Vector<string>> createMap(string fileName, int nValue) {
    HashMap<Queue<string>, Vector<string>> map;
    Queue<string> window;
    ifstream input;
    input.open(fileName.c_str());
    string next;
    for (int i = 0; i < nValue; i++) {
        input >> next;
        window.enqueue(next);
}

Queue<string> firstWindow = window;
while (true) {
input >> next;   // read one word
if (input.fail()) {
    for (int i = 0; i < nValue; i++) {
        string nextLooped = firstWindow.dequeue();
        map[window] += nextLooped;
        window.dequeue();
        window.enqueue(nextLooped);
    }
    break;
}
map[window] += next;
window.dequeue();
window.enqueue(next);
}
return map;
}

/*Randomly generates words from the given HashMap passed in as a parameter. Generates numWords number of
 words.*/
void randomlyGenerate(int nValue, int numWords, HashMap<Queue<string>, Vector<string>> &textMap) {
    Vector<Queue<string>> keys = textMap.keys();
    Queue<string> randomWindow = keys.get(randomInteger(0, keys.size()-1)); //selects a random window from keys
    Vector<string> suffixes = textMap[randomWindow]; //retrieves vector of potential suffixes following random window
    string chosenSuffix = suffixes.get(randomInteger(0, suffixes.size()-1)); //chooses a random suffix
    cout <<"...";
    for (int i = 0; i < randomWindow.size(); i++) {
        string displayWord = randomWindow.dequeue();
        cout << displayWord << " ";
        randomWindow.enqueue(displayWord);
    }
    for (int i = 0; i < numWords-nValue; i++) {
        randomWindow.dequeue();
        randomWindow.enqueue(chosenSuffix);
        suffixes = textMap[randomWindow];
        chosenSuffix = suffixes.get(randomInteger(0, suffixes.size()-1));
        cout << chosenSuffix << " ";
    }
    cout << "..." << endl;
    cout << endl;
}


/*Prints introductory statements (including instructions) to console*/
void introStatement() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}
