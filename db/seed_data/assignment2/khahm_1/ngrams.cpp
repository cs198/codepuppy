/*
 * Katie Hahm; section leader: Jimmy Lee
 * NGrams
 * This method asks the user for the file name,
 * number of words the file should be broken into,
 * and connects those words with the next probable word
 * from the file.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <istream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "hashmap.h"
#include "random.h"
using namespace std;

HashMap<Vector<string>, Vector<string> > mapNGram(ifstream& input,int nValue);
void generateText(HashMap<Vector<string>, Vector<string> >& nGramMap, int nValue, int generateNum);
Vector<string> getRandomStart(HashMap<Vector<string>, Vector<string> >& nGramMap);
string getRandomWord(Vector<string>& values);
string changeToString(Vector<string>& start);
Vector<string> getNewWindow(Vector<string>& start, string& nextWord);
void printIntro();
void getTextFile(ifstream& input);
void getNValue(int& nValue);
void getGenerateNum(HashMap<Vector<string>, Vector<string> > &nGramMap, int &nValue);
void addWindow(Vector<string> &window, string &word, HashMap<Vector<string>, Vector<string> > &nGramMap);

/*
 * prints the introductory messages
 * reads the input file from the user
 * obtains a valid nValue and valid number of words to generate
 * and produces the desired nGram.
 * It asks the user for another number of words to generate
 * until the user enters 0 to quit the program.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    ifstream input;
    getTextFile(input);
    int nValue;
    getNValue(nValue);
    HashMap<Vector<string>, Vector<string> > nGramMap = mapNGram(input, nValue);
    getGenerateNum(nGramMap, nValue);
    cout << "Exiting." << endl;
    return 0;
}

// prints the introductory messages
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

void getTextFile(ifstream& input) {
    string fileName = getLine("Input file name? ");
    while (!fileExists(fileName)) {
        cout << "Unable to open that file.  Try again." << endl;
        fileName = getLine("Input file name? ");
    }
    input.open(fileName.c_str());
}

/*
 * asks the user for a valid NValue
 * parameter: nValue used in the main method
 * the nValue is only valid when it is greater or equal to 2
 */
void getNValue(int& nValue) {
    nValue = getInteger("Value of N? ");
    while (nValue < 2) {
        cout << "N must be 2 or greater." << endl;
        nValue = getInteger("Value of N? ");
    }
    cout << endl;
}

/*
 * creates a hashmap of the window and its corresponding values
 * parameter: input, the n value
 * returns: a map containing the windows from the file as keys and its corresponding values
 * each value may have duplicates, since duplicates means that
 * those words are used more often after the unique window.
 */
HashMap<Vector<string>, Vector<string> > mapNGram (ifstream& input, int nValue) {
    HashMap<Vector<string>, Vector<string> > nGramMap;
    string word;
    Vector<string> window;
    while (window.size() < nValue-1) {
        input >> word;
        window.add(word);
    }
    Vector<string> firstWindow = window;
    while (input >> word) {
        addWindow(window,word,nGramMap);
    }
    for (int i = 0; i < firstWindow.size(); i++) {
        string word = firstWindow.get(i);
        addWindow(window, word, nGramMap);
    }
    return nGramMap;
}

/*
 * adds the given window to the map
 * parameter: Vector of strings to use as key for the map, the current map, and all the words used as a vector
 */
void addWindow(Vector<string> &window, string &word, HashMap<Vector<string>, Vector<string> > &nGramMap) {
    nGramMap[window] += word;
    window.remove(0);
    window.add(word);
}

/*
 * generates the nGram by asking the user for a number and generates the text
 * parameter: hashmap of vectors to vectors of each window to its value from the file
 * parameter: the nValue inputted by the user
 * asks for the number of random words to generate until the user gives a number
 * that is larger than the nValue inputted previously.
 * While the user doesn't input a 0 to quit the program,
 * it generates the nGram using the file and nValue and the current generate number
 * and displays the text.
 */
void getGenerateNum(HashMap<Vector<string>, Vector<string> >& nGramMap, int& nValue) {
    int generateNum = getInteger("# of random words to generate (0 to quit)? ");
    while (generateNum != 0) {
        while (generateNum < nValue && generateNum != 0) {
            cout << "Must be at least " << nValue << " words." << endl << endl;
            generateNum = getInteger("# of random words to generate (0 to quit)? ");
        }
        if (generateNum == 0) {
            break;
        }
        generateText(nGramMap, nValue, generateNum);
        cout << endl;
        generateNum = getInteger("# of random words to generate (0 to quit)? ");
    }
}

/*
 * From the map and given nValue and number of words to generate, it prints the ngram.
 * parameter: map of window and its values, nValue, and the number of words to generate
 * it picks a random window in the map,
 * and while the number of words printed are less than the given words to generate,
 * it selects a random word from its values, prints it, and moves the window.
 */
void generateText(HashMap<Vector<string>, Vector<string> >& nGramMap, int nValue, int generateNum) {
    Vector<string> start = getRandomStart(nGramMap);
    cout << "..." << changeToString(start);
    generateNum -= nValue - 1;
    while (generateNum > 0) {
        Vector<string> values = nGramMap.get(start);
        string nextWord = getRandomWord(values);
        start = getNewWindow(start, nextWord);
        nextWord += " ";
        cout << nextWord;
        generateNum -= 1;
    }
    cout << "..." << endl;
}

/*
 * Moves the window to the next increment
 * parameter: a vector of strings of the starting window, and the next selected word
 * returns: the new vector of string
 * it ignores the first word in the current window,
 * adds the next word,
 * and outputs the new vector
 */
Vector<string> getNewWindow(Vector<string>& start, string& nextWord) {
    start.remove(0);
    start.add(nextWord);
    return start;
}

/*
 * converts the given vector to a string
 * parameter: vector of strings
 * returns: the string to be printed
 */
string changeToString(Vector<string>& start) {
    string output = "";
    for (int i = 0; i < start.size(); i++) {
        output += start.get(i);
        output += " ";
    }
    return output;
}

/*
 * chooses a random window to begin the nGrams
 * parameter: map of windows to its values
 * returns: a vector of strings to start the nGrams
 */
Vector<string> getRandomStart(HashMap<Vector<string>, Vector<string> > &nGramMap) {
    Vector<Vector<string> > allKeys = nGramMap.keys();
    int windowLength = allKeys.size();
    int randomNum = randomInteger(0, windowLength-1);
    return allKeys.get(randomNum);
}

/*
 * Picks a random word from the given values
 * parameter: a vector of strings of values
 * returns: the string chosen randomly from the values to be the next word
 * it obtains the values corresponding to a certain window
 */
string getRandomWord(Vector<string>& values) {
    int valueLength = values.size();
    int randomNum = randomInteger(0, valueLength-1);
    return values.get(randomNum);
}
