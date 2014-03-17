// Xinsheng Chu
// This function connects  one word to another formed by changing one letter at a time with the
// constraint that at each step the sequence of letters still forms a valid word.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"

using namespace std;

/*
 * This function displays all the messages on the console.
 */
void displayMessage() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;
}

/*
 * This function checks if the user inputs of two words are valid.
 * It takes the two words and the dictionary.
 * If lengths of two words are different, or the two words are not found in the dictionary, or the two words are the same,
 * it returns false, else it returns true.
 */
bool validInput (string s1, string s2, Lexicon& dictionary) {
    if (s1.length() != s2.length()) {
        cout << "The two words must be the same length." << endl;
        cout << "" << endl;
        return false;
    } else if (!dictionary.contains(s1) || !dictionary.contains(s2)) {
        cout << "The two words must be found in the dictionary." << endl;
        cout << "" << endl;
        return false;
    } else if (s1 == s2) {
        cout << "The two words must be different." << endl;
        cout << "" << endl;
        return false;
    } else {
        return true;
    }
}

/*
 * This function detects if the a word is contained in the dictionary.
 * It takes in the word and the dictionary.
 * If yes it returns true, else it returns false.
 */
bool validWord (string word, Lexicon& dictionary) {
    if (dictionary.contains(word)) {
        return true;
    } else {
        return false;
    }
}

/*
 * This function finds the word ladder between two words.
 * It takes two words and dictionary as input,
 * and goes through the algorithm as pseudocode description as in the spec.
 */

/*
void findWordLadder(string word1, string word2, Lexicon& dictionary, Stack<string> ladderStack) {
    Queue<Stack<string> > ladderQueue;
    Set<string> usedWords;
    usedWords.add(word1);
    ladderStack.push(word1);
    ladderQueue.enqueue(ladderStack);
    while(!ladderQueue.isEmpty()) {
        ladderStack = ladderQueue.peek();
        ladderQueue.dequeue();
        string topWord = ladderStack.peek();
        for (int i = 0; i < word1.size(); i++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                string neighbor = topWord;
                neighbor[i] = ch;
                if (validWord(neighbor, dictionary) && !usedWords.contains(neighbor)) {
                    usedWords.add(neighbor);
                    if (neighbor == word2) {
                        ladderStack.push(neighbor);
                        return ladderStack;
                    } else {
                        Stack<string> ladderStackCopy = ladderStack;
                        ladderStackCopy.push(neighbor);
                        ladderQueue.enqueue(ladderStackCopy);
                    }
                }
            }
        }
    }
    return ladderStack;
}
*/

Stack<string> findWordLadder(string word1, string word2, Lexicon& dictionary) {
    Queue<Stack<string> > ladderQueue;
    Stack<string> ladderStack;
    Set<string> usedWords;
    usedWords.add(word1);
    ladderStack.push(word1);
    ladderQueue.enqueue(ladderStack);
    while(!ladderQueue.isEmpty()) {
        ladderStack = ladderQueue.peek();
        ladderQueue.dequeue();
        string topWord = ladderStack.peek();
        for (int i = 0; i < word1.size(); i++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                string neighbor = topWord;
                neighbor[i] = ch;
                if (validWord(neighbor, dictionary) && !usedWords.contains(neighbor)) {
                    usedWords.add(neighbor);
                    if (neighbor == word2) {
                        ladderStack.push(neighbor);
                        return ladderStack;
                    } else {
                        Stack<string> ladderStackCopy = ladderStack;
                        ladderStackCopy.push(neighbor);
                        ladderQueue.enqueue(ladderStackCopy);
                    }
                }
            }
        }
    }
    return ladderStack;
}

/*
 * This function prints out the final result and message.
 */
void printResult(Stack<string>& ladder, string word1, string word2) {
    if (ladder.peek() != word2) {
        cout << "No word ladder found from " << word2 << " to " << word1 << endl;
    } else {
        cout << "A ladder from " << word2 << " back to " << word1 << endl;
        while (!ladder.isEmpty()) {
            cout << ladder.pop() << " ";
        }
        cout << "" << endl;
    }
    cout << "" << endl;
}

/*
 * This function trims the words, so that all the space are deleted from both beginning of the word and end of the word.
 * For example, if user inputs "   example     ", it returns the string "example";
 */
string trimWord(string word) {
    while (word[word.size()-1] == ' ') {
        word = word.substr(0, word.size()-1);
    }
    while (word[0] == ' ') {
        word = word.substr(1, word.size());
    }
    return word;
}

/*
 * This main function displays the message, opens the dictionary,
 * and prompts user to input valid words, finds word ladder and prints results.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    displayMessage();
    Lexicon dict;
    dict.addWordsFromFile("dictionary.txt");

    while (true) {
        string w1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        w1 = trimWord(w1);
        if (w1 == "") {
            break;
        } else {
            string w2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
            w2 = trimWord(w2);
            if (w2 == "") {
                break;
            } else {
                while (!validInput(w1, w2, dict)) {
                    w1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
                    w2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
                    w1 = trimWord(w1);
                    w2 = trimWord(w1);
                }
                //Stack<string> wordLadderResult;
                //findWordLadder(w1, w2, dict, wordLadderResult);
                Stack<string> wordLadderResult = findWordLadder(w1, w2, dict);
                printResult(wordLadderResult, w1, w2);
            }
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}
