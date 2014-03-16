/* Program: Word Ladder
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * ------------------------------
 * This program allows the user to enter two words of the same length.
 * It then constructs a word ladder between the two words. A word ladder
 * is a sequence of words that starts at the first words and then proceeds
 * to the second word, changing one letter each iteration with the constraint
 * that at each step the sequence of letters still forms a valid word.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"
using namespace std;

//Function prototypes
bool determineLadderWords(string& w1, string& w2, Lexicon& lexicon);
void findShortestWordLadder(string& w1, string& w2, Lexicon& lexicon, Stack<string>& result);
void findNeighbors (Stack<string>& currentLadder, Vector<string>& neighbors, Set<string> &usedWords, Lexicon& lexicon);
void printResult (Stack<string>& result, string& w1, string& w2);

//Runs the Word Ladder program until the user chooses to quit.
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
    Lexicon lexicon("dictionary.txt");
    while (true) {
        string w1, w2;
        bool endOfProgram = false;
        endOfProgram = determineLadderWords(w1,w2,lexicon);
        if (endOfProgram) break;
        if (w1 != "" && w2 != "") {
            cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
            Stack<string> result;
            findShortestWordLadder(w1,w2,lexicon, result);
            printResult(result,w1,w2);
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/* Asks the user for the two words that will be the beginning and end of the word ladder.
 * It does not accept any input that is not a valid English word except for simply pressing
 * Enter, which causes the program to end. In addition, the two words cannot be the same
 * and they must be the same length.
 */
bool determineLadderWords(string& w1, string& w2, Lexicon& lexicon) {
    while (true) {
        w1 = getLine("Word #1 (or Enter to quit): ");
        for (int i = 0 ; i < w1.length() ; i++) {
            w1[i] = tolower(w1[i]);
        }
        if (w1 == "") {
            return true;
        } else if (lexicon.contains(w1)) {
            break;
        } else {
            cout << "That word is not in the dictionary. Try again." << endl;
        }
    }
    while (true) {
        w2 = getLine("Word #2 (or Enter to quit): ");
        for (int j = 0 ; j < w2.length() ; j++) {
            w2[j] = tolower(w2[j]);
        }
        if (w2 == "") {
            return true;
        } else if (w2 == w1) {
            cout << "Those are the same word. Please choose another." << endl;
        } else if (w2.length() != w1.length()) {
            cout << "Those words are not equal length. Please choose another." << endl;
        } else if (lexicon.contains(w2)) {
            break;
        } else {
            cout << "That word is not in the dictionary. Try again." << endl;
        }
    }
    return false;
}

/* Determines the shortest word ladder by searching through the queue containing different stacks
 * that each represent a possible valid word ladder. It searches through the neighbors of the word
 * at the top of the stack and creates a new stack for each valid neighbor. The algorithm is described
 * in more detail in the handout. In addition, no word can be reused.
 */
void findShortestWordLadder(string& w1, string& w2, Lexicon& lexicon, Stack<string>& result) {
    Queue<Stack<string>> queue;
    Set<string> usedWords;
    usedWords.add(w1);
    Stack<string> initialWord;
    initialWord.push(w1);
    queue.enqueue(initialWord);
    while (!queue.isEmpty()) {
        Stack<string> currentLadder = queue.dequeue();
        Vector<string> neighbors;
        findNeighbors(currentLadder,neighbors, usedWords, lexicon);
        for (int i = 0 ; i < neighbors.size() ; i++) {
            string neighborWord = neighbors.get(i);
            if (neighborWord == w2) {
                currentLadder.push(w2);
                result = currentLadder;
            } else {
                Stack<string> currentCopy = currentLadder;
                currentCopy.push(neighborWord);
                queue.enqueue(currentCopy);
            }
        }
    }
}

/* Looks at the top word in the current word ladder and creates a Vector of each word that is one letter
 * different from that top word and that has not been used yet.
 */
void findNeighbors (Stack<string>& currentLadder, Vector<string>& neighbors, Set<string> &usedWords, Lexicon& lexicon) {
    string word = currentLadder.peek();
    for (int i = 0 ; i < word.length() ; i++) {
        word = currentLadder.peek();
        for (char letter = 'a' ; letter <= 'z' ; letter++) {
            word[i] = letter;
            if (usedWords.contains(word) == false && lexicon.contains(word)) {
                neighbors.add(word);
                usedWords.add(word);
            }
        }
    }
}

/* Looks at the stack of strings that represents the final word ladder and
 * prints it to the console. If no result was found, it prints that no word
 * ladder was found between the two words instead.
 */
void printResult (Stack<string>& result, string& w1, string& w2) {
    if (result.isEmpty()) {
        cout << "No word ladder found from " << w1 << " to " << w2 << ".";
    }
    while (result.isEmpty() == false) {
        string word = result.pop();
        cout << word << " ";
    }
    cout << endl;
}


