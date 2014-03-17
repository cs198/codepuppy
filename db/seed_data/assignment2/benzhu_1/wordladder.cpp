//Name: Benzheng Zhu
//CS 106B Assignment 2 Part A
//Citation: lecture slides
//This program generates a word ladder that connects one word to another.
//It changes one letter at a time with the constraint that at each step the sequence of letters still forms a valid word


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "Lexicon.h"
#include "simpio.h"
#include "hashset.h"
#include "queue.h"
#include "stack.h"

using namespace std;

void welcome();
bool prompt(Lexicon& dictionary, string& word1, string& word2);
bool checkErrors(Lexicon& dictionary, string word1, string word2);
void buildLadder(Lexicon& dictionary, string& word1, string& word2);
void printLadder(Stack<string>& ladder, string& word1, string& word2);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    welcome();
    Lexicon dictionary;
    dictionary.addWordsFromFile("dictionary.txt");

    string word1 = ""; string word2 = "";
    while (prompt(dictionary, word1, word2)) {
        buildLadder(dictionary, word1, word2);
    }
    cout << "Have a nice day." << endl;
    return 0;
}

void welcome() {
    cout << "Welcome to CS 106B Word Ladder." << endl
         << "Please give me two English words, and I will change the" << endl
         << "first into the second by changing one letter at a time." << endl << endl;
}


bool prompt(Lexicon& dictionary, string& word1, string& word2) {
    //this function takes dictionary and two strings as inputs
    //it keeps prompting the usedr to enter two words until no error is detected
    //it returns false when the user press Enter.

    bool error = true;

    while (error) {
        word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (word1.empty()) {
            return false;
        }
        word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (word2.empty()) {
            return false;
        }
        error = checkErrors(dictionary, word1, word2);
    }
    return true;
}

bool checkErrors(Lexicon& dictionary, string word1, string word2) {
    //this function takes dictionary and two strings as inputs
    //it is called in prompt(...) function, to check for the following errors
    //1. if two words are found in the dictionary
    //2. if two words are of the same length
    //3. if two words are the same

    if (!dictionary.contains(word1) || !dictionary.contains(word2)) {
        cout << "The two words must be found in the dictionary." << endl << endl;
        return true;
    } else if (word1.size()!= word2.size()) {
        cout << "The two words must be the same length."<< endl << endl;
        return true;
    } else if (word1 == word2) {
        cout << "The two words must be different." << endl <<endl;
        return true;
    } else {
        return false;
    }
}

void findNeighbor(Lexicon& dictionary, string& word, HashSet<string>& neighbor) {
    //this function takes the dictionary, a string, and a hashset as inputs
    //it looks for all valid words that differ from the provided one by one letter
    //and stores them in the hashset

    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string backup = word;
    for (unsigned int i=0; i<word.size(); i++) {
        for (unsigned int j=0; j<alphabet.size(); j++) {
            word[i] = alphabet[j];
            if (dictionary.contains(word) && word != backup) {
                neighbor.add(word);
            }
            word = backup; //restore the modified word to the orginal word
        }
    }
}

void buildLadder(Lexicon& dictionary, string& word1, string& word2) {
    //this function takes the dictionary and two strings as inputs
    //it builds a ladder from one string to the other

    Queue<Stack<string>> stacks;
    Stack<string> ladder;
    ladder.push(word1);
    stacks.enqueue(ladder);

    HashSet<string> neighbor;
    HashSet<string> usedwords; //used to store words that are used in previous partial ladders
    usedwords.add(word1);

    while (!stacks.isEmpty()) {
        ladder = stacks.dequeue();
        string word = ladder.peek();
        findNeighbor(dictionary, word, neighbor);

        for (string s: neighbor) {
            if (!usedwords.contains(s)) {
                if (s == word2) {
                    ladder.push(s);
                    printLadder(ladder, word1, word2);
                    return;
                } else {
                    Stack<string> copy = ladder;
                    copy.push(s);
                    stacks.enqueue(copy);
                }
                usedwords.add(s); //tracks the words that have been used
            }
        }
        neighbor.clear(); //clear the neighbor hashset at the end of every iteration
    }
    cout << "No word ladder found from " << word2 << " back to " << word1 << "."<<endl << endl; //when no ladder is found
}


void printLadder(Stack<string>& ladder, string& word1, string& word2) {
    //this function takes a stack of strings and two strings as inputs
    //it prints out the strings in the stack

    cout << "A ladder from "<< word2 <<" back to " << word1 << ":" << endl;
    while (!ladder.isEmpty()) {
        cout << ladder.pop()<< " ";
    }
    cout << endl << endl;
}
