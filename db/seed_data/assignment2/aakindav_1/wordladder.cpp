/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides
* This program allows a user to put in two
* different words then makes a ladder connecting
* the words together.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "hashset.h"

using namespace std;

void welcomeMessage();
void getWords (Lexicon& dict, string& w1, string& w2);
void createFirstStack (Queue <Stack <string> >& ladder, string& w1);
Stack <string> findSolution (Queue <Stack <string> > &ladder, Lexicon &dict, string &w2);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    welcomeMessage();
    Lexicon dict ("dictionary.txt");
    while (true) {
        string w1 = "";
        string w2 = "";
        getWords (dict, w1,w2);
        if (w1 == "" || w2 == "" ) break;
        Queue <Stack <string> > ladder;
        createFirstStack(ladder,w1);
        Stack <string> solution = findSolution(ladder,dict,w2);
        cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
        while (!solution.isEmpty()) {
            string word = solution.pop();
            cout << word << " ";
        }
        cout << endl;
        cout << endl;
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/* Displays welcome message and asks the user for two
 * words and says how the ladder will be made.
 */
void welcomeMessage() {
    cout << "Welcome to CS106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

/* Prompts the user for the first word and checks to see if that word
 * actually has value then does the same for the second word. Finally runs
 * both words in a set of checks to make sure that they are in the
 * English dictionary, have the same size and are not the same word.
 * Passed in the lexicon containing all words in the dictionary and values
 * for both words.
 */
void getWords (Lexicon& dict, string& w1, string& w2) {
    w1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
    if (w1 == "") {
    } else {
    w2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    }
    if (w2 == "") {
    } else {
        while (!dict.contains(w1) || !dict.contains(w2) || w1.size()!=w2.size() || w1==w2) {
            cout << "Invalid selection of words!" << endl;
            w1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
            w2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        }
    }
}

/* Creates a stack and pushes the first one into it, then
 * enqueues that stack inside of the ladder containing all
 * possible routes.
 * Passed in A Queue of Stacks that this first stack will be
 * in and the string of the first word.
 * Assumes that the passed in Queue of Stacks is empty.
 */
void createFirstStack (Queue <Stack <string> >& ladder, string& w1){
    Stack <string> first;
    first.push(w1);
    ladder.enqueue(first);
}

/* Runs the ladder to a loop until it is empty. Saves the first stack
 * in the ladder as a temp stack then makes a set that will contain all
 * the already seen words. Pulls out the first word and checks all the
 * neighbors of that word makes sure its a new word and its in the
 * dictionary then checks to see if its the second word and if not
 * puts that wprd and the word passed in into a stack and equeues it
 * back into the ladder.
 */
Stack <string> findSolution (Queue <Stack <string> > &ladder, Lexicon &dict, string &w2) {
    while (!ladder.isEmpty()) {
        Stack <string> temp = ladder.dequeue();
        HashSet <string> oldWords;
        string test = temp.peek();
        for (int i=0; i<test.size(); i++) {
            for (char ch='a'; ch<='z'; ch++) {
                char original = test[i];
                test[i] = ch;
                if (dict.contains(test) && !oldWords.contains(test)) {
                    if (test == w2) {
                        temp.push(test);
                        return temp;
                    }
                    Stack <string> neighbor = temp;
                    neighbor.push(test);
                    ladder.enqueue(neighbor);
                    oldWords.add(test);
                }
                test[i] = original;
            }
        }
    }
    Stack <string> nothing;
    nothing.push("There is no ladder!");
    return nothing;
}
