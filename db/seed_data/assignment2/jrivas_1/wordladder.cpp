// This program writes a word ladder to show the shortest route
// between two words of equal length. This program uses lexicons,
// stacks, and queues to arrive at the solution.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "lexicon.h"
#include "queue.h"
#include "stack.h"

// declaring functions
using namespace std;
void IntroduceGame();
void GetTwoWords(Lexicon lex, string & word1, string & word2);
bool WordsAreGood(Lexicon lex, string word1, string word2);
bool CreateLadder(Lexicon lex, string word1, string word2);
void ShowSolution(Stack<string> stack);

int main() {
    Lexicon lex("dictionary.txt");
    string word1;
    string word2;
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    IntroduceGame();
    GetTwoWords(lex, word1, word2);
    if (CreateLadder(lex, word1, word2)) {
        cout << "Have a nice day." << endl;
    } else {
        cout << "There is no word ladder." << endl;
    } return 0;
}

/*
 * This part outputs the initial part of the program that introduces
 * the game.
*/
void IntroduceGame() {
    cout << "Welcome to CS106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}
/*
 * This function makes sure that the
*/
void GetTwoWords(Lexicon lex, string & word1, string & word2) {
    string prompt1 = "Word #1 (or Enter to quit): ";
    string prompt2 = "Word #2 (or Enter to quit): ";
    word1 = toLowerCase(getLine(prompt1));
    word2 = toLowerCase(getLine(prompt2));
    while (!WordsAreGood(lex, word1, word2)) {
        word1 = toLowerCase(getLine(prompt1));
        word2 = toLowerCase(getLine(prompt2));
    }
}

bool WordsAreGood(Lexicon lex, string word1, string word2) {
    if (!(lex.contains(word1)) && !(lex.contains(word2))) {
        cout << "The two words must be found in the dictionary." << endl;
        cout << endl;
        return false;
    } else if (word1.length() != word2.length()){
        cout << "The two words must be the same length." << endl;
        cout << endl;
        return false;
    }
    return true;
}

/*
 * CreateLadder creates a Lexicon of usedWords and then goes
 * through all the possible words using a string containing the
 * alphabet to find the best possible word ladder. The function is
 * long, but a LAiR TA told me that because half of it is required
 * in order to declare all the necessary aspects of the function,
 * that it wouldn't be worth it passing all the different sections
 * again to shorten the function by a little bit.
 */
bool CreateLadder(Lexicon lex, string word1, string word2) {
    string currentWord;
    string tempWord;
    Stack<string> tempStack;
    Lexicon usedWords;
    usedWords.add(word1);
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    Queue<Stack<string>> tester;
    Stack<string> stack;
    stack.push(word1);
    tester.enqueue(stack);
    while(!tester.isEmpty()) {
        stack = tester.dequeue();
        currentWord = stack.peek();
        for (int i = 0; i < currentWord.length(); i++) {
            for (int j = 0; j < alphabet.length(); j++) {
                tempWord = currentWord;
                tempWord[i] = alphabet[j];
                if (lex.contains(tempWord)) {
                    if (!usedWords.contains(tempWord)) {
                        if (tempWord == word2) {
                            stack.push(tempWord);
                            cout << "A ladder from " << word1 << " back to " << word2 << ":" << endl;
                            ShowSolution(stack);
                            return true;
                        } else {
                            usedWords.add(tempWord);
                            tempStack = stack;
                            tempStack.push(tempWord);
                            tester.enqueue(tempStack);
                        }
                    }
                }
            }
        }
    }
    return false;
}

/*
 * Show solution pops off the top of the stack one by
 * one until the entire solution is shown.
 */

void ShowSolution(Stack<string> stack) {
        while (!stack.isEmpty()) {
            cout << stack.pop() << " ";
        }
        cout << endl;
}

