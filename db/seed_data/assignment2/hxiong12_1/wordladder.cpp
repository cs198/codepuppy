//// CS 106B WordLadder
/// Coded by: Helen Xiong
/// This program gneerates a word ladder, or a sequence of English words achieved by changing one letter between two words.
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

/*Prints a successful word ladder onto the console*/
void printWordLadder(Stack<string> & tempstack, string word1, string word2) {
    string output = word2 + " ";
    while (!tempstack.isEmpty()) {
        output += tempstack.pop() + " ";
    }
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    cout << output << endl;
}

/*Returns true if a word ladder exists between the two words entered.
 * Based on pseudocode in the Assignment 2 spec document. (quoted in comments). Each potential word ladder is stored as a stack in a queue.*/
bool findWordLadder(string & word1, string & word2, Map<string, bool> & wordtracker) {
    Stack<string> wordstack;
    Queue<Stack<string> > wordLadderQueue; //"create an empty queue of stacks"
    wordstack.push(word1); //"add a stack containing {word1} to the queue"
    wordLadderQueue.enqueue(wordstack);
    while (!wordLadderQueue.isEmpty()) { //"while the queue is not empty:"
        Stack<string> tempstack = wordLadderQueue.dequeue(); //"dequeue the partial-ladder stack from the front of the queue."
        string tempword = tempstack.peek();
        for (int i = 0; i < int(tempword.length()); i++) { //"for each valid English word that is a "neighbor"" (cycles through letters)
            for (int j = 0; j < ALPHABET_LENGTH; j++) {
                int ch = j + 97;
                tempword[i] = ch;
                if (wordtracker.containsKey(tempword) && !wordtracker[tempword]) { //"if that neighbor word has not already been used in a ladder before:"
                    wordtracker[tempword] = true;
                    if (tempword == word2) { //"if the neighbor word is w2:"
                        printWordLadder(tempstack, word1, word2); // prints the valid word ladder
                        return true;
                    } else { //"otherwise:"
                        Stack<string> newStack = tempstack; //"create a copy of the current partial-ladder stack."
                        newStack.push(tempword); //"stores new word on the wordladder"put the neighbor word on top of the copy stack."
                        wordLadderQueue.enqueue(newStack); //"add the copy stack to the end of the queue."
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
