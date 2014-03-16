//// CS 106B WordLadder
/// Coded by: Helen Xiong
/// This program generates a word ladder, or a sequence of English words achieved by changing one letter between two words.
/// Sources cited: pseudocode from Assignment 2 Spec

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

/*The length of the English alphabet, or the number of times that the code has to cycle through to find "neighbors"*/
int ALPHABET_LENGTH = 26;

/*Prints a successful word ladder onto the console. Inputs the word ladder stack that is believed to be successful.*/
void printWordLadder(Stack<string> & tempstack, string word1, string word2) {
    string output = word2 + " ";
    while (!tempstack.isEmpty()) {
        output += tempstack.pop() + " ";
    }
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    cout << output << endl;
}

/*Returns true if a word ladder exists between the two words entered.
 * Based on pseudocode in the Assignment 2 spec document. (quoted in comments).
 * Each potential word ladder is stored as a stack in a queue. The program finds "neighbors", and then "neighbors of neighbors",
 * i.e. words that differ by only a letter. These are all potential next steps in the correct ladder.
 * If one of the "neighbors" is the correct word, then the word ladder exists. If the program cycles through
 * all possible "neighbors" and "neighbors of neighbors" without finding a match, then it returns false
 * and assumes that word ladder does not exist.*/
bool findWordLadder(string & word1, string & word2, Map<string, bool> & wordtracker) {
    Stack<string> wordstack;
    Queue<Stack<string> > wordLadderQueue; //creates a queue of stacks
    wordstack.push(word1);
    wordLadderQueue.enqueue(wordstack);
    while (!wordLadderQueue.isEmpty()) {
        Stack<string> tempstack = wordLadderQueue.dequeue();
        string tempword = tempstack.peek();
        for (int i = 0; i < int(tempword.length()); i++) { //replaces each letter of the word with other letters, one at a time
            for (int j = 0; j < ALPHABET_LENGTH; j++) { //cycles through alphabet
                int ch = j + 97;
                tempword[i] = ch;
                if (wordtracker.containsKey(tempword) && !wordtracker[tempword]) { //the generated word is a valid English word that has not been used
                    wordtracker[tempword] = true;
                    if (tempword == word2) { //the word ladder ends
                        printWordLadder(tempstack, word1, word2); // prints the valid word ladder
                        return true;
                    } else { //"otherwise:"
                        Stack<string> newStack = tempstack; //copies ladder and stores new word into an existing potential ladder
                        newStack.push(tempword); //stores new word on the wordladder
                        wordLadderQueue.enqueue(newStack); //stores wordladder in the queue
                    }
                }
            }
            tempword = tempstack.peek(); //reset neighboring word
        }
    }
    return false;
}

/*Checks if the pair of input words are valid entries. Returns false (not valid) if:
 * - words are not the same length
 * - one or both user-entered words does not exist in English
 * - the words are the same word*/
bool wordsAreValid(string word1, string word2, Lexicon & english) {
    if (word1.length() != word2.length()) {
        cout << "The two words must be the same length." << endl;
        return false;
    } else if (!english.contains(word1) || !english.contains(word2)) {
        cout << "The two words must be found in the dictionary." << endl;
        return false;
    } else if (word1 == word2) {
        cout << "The two words must be different." << endl;
        return false;
    } else return true;
}

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;

    Lexicon english("dictionary.txt"); //builds lexicon
    Map<string, bool> wordtracker; //keeps track of which words have been used

    string word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
    string word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    //Finds if word ladder exists for user-input words
    while (!(word1 == "" || word2 == "")) {
        if (wordsAreValid(word1, word2, english)) { //error-checking, see wordsAreValid method
            for (string word : english) { //resets the "used" status of the words in the map in preparation for a new set of words.
                wordtracker.put(word, false);
            }
            if (!findWordLadder(word1, word2, wordtracker)) cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
        }
        cout << "" << endl;
        word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (word1 == "") break;
        word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    }

    cout << "Have a nice day." << endl;
    return 0;
}
