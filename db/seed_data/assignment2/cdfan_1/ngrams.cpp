// This is the CPP file that implements ngrams.
// Given a file and the number of n, it creates a data structure in HashMap that associate prefixes and suffixes
// Given a number of words, it will randomly generate a text based on the style of writing of original text.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "random.h"
#include "hashmap.h"
#include "vector.h"

using namespace std;

//prototypes:
void welcomeMsg();
void readFileToMap(HashMap<Vector<string>, Vector<string> > & prefixSuffix);
void addToMap(HashMap<Vector<string>, Vector<string> > & prefixSuffix, Vector<string> & window, string word);
void generateText(HashMap<Vector<string>, Vector<string> > & prefixSuffix);
string windowToString(Vector<string> window);
void shiftWindow(Vector<string> & window, string word);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    welcomeMsg();
    HashMap<Vector<string>, Vector<string> > prefixSuffix;
    readFileToMap(prefixSuffix);
    generateText(prefixSuffix);

    cout << "Exiting." << endl;
    return 0;
}

void welcomeMsg() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups "<< endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

// creates a ifstream, asks for file name and the value of n from the user.
void readFileToMap(HashMap<Vector<string>, Vector<string> > & prefixSuffix) {
    ifstream input;
    promptUserForFile(input, "Input file name? ");
    int n = getInteger("Value of N? ");
    while(n<2) {
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N? ");
    }

    Vector<string> window;
    Vector<string> wrapAround;
    string word;

    // process the first n words of the file. and add these words to HashMap according to the rule of prefix and suffix
    //It also stores them in a vector for later use in wrapping around.
    for(int i=0; i< n; i++) {
        input >> word;
        wrapAround.add(word);

        if(window.size() < n-1) {
            window.add(word);
            continue;
        }
        addToMap(prefixSuffix, window, word);
    }

    //Reads the rest of the file word by word and add them to map
    while(true) {
        input >> word;
        if(input.fail()) break;
        addToMap(prefixSuffix, window, word);
    }

    // Process the wrapping around part :
    //N more iterations to connect the last n prefixes in the end of the file to the first n words at the start of the file
    for(int i=0; i< wrapAround.size(); i++) {
        word = wrapAround[i];
        addToMap(prefixSuffix, window, word);
    }
}

// creates a new vector "prefix" and assign it to the same contents of the window given. Add it as a key to the map, and
//add the given word to the value vector of the map assocaited with the prefix
void addToMap(HashMap<Vector<string>, Vector<string> > & prefixSuffix, Vector<string> & window, string word) {
    Vector<string> prefix = window;
    prefixSuffix[prefix].add(word);
    shiftWindow(window, word);
}

//It shift the window by removing the elment at the front of vector, and append the given word to its end
void shiftWindow(Vector<string> & window, string word) {
    window.remove(0);
    window.add(word);
}

//Randomly generate the text in the number of words required by the user. The random generation was done by ramdomly
//select the first prefix and then randomly selecte the weighted suffix based on the style of orgininal text. This is
//accomplished through the HashMap built when reading the original file.
void generateText(HashMap<Vector<string>, Vector<string> > & prefixSuffix) {
    string text = "... ";
    while (true) {
        cout << ""  << endl;
        int numWords = getInteger("# of random words to generate (0 to quit)? ");
        if(numWords == 0) break;
        if(numWords < 4) {
            cout << "Must be at least 4 words." << endl;
            continue;
        }

        Vector<string> window;
        //the first window is a key selected from the keyset of the HashMap
        window = prefixSuffix.keys()[randomInteger(0,prefixSuffix.size()-1)];
        string firstPrefix = windowToString(window);
        text += firstPrefix;

        //Randomly selects the suffix given a window. Afterwards, shift the window for next iteraction
        for(int i=0; i<numWords-window.size(); i++) {
            Vector<string> value = prefixSuffix[window];
            string suffix = value[randomInteger(0,value.size()-1)];
            text += suffix + " ";
            shiftWindow(window, suffix);
        }
        text += "...";
        cout << text << endl;
        text = "... ";
    }
}

//Change the Vector<string>into a string line of words, and return it to the caller.
string windowToString(Vector<string> window) {
    string prefix;
    for(int i=0; i<window.size(); i++) {
       prefix += window[i]+ " ";
    }
    return prefix;
}


