/*
 * Emma Daugherty; CS106B; Section Leader: Jimmy Lee
 *
 * This program is called N-grams. It reads a data file of the users choice, and allows the user to choose a value
 * 'N' for how large groups of words are. Based on 'N', random text is generated that mimics the original text.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "hashmap.h"
#include "vector.h"
#include "random.h"

using namespace std;

//Methods
void printIntro();
void readFile(string filename, int n, HashMap<Vector<string>, Vector<string> >& map);
void generateText(HashMap<Vector<string>, Vector<string> >& map, int numWords);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    printIntro();

    string filename = getLine("Input file name? ");
    while (!fileExists(filename)) {
        cout << "Unable to open that file. Try again." << endl;
        filename = getLine("Input file name? ");
    }

    int n = -1;
    while (n < 2) {
        n = getInteger("Value of N? ");
    }

    HashMap<Vector<string>, Vector<string> > map;
    readFile(filename, n, map);

    int numWords = -1;
    while (true) {
        if (numWords < n) {
            cout << "" << endl;
            numWords = getInteger("# of random words to generate (0 to quit)? ");
        }
        if (numWords == 0) break;
        generateText(map, numWords);
        numWords = -1;
    }

    cout << "Exiting." << endl;
    return 0;
}

/*
 * This method prints an introduction to the program, explaining the rules to the user.
 */
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

/*
 * This method reads the file and appropriately stores groups of words based on 'N'.
 */
void readFile(string filename, int n, HashMap<Vector<string>, Vector<string> >& map) {
    Vector<string> window;
    Vector<string> firstWords;
    ifstream input;
    input.open(filename);
    for (int i = 0; i < (n - 1); i++) {
        string word;
        input >> word;
        if (input.fail()) {
            break;
        }
        window.add(word);
        firstWords.add(word);
    }
    while (true) {
        string word;
        input >> word;
        if (input.fail()) {
            break;
        }
        if (map.containsKey(window)) {
            Vector<string> temp = map.get(window);
            temp.add(word);
            map.put(window, temp);
        }
        else {
            Vector<string> temp;
            temp.add(word);
            map.put(window, temp);
        }

        window.remove(0);
        window.add(word);
    }

    for (int k = 0; k < (n - 1); k++) {
        if (map.containsKey(window)) {
            Vector<string> temp = map.get(window);
            temp.add(firstWords.get(k));
            map.put(window, temp);
        }
        else {
            Vector<string> temp;
            temp.add(firstWords.get(k));
            map.put(window, temp);
        }

        window.remove(0);
        window.add(firstWords.get(k));
    }

}

/*
 * This method randomly generates text. It generates a specified number of words based on user input.
 */
void generateText(HashMap<Vector<string>, Vector<string> >& map, int numWords) {
    int size = map.size();
    int r = randomInteger(0, size - 1);
    string result = "";
    Vector<Vector<string> > keys = map.keys();
    Vector<string> window = keys.get(r);
    Vector<string> values = map.get(window);
    for (int k = 0; k < window.size() - 1; k++) {
        result += window.get(k) + " ";
    }
    for (int i = 0; i < numWords - (window.size() - 1); i++) {
        result += window.get(window.size() - 1) + " ";
        int x = randomInteger(0, values.size() - 1);
        string next = values.get(x);
        window.remove(0);
        window.add(next);
        values = map.get(window);
    }

    cout << "..." + result + "...";
    cout << endl;
}





