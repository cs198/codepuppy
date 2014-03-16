/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: NGrams
 * This program intakes a file and then randomly generates
 * text based on the probability of
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
void intro();
HashMap<string, Vector<string> > mapFile(ifstream&, int);
void addToMap(HashMap<string, Vector<string> >&, string, string);
string randomText(ifstream&, int, int);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    intro();
        cout << endl;
        cout << "Input file? ";
        string filename;
        cin >> filename;
        cout << "Value of N? ";
        int nGram;
        cin >> nGram;
        cout << endl;
        cout << "# of random words to generate (0 to quit)? ";
        int number;

        cin >> number;
        ifstream input;
        input.open(filename.c_str());

        string output;

        // Loops through asking the user how much to generate until they enter 0
        while(number != 0) {
            ifstream input;
            input.open(filename.c_str());
            output = randomText(input, number, nGram);
            cout << output << endl;
            cout<< endl;
            cout << "# of random words to generate (0 to quit)? ";
            cin >> number;
            if (number == 0) {
                cout << "Exiting." << endl;
            }
        }

        return 0;
    }

    /*
     * Prints the introductory message to the program.
     */
    void intro() {
        cout << "Welcome to CS106B Random Writer (aka 'N-Grams')." << endl;
        cout << "This program makes random text based on a document." << endl;
        cout << "Give me an input file and an 'N' value for groups" << endl;
        cout << "of words, and I'll generate random text for you." << endl;
    }

    /*
     * Chooses a random point in the document and generates random
     * text according to how many words and NGrams the user requested.
     */
    string randomText(ifstream& input, int number, int nGram) {
        HashMap<string, Vector<string> > textMap = mapFile(input, nGram);
        string output = "... ";
        string randomWord;

        //chooses a random starting point for the document
        int mapSize = textMap.size();
        int startingPoint = randomInteger(0,mapSize);
        int counter = 0;
        string startingWord;
        for(string keys: textMap) {
            counter++;
            if (counter == startingPoint) {
                startingWord = keys;
                break;
            }
        }

        //finds word in the map and builds random text
        for (int n = 0; n < number; n++) {
            Vector<string> tempVector = textMap.get(startingWord);
            int vectorSize = tempVector.size();
            int randomNum = randomInteger(0,vectorSize - 1);
            string temp = tempVector.get(randomNum);

            output = output + temp + " ";
            startingWord.erase(0,startingWord.find_first_of(" ") + 1);
            startingWord = startingWord + temp + " ";
        }
        return output + "...";
    }

    /*
     * Builds the map of prefixes and suffixes based on the text and
     * number of nGrams requested.
     */
    HashMap<string, Vector<string> > mapFile(ifstream& input, int nGram) {
        HashMap<string, Vector<string> > textMap;
        string key = "";
        string value;
        string temp;
        Vector<string> wrapVector;

        //to get the initial strand of words
        for (int n = 0; n < nGram - 1; n++) {
            input >> temp;
            key.append(temp + " ");
            wrapVector.add(temp);
        }

        while (true) {
            input >> value;
            if (input.fail()) {
                break;
            }
            if (key != "") addToMap(textMap, key, value);
            key.erase(0,key.find_first_of(" ") + 1);
            key = key + value + " ";
        }

        //for the extra iterations
        for (int n = 0; n < nGram - 1; n++) {
            value = wrapVector.get(n);
            addToMap(textMap, key, value);
            key.erase(0,key.find_first_of(" ") + 1);
            key = key + value + " ";
        }
        return textMap;
    }

    /*
     * Adds the keys and values to the map.
     */
    void addToMap(HashMap<string, Vector <string> >& textMap, string key, string value) {
        if (!textMap.containsKey(key)) {
            Vector<string> valuesVector;
            valuesVector.add(value);
            textMap.put(key, valuesVector);
        } else {
            Vector<string> tempVector = textMap.get(key);
            tempVector.add(value);
            textMap.put(key, tempVector);
        }
    }

