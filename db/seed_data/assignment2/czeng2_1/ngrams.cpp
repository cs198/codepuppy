/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 2B: N-grams
 * This program uses an input file to randomly generate new text containing
 * word chains, called N-grams, from the input file. The length, N, of the
 * chains is determined by the user.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "vector.h"
#include "hashmap.h"
#include "random.h"

using namespace std;

/* Function prototypes */

int promptUserForN();
void createNGram(HashMap< Queue<string>, Vector<string> >& Ngrams, Queue<string>& words, int N);
void generateTexts(HashMap< Queue<string>, Vector<string> >& Ngrams, int N);
void fillText(HashMap< Queue<string>, Vector<string> >& Ngrams, int N, int numWords);

/* Main program */

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;

    ifstream file;
    string word;
    Queue<string> fileWords;
    HashMap< Queue<string>, Vector<string> > Ngrams;

    // Asks user for file to read N-grams from
    promptUserForFile(file, "Input file name? ");
    while(file >> word){
        fileWords.enqueue(word);
    }
    file.close();

    // Asks user for N-gram length and creates random text from the N-grams
    int N = promptUserForN();
    createNGram(Ngrams, fileWords, N);
    generateTexts(Ngrams, N);

    cout << "Exiting." << endl;
    return 0;
}

/* Function: promptUserForN
 * Usage: int N = promptUserForN();
 * ---------------------------------
 * This function prompts the user to enter the length of N-grams
 * to use and returns the value as an integer. If the input is less than 2,
 * the user is given additional chances to enter a larger number.
 */

int promptUserForN(){
    int N;
    while(true){
        N = getInteger("Value of N? ");
        if(N < 2){
            cout << "N must be 2 or greater." << endl;
        }
        else{
            cout << endl;
            return N;
        }
    }
}

/* Function: createNGram
 * Usage: createNGram(Ngrams, words, N);
 * --------------------------------------
 * This function creates a map of N-grams by keeping track of all N-1 word
 * chain "prefixes" in the file text, along with the word "suffix" that follows
 * each instance of those N-1 words. Repeated suffixes are allowed, and all
 * casing and punctuation is preserved. The function wraps around the file text,
 * so that the last words of the file are followed by the first words. Files
 * are assumed to contain at least N words.
 */

void createNGram(HashMap< Queue<string>, Vector<string> >& Ngrams, Queue<string>& words, int N){
    int size = words.size() + N;
    string curr;
    Queue<string> window;

    // Moves through file text using an N-1 word window
    for(int i = 1; i < size; i++){
        curr = words.dequeue();
        if(window.size() == (N - 1)){
            if(!Ngrams.containsKey(window)){
                Vector<string> suffix(1, curr);
                Ngrams[window] = suffix;
            }
            else{
                Ngrams[window].add(curr);
            }
            window.dequeue();
        }
        window.enqueue(curr);
        words.enqueue(curr);
    }
}

/* Function: generateTexts
 * Usage: generateTexts(Ngrams, N);
 * ---------------------------------
 * This function repeatedly asks the user for the number of words of random
 * text to generate. If the input is less than N, the user is given additional
 * chances to enter a larger number. The user can also enter 0 to quit.
 */

void generateTexts(HashMap< Queue<string>, Vector<string> >& Ngrams, int N){
    int numWords;
    while(true){
        numWords = getInteger("# of random words to generate (0 to quit)? ");
        if(numWords == 0) break;
        else if(numWords < N ){
            cout << "Must be at least " << N << " words." << endl;
        }
        else fillText(Ngrams, N, numWords);
    }
}

/* Function: fillText
 * Usage: fillText(Ngrams, N, numWords);
 * --------------------------------------
 * This function creates random text of the requested number of words using
 * the N-grams previously created. It randomly chooses a prefix as a starting
 * point and continues to randomly choose suffixes to follow.
 */

void fillText(HashMap< Queue<string>, Vector<string> >& Ngrams, int N, int numWords){
    Vector< Queue<string> > keys = Ngrams.keys();
    Vector<string> next;
    Queue<string> window;

    // Randomly chooses and prints starting prefix
    int r = randomInteger(0, Ngrams.size() - 1);
    window = keys[r];
    cout << "... ";
    for(int i = 1; i < N; i++){
        string str = window.dequeue();
        cout << str << " ";
        window.enqueue(str);
    }

    // Updates word window by removing first word in prefix and adding
    // previously found suffix, then finds next suffix based on new window
    for(int i = N; i <= numWords; i++){
        next = Ngrams[window];
        r = randomInteger(0, next.size() - 1);
        window.dequeue();
        window.enqueue(next[r]);
        cout << next[r] << " ";
    }
    cout << "..." << endl << endl;
}
