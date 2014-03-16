/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------------
 * Assignment 2A: Word Ladder
 * This program generates a minimum-length word ladder between two words
 * provided by the user. It connects the two words through a sequence of other
 * words that each differs from its immediate neighbors by one letter change.
 * The user can keep generating new word ladders until deciding to quit.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "lexicon.h"
#include "queue.h"
#include "stack.h"

using namespace std;

/* Function prototypes */

void getWords(string& word1, string& word2, Lexicon& lex);
Stack<string> buildLadder(string word1, string word2, Lexicon& lex);
void findNeighbors(Set<string>& neighbors, string word, Lexicon& lex);
void printLadder(string word1, string word2, Stack<string>& ladder);

/* Main program */

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;

    Lexicon dictionary("dictionary.txt"); // Creates lexicon of words in dictionary.txt
    string word1, word2;
    Stack<string> ladder;

    // Repeatedly asks user for two words and creates a ladder between them
    while(true){
        getWords(word1, word2, dictionary);
        if(word1 == "" || word2 == "") break;
        ladder = buildLadder(word1, word2, dictionary);
        printLadder(word1, word2, ladder);
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/* Function: getWords
 * Usage: getWords(word1, word2, dictionary);
 * -------------------------------------------
 * This function prompts the user for two words to create a ladder between.
 * If the words are not in the dictionary, are different lengths, or are the
 * same word, the user is given additional chances to enter two valid words.
 * The user can also press Enter to quit the program.
 */

void getWords(string& word1, string& word2, Lexicon& lex){
    while(true){
        word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if(word1 == "") break;
        word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if(word2 == "") break;

        if(!lex.contains(word1) || !lex.contains(word2)){
            cout << "The two words must be found in the dictionary.";
        }
        else if(word1.length() != word2.length()){
            cout << "The two words must be the same length.";
        }
        else if(word1 == word2){
            cout << "The two words must be different.";
        }
        else break;
        cout << endl << endl;
    }
}

/* Function: buildLadder
 * Usage: Stack<string> ladder = buildLadder(word1, word2, dictionary);
 * ---------------------------------------------------------------------
 * This function creates and returns a minimum-length ladder between the two
 * words passed to it, using a breadth-first search. It builds all partial
 * ladders one step away from word1 by finding its neighbor words, and then
 * continues building partial ladders by adding additional neighbors,
 * until word2 is reached. All partial ladders of a given length must be built
 * before any longer partial ladders are started. If no partial ladder can
 * reach word2, then an empty ladder is returned. The function also keeps
 * track of all the words that have been added so that there are no repeats.
 */

Stack<string> buildLadder(string word1, string word2, Lexicon& lex){
    Queue< Stack<string> > breadth;
    Stack<string> ladder, newLadder;
    Set<string> words, neighbors;

    // Begins queue with partial ladder containing word1 only
    ladder.push(word1);
    breadth.enqueue(ladder);
    words.add(word1);


    // Replaces each partial ladder in queue with new partial ladders containing one additional neighbor
    while(!breadth.isEmpty()){
        ladder = breadth.dequeue();
        findNeighbors(neighbors, ladder.peek(), lex);
        for(string temp : neighbors){
            if(!words.contains(temp)){
                newLadder = ladder;
                newLadder.push(temp);
                breadth.enqueue(newLadder);
                words.add(temp);

                if(temp == word2) return newLadder;
            }
        }
    }

    newLadder.clear();
    return newLadder;
}

/* Function: findNeighbors
 * Usage: findNeighbors(neighbors, word, dictionary);
 * ----------------------------------------------------------------
 * This function finds all the neighbors of a word and returns them as a set.
 * A neighbor is defined as a valid word that differs from the passed word
 * by one letter change. The function loops through each index of the word and
 * replaces that letter with each letter of the alphabet, checking if that
 * results in a valid word.
 */

void findNeighbors(Set<string>& neighbors, string word, Lexicon& lex){
    string temp;
    for(int i = 0; i < word.length(); i++){
        temp = word;
        for(char ch = 'a'; ch <= 'z'; ch++){
            temp[i] = ch;
            if(lex.contains(temp)) neighbors.add(temp);
        }
    }
}

/* Function: printLadder
 * Usage: printLadder(word1, word2, ladder);
 * ------------------------------------------
 * This function prints out the word ladder from word2 back to word1. If the
 * ladder is empty, then it prints that no ladders were found.
 */

void printLadder(string word1, string word2, Stack<string>& ladder){
    if(ladder.isEmpty()){
        cout << "No word ladder found from " << word2 << " back to " << word1;
    }
    else{
        cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
        while(!ladder.isEmpty()){
            cout << ladder.pop() << " ";
        }
    }
    cout << endl << endl;
}
