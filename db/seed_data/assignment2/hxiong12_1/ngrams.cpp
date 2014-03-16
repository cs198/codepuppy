/// Assignment 2B: N-Grams
/// This program generates text from n-grams, or a data structure of word groups, to
/// generate text that sounds as if it came from the same author as the input file.
/// Sample input files have been provided.
/// Coded by: Helen Xiong


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "hashmap.h"
#include "queue.h"
#include "vector.h"
#include "random.h"
#include "foreach.h"

using namespace std;

/*Checks if an input character is one that delineates a word. Used in the getWord method.
 * Method returns true if input character is a whitespace, tab, or vertical tab.*/
bool isBreakCharacter(char ch) {
    int num = int(ch);
    switch (num) {
    case 32: //space
    case 11: //vertical tab
    case 9: //horizontal tab
        return true;
        break;
    default: return false;
    }
}

/*Returns true if it parses a word from the input line.
 * Breaks an input line of text into separate words (without stripping punctuation or capitals).
 * "track" is updated to the start of the word (i.e. following the last whitespace), and i is the position of the character in the line.
 * The string word is passed by reference, and updated to match the next word from the input line.*/
bool getWord(string line, string & word, int & i, int & track) {
    word = "";
    if (isBreakCharacter(line[i])) { //finds the next whitespace in the line
        word = line.substr(track, i - track);
        track = i + 1;
        if (word != "") return true;
    }
    if (i == int(line.size()) - 1 && !isBreakCharacter(line[i])) { //there is a non-space character at the end of the line
        for (int j = line.size() - 1; j > -1; j--) {
            if (isBreakCharacter(line[j])) {
                word = line.substr(j + 1, line.size() - j - 1);
                if (word != "") return true;
            }
            if (j == 0 && !isBreakCharacter(line[i])) { // there are no whitespaces in the line, i.e. the entire line is one word
                word = line;
                return true;
            }
        }
    }
    return false; //the substring of the line is either blank or consists only of tabs/spaces
}

/*Updates the key-queue and the value vector based on the next word read from the document, and
 * stores it in the map of all the n-gram mappings in the document.
 * Assumes that the key-queue has already been initialized to the correct length (done in initialQueueing method).*/
void storeInQueue(HashMap< Queue<string>, Vector<string> > & wordmap, Queue<string> & nGramsKey, string & nextWord, string & word) {
    Queue<string> temp = nGramsKey; //shifts the values in the queue up by one word, and adds the new word
    Vector<string> Value;
    temp.dequeue();
    temp.enqueue(word);
    nGramsKey = temp;
    Value = wordmap[nGramsKey]; //Calls, and adds the new word to the value-vector of the key-queue
    Value.add(nextWord);
    wordmap.put(nGramsKey, Value);
    word = nextWord; //updates the next word
}

/*Creates an appropriately-sized map entry from the first N words in the document.
 * Stores the first entry in the map.*/
void initialQueueing(HashMap< Queue<string>, Vector<string> > & wordmap, Queue<string> & nGramsKey, int nInteger, string & nextWord, string & word) {
    int keysize = nGramsKey.size();
    if (keysize < nInteger - 1) {
        nGramsKey.enqueue(nextWord);
    } else if (keysize == nInteger - 1) {
        Vector<string> nGramsValue;
        nGramsValue.add(nextWord);
        wordmap.put(nGramsKey, nGramsValue);
        word = nextWord;
    }
}

/*Fills in the N-gram mapping for the document.
 * Generates individual words using the getWord method.
 * Calls on the storeInQueue method to store the words into a HashMap, wordmap, that contains the
 * possible n-gram pairings.
 * When the document runs out of words, method "wraps around" and maps the final words adjacent to
 * the words at the beginning of the document.*/
void parseDocument(HashMap< Queue<string>, Vector<string> > & wordmap, ifstream & input, int nInteger) {
    string line;
    string nextWord; //next word from the document
    string prevWord;
    Queue<string> nGramsKey; //queue to be stored as key
    Queue<string> firstWords; //queue that stores the initial words to "wraparound" document
    //begins to read document
    while (getline(input, line)) {
        int track = 0;
        for (int i = 0; i < int(line.size()); i++) { //goes through the characters in each line to "read" words
            if (getWord(line, nextWord, i, track)) {
                if (wordmap.isEmpty()) {
                    initialQueueing(wordmap, nGramsKey, nInteger, nextWord, prevWord);
                    firstWords = nGramsKey; //stores first words in the document
                } else {
                    storeInQueue(wordmap, nGramsKey, nextWord, prevWord);
                }
            }
        }
    }
    while (!firstWords.isEmpty()) {
        nextWord = firstWords.dequeue();
        storeInQueue(wordmap, nGramsKey, nextWord, prevWord);
    }
}

/*Creates a vector that stores the queues of words generated by parseDocument.
 * Called on by method getRandomWord for creating random starting points..*/
Vector<Queue<string> > generateRandomWordmapKeys(HashMap< Queue<string>, Vector<string> > & wordmap) {
    Vector<Queue<string> > wordcollection;
    for (Queue<string> key : wordmap) {
        wordcollection.add(key);
    }
    return wordcollection;
}

/*Returns a random n-gram word from the vector of the current reading "window".
 * Updates the current queue to find the next n-gram word easily*/
string getRandomWord(HashMap< Queue<string>, Vector<string> > & wordmap, Queue<string> & workingQueue) {
    int random = randomInteger(0, wordmap[workingQueue].size() - 1);
    string word = wordmap[workingQueue].get(random);
    workingQueue.dequeue();
    workingQueue.enqueue(word);
    return word;
}

/*Generates and displays the randomly-generated text.*/
void generateRandomText(HashMap< Queue<string>, Vector<string> > & wordmap, Vector<Queue<string> > & wordcollection, int nWords) {
    int random = randomInteger(0, wordcollection.size() - 1);
    Queue<string> newQueue = wordcollection[random];
    Queue<string> workingQueue;
    string printstring = "... ";
    while (!newQueue.isEmpty()) {
        string word = newQueue.dequeue();
        printstring += word + " ";
        workingQueue.enqueue(word);
    }
    for (int i = workingQueue.size(); i < nWords; i++) {
        printstring += getRandomWord(wordmap, workingQueue) + " ";
    }
    printstring += "...";
    cout << printstring << endl;
}

/*Takes user input and sets up a database to generate n-grams. Displays the randomly-generated text on the console.*/
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;

    HashMap<Queue<string>, Vector<string> > wordmap; //the "master" mapping for the entire document

    //User inputs file name. Includes error-checking for files that do not exist.
    ifstream input;
    string textfile = getLine("Input file name? ");
    input.open(textfile.c_str());
    while (input.fail()) { //Checks if file name is valid
        cout << "Unable to open that file.  Try again." << endl;
        textfile = getLine("Input file name? ");
        input.open(textfile.c_str());
    }

    //User inputs the n-value for the n-grams.
    int nInteger = getInteger("Value of N? ");
    while (nInteger < 2) { //Checks if number entered is an illegal value, i.e. less than 2.
        cout << "N must be 2 or greater." << endl;
        nInteger = getInteger("Value of N? ");
    }
    cout << "" << endl;

    //If N-value and input file are valid, the program sets up the "master" n-gram database.
    parseDocument(wordmap, input, nInteger);
    Vector<Queue<string> > wordcollection = generateRandomWordmapKeys(wordmap);

    //User inputs the number of words to be generated, and the program generates n-number of words.
    int nWords = getInteger("# of random words to generate (0 to quit)? ");
    while (true) {

        while (nWords < nInteger && nWords != 0) { //Checks if number entered is an illegal value, i.e. less than the N-value.
            cout << "Must be at least " << nInteger << " words." << endl;
            cout << "" << endl;
            nWords = getInteger("# of random words to generate (0 to quit)? ");
        }
        if (nWords == 0) break; //exiting the program
        generateRandomText(wordmap, wordcollection, nWords);
        cout << "" << endl;
        nWords = getInteger("# of random words to generate (0 to quit)? ");
    }

    cout << "Exiting." << endl;
    return 0;
}
