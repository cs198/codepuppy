// Min Cheol Kim
// CS106B
// Section Leader Jimmy Lee
// Section time 5:15 Wed
// N-grams

/* The assignment was to write a program that takes in an input file and generates a data structure called
 * the N-grams, and use the newly created data structure to randomly generate new text that sounds similar
 * to what the author of the original file would say. */

#include <cctype>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "hashmap.h"
#include "vector.h"
#include "filelib.h"
#include "set.h"
#include "queue.h"
#include "random.h"
#include "simpio.h"

using namespace std;
void buildMap(HashMap <Queue<string>, Vector<string>>& nGramMap, string filename, int valueOfN);
void buildMapHelper(HashMap<Queue<string>, Vector<string> >& nGramMap, Queue<string>& window,
                    Vector<string>& text, int n);
string randomWordGenerator(HashMap <Queue<string>, Vector<string>>& nGramMap, int numberOfWords);
void printIntro();

/* The main sets up the console, prints the introductory message, prompts for a valid filename, and runs the
 * while loop to ask for the number of words randomly generated. The randomly generated text of the input length
 * is shown until the user puts "0" and the program is exited. */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    HashMap<Queue<string>, Vector<string>> map;
    string filename = getLine("Input file name? ");
    while(!fileExists(filename)) { //Continues to ask for a valid file name.
        cout << "Unable to open that file.  Try again." << endl;
        filename = getLine ("Input file name? " );
    }
    int valueOfN = getInteger("Value of N? ");
    while(valueOfN < 2) { // Continutes to ask for a N value greater than 2.
        cout << "N must be 2 or greater." << endl;
        valueOfN = getInteger("Value of N? ");
    }
    cout << endl;
    buildMap(map, filename, valueOfN);
    while (true) {
        int numRandWords = getInteger("# of random words to generate (0 to quit)? ");
        if (numRandWords == 0)
            break;
        while (numRandWords < valueOfN) {
            cout << "Must be at least " << valueOfN << " words." << endl << endl;
            numRandWords = getInteger("# of random words to generate (0 to quit)? ");
        }
        cout << randomWordGenerator(map, numRandWords) << endl << endl;
    }
    cout << "Exiting." << endl;
    return 0;
}
/* This map takes in a hashmap of key type Queue<string> and value type Vector<string>, a string filename, and
 * the user-inputed value of N. the method then opens the file, reads all words from the file and stores the words in
 * a vector. Then, a window of size N-1 moves through the vector and stores all the words that comes after a particular
 * set of words in the hashmap. Once the entire vector is looped around, the first N words are examined so that
 * the program wraps around the text. */
void buildMap(HashMap<Queue<string>, Vector<string> >& nGramMap, string filename, int valueOfN)
{
    ifstream input;
        openFile(input, filename);
    string word;
    Queue<string> window;
    Vector<string> text;
    while (input >> word)
        text.add(word);
    for (int n = 0; n < valueOfN-1; n++)
        if (n < text.size())
            window.enqueue(text[n]);
    for (int n = valueOfN-1; n < text.size(); n++)
        if (n < text.size())
            buildMapHelper(nGramMap, window, text, n);
    for (int n = 0; n < valueOfN-1; n++)
        if (n < text.size())
            buildMapHelper(nGramMap, window, text, n);
}

/* This method prints the appropriate introductory message of the program, which contains a very brief description
 * of the program and the instructions. */
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams'). " << endl <<
    "This program makes random text based on a document. " << endl <<
    "Give me an input file and an 'N' value for groups " << endl <<
    "of words, and I'll create random text for you." << endl << endl;
}

/* This method takes in a reference to a hashmap, reference to a window queue, reference to a vector of strings,
 * and an integer. The method then examines the word that comes after a particular window of words and adds the
 * window:next word pair to the map if the window queue is already not a key of the map. If the window is already
 * a key of the map, then the current value corresponding to the window is modified to accomodate the recent data. */
void buildMapHelper(HashMap<Queue<string>, Vector<string> >& nGramMap, Queue<string>& window,
                    Vector<string>& text, int n)
{
    string word = text[n];
    if (!nGramMap.containsKey(window)) {
        Vector<string> temp;
        temp.add(word);
        nGramMap.put(window, temp);
    }
    else {
        Vector<string> temp = nGramMap.get(window);
        temp.add(word);
        nGramMap.put(window, temp);
    }
    window.dequeue();
    window.enqueue(word);
}

/* This method takes in a hashmap nGramMap and an integer and returns a string of randomly generated words. A random
 * index is chosen in the vector of all the keys in the given map, and a string in the corresponding value vector
 * of the key is chosen to be appended to the string to be returned. */
string randomWordGenerator(HashMap<Queue<string>, Vector<string> > &nGramMap, int numberOfWords)
{
    srand((time(NULL)));
    Vector<Queue<string>> keys = nGramMap.keys();
    int randIntKey = randomInteger(0, keys.size()-1);
    Queue<string> window = keys[randIntKey];
    string message = "...";
    for (int n = 0; n < numberOfWords; n++) {
        Vector<string> wordSet = nGramMap.get(window);
        int randIntValue = rand()%wordSet.size();
        string word = wordSet[randIntValue];
        message = message + word + " ";
        window.dequeue();
        window.enqueue(word);
    }
    return message + "...";
}
