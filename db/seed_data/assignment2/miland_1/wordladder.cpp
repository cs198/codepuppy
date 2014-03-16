/**
 * Name: Milan Doshi
 * Assignment: wordladder.cpp
 * TA: Jimmy Lee
 *
 * This program runs the program by taking in two words, and then by changing one letter of the word
 * at a time creates a ladder of words where each word in the ladder differs by one word from the previos
 * word in the ladder until the ladder from the second word entered to the first word entered is given.
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
#include "stack.h"
#include "vector.h"

using namespace std;
void printMessage();
void wordLadder(string w1, string w2);
void readInDictionary(string filename);
void takeInWords();
void checkWords(string word1, string word2);
void restoreDictionary();

HashMap<string,bool>dictionary;
Vector<string> ladder;

int main() {
    printMessage();
    readInDictionary("dictionary.txt");
    takeInWords();
    return 0;
}   

void printMessage() {
    setConsoleSize(750, 450);
    setConsoleFont("Courier New-16");
    setConsoleEcho(true);

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "If you give me two English words, I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

/**
 * method: readInDictionary(string filename)
 * return: void
 * This method reads in a file name, which in this case is a dictionary file, and stores each word
 * in a collection that holds all the words.
 */
void readInDictionary(string filename) {
   string line;
   string word;
   ifstream input;
   input.open(filename.c_str());
   while(getline(input,line)) {
       istringstream tokens(line);
       tokens >> word;
       dictionary.put(word,true);
   }
}

/**
 * method: takeInWords()
 * return: void
 * This method prompts the user to enter two words and then calls a method which formulates the word
 * ladder based on those two words. Until the user presses "Enter" to quit, the program continues to
 * find word ladders for words entered.
 */
void takeInWords() {
    string word1 = getLine("Word #1 (or Enter to quit): ");
    word1 = toLowerCase(word1);
    word1 = trim(word1);
    while (word1 != "") {
        string word2 = getLine("Word #2 (or Enter to quit): ");
        if(word2 == "") break;
        word2 = toLowerCase(word2);
        word2 = trim(word2);
        checkWords(word1, word2);
        cout << "" << endl;
        word1 = getLine("Word #1 (or Enter to quit): ");
        word1 = toLowerCase(word1);
        word1 = trim(word1);
    }
    cout << "Have a nice day." << endl;
}

/**
 * method: checkWords()
 * return: void
 * This method checks both words, and based on the length of the words, whether or not they are the same
 * word, and if they are in the dictionary or not, the program takes an action.
 */
void checkWords(string word1, string word2) {
    if (dictionary.containsKey(word1) == false || dictionary.containsKey(word2) == false) {
        cout << "The two words must be found in the dictionary." << endl;
    } else if (word1.length() != word2.length()) {
        cout << "The two words must be the same length." << endl;
    } else if (word1 == word2) {
        cout << "The two words must be different." << endl;
    } else {
        wordLadder(word1,word2);
        restoreDictionary();
    }
}

/**
 * method: restoreDictionary();
 * return: void
 * This method goes through and sets the corresponding values of all the words used in the previous ladder to true
 * to ensure that those same words can be used again in the next ladder.
 */
void restoreDictionary() {
    foreach (string s in dictionary) {
        dictionary[s] = true;
    }
    ladder.clear();
}

/**
 * method: wordLadder(string w1, string w2)
 * return: void
 * This method uses an algorithm generates the word ladder for the two strings (in this case words) which
 * are passed in as parameters. It does this through using the dictionary which was previously read in
 * and then going through each letter of the second word entered (w2) to find words that are in the
 * dictionary and continues this process one letter at at time, adding a new word to the ladder each time,
 * until the final word added to the ladder is the first word entered (w1).
 */
void wordLadder(string w1, string w2) {
    Queue<Stack<string> >ladders;
    Stack<string> words;

    words.push(w1);
    dictionary[w1] = false;
    ladders.enqueue(words);
    while(ladders.isEmpty() == false)  {
        Stack<string> currentStack = ladders.dequeue();
        string word = currentStack.peek();
        string temp = word;
        if(word == w2) {
            cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
            while(currentStack.isEmpty() == false) {
               string temp = currentStack.pop();
               cout << temp << " ";
               ladder.add(temp);
            }
            cout << " " << endl;
            return;
        } else {
             for(int i = 0; i < (int) w1.length(); i++) {
                 for(char l = 'a'; l <= 'z'; l++) {
                    string letter;
                    stringstream s;
                    s << l;
                    s >> letter;
                    string neighbor = word.replace(i, 1, letter);
                    if(dictionary.containsKey(neighbor) == true) {
                       if(dictionary[neighbor] == true) {
                            Stack<string>copy = currentStack;
                            copy.push(neighbor);
                           dictionary[neighbor] = false;
                            ladders.enqueue(copy);
                        }
                      }
                 }
                 word = temp;
            }
        }
    }
    cout << "No word ladder found from " << w2 << " back to " << w1 << ".";
    cout << " " << endl;
}
