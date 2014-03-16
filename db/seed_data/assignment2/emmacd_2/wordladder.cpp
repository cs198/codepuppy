/*
 * Emma Daugherty; CS106B; Section Leader: Jimmy Lee
 *
 * This program allows users to enter two words then builds a word ladder between them by only changing one letter
 * at a time.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "stack.h"
#include "queue.h"
#include "hashset.h"
#include "simpio.h"
#include "lexicon.h"
#include "strlib.h"


using namespace std;

//Methods
void printIntro();
Stack<string> findLadder(string w1, string w2, Lexicon& dictionary);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    printIntro();

    string w1 = getLine("Word #1 (or Enter to quit): ");
    while (true) {
        if (w1 != "") {
            string w2 = getLine("Word #2 (or Enter to quit): ");
            if (w2 != "") {
                Lexicon dictionary("dictionary.txt");

                toLowerCase(w1);
                toLowerCase(w2);
                if (w1.length() == w2.length() && dictionary.contains(w1) && dictionary.contains(w2)) {
                    Stack<string> solution = findLadder(w1, w2, dictionary);
                    cout << w2 + " ";
                    while (!solution.isEmpty()) {
                        cout << solution.pop() + " ";
                    }
                    cout << endl;
                    cout << "" << endl;
                    w1 = getLine("Word #1 (or Enter to quit): ");
                }
                else {
                    cout << "Invalid entries. Please make sure both words are the same length";
                    cout << " and are valid English words." << endl;
                    w1 = getLine("Word #1 (or Enter to quit): ");
                }
            }
            else { break; }
        }
        else { break; }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * This method finds a ladder between two words of the same length that are both English words by changing one letter
 * at a time. It returns an empty stack of strings if it fails to find a word ladder.
 */
Stack<string> findLadder(string w1, string w2, Lexicon& dictionary) {
    Queue<Stack<string> > ladders;
    Stack<string> first;
    first.push(w1);
    ladders.enqueue(first);
    HashSet<string> used;
    used.add(w1);

    while(!ladders.isEmpty()) {
        Stack<string> stack = ladders.dequeue();
        string top = stack.peek();
        int length = w1.length();
        for (int i = 0; i < length; i++) {
            string top = stack.peek();
            for (char k = 'a'; k <= 'z'; k++) {
                top[i] = k;
                if (dictionary.contains(top)) {
                    if (!used.contains(top)) {
                        if (top == w2) {
                            return stack;
                        }
                        else {
                            used.add(top);
                            Stack<string> copy = stack;
                            copy.push(top);
                            ladders.enqueue(copy);
                        }
                    }
                }
            }
        }
    }
    Stack<string> empty;
    return empty;
}

/*
 * This method prints out an introduction to the program explaining what it does.
 */
void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;
}













