/* Michelle S. Lee
 * CS 106B Assignment 2 (WORDLADDER)
 *
 * This program takes an input of two words of the same length from the user
 * and prints a word ladder, a connection formed by changing one letter at a time producing
 * a valid word at each sequence.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "vector.h"
#include "queue.h"
#include "stack.h"
#include "set.h"

using namespace std;

string filename = "dictionary.txt"; //File with a dictionary of valid words

Stack<string> findWordLadder (Set<string> &dictionary, string &word1, string &word2);
Vector<string> returnNeighbors (Set<string> &dictionary, string &word);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    Set<string> dictionary;

    //Open the dictionary file and save the words into a set.
    ifstream infile;
    infile.open("dictionary.txt");
    if (infile.fail()) {
        error("file didn't open");
        infile.clear();
    } else {
        string word;
        while (getline(infile, word)) {
            dictionary.add(word);
        }
    }
    infile.close();

    //Get two valid words from the user.
    string word1, word2;
    while (true) {
        cout << "Word #1 (or Enter to quit): ";
        getline(cin, word1);
        if (word1 == "") break;
        word1 = toLowerCase(word1);
        while (!dictionary.contains(word1)) {
            cout << "Enter a valid word. Enter Word #1: ";
            getline(cin, word1);
        }
        cout << "Word #2 (or Enter to quit): ";
        getline(cin, word2);
        if (word2 == "") break;
        word2 = toLowerCase(word2);
        while (!dictionary.contains(word2)) {
            cout << "Enter a valid word. Enter Word #2: ";
            getline(cin, word2);
        }
        while (word1.length() != word2.length()) {
            cout << "Word #1 and Word #2 must be of same length. Enter Word #2: ";
            getline(cin, word2);
        }
        while (word1 == word2) {
            cout << "Word #1 and Word #2 cannot be the same word. Enter Word #2: ";
            getline(cin, word2);
        }

        //Find the word ladder and print it.
        Stack<string> ladder = findWordLadder(dictionary, word1, word2);
        cout << ladder << endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}

//This function finds the word ladder and returns the stack of strings containing the ladder
Stack<string> findWordLadder (Set<string> &dictionary, string &word1, string &word2) {
    Stack<string> s;
    s.push(word1);
    Queue<Stack<string>> q;
    q.enqueue(s);
    Set<string> used;
    used.add(word1);
    while (!q.isEmpty()) {
        Stack<string> next = q.dequeue();
        string currentString = next.peek();
        Vector<string> neighbors = returnNeighbors(dictionary, currentString);
        for (int i = 0; i < neighbors.size(); i++) {
            if (!used.contains(neighbors[i])) {
                Stack<string> copy = next;
                if (neighbors[i] != word2) {
                    copy.push(neighbors[i]);
                    q.enqueue(copy);
                    used.add(neighbors[i]);
                } else {
                    copy.push(word2);
                    return copy;
                }
            }
        }
    }
    return s;
}

//This function takes a word and finds all its "neighbors" by changing one letter of the word
Vector<string> returnNeighbors (Set<string> &dictionary, string &word) {
    Vector<string> neighbors;
    for (int i = 0; i < word.length(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            string copy = word;
            string insert = "";
            insert += ch;
            string neighbor = copy.replace(i, 1, insert);
            if (dictionary.contains(neighbor) && word != neighbor) {
                neighbors.add(neighbor);
            }
        }
    }
    return neighbors;
}
