/* Name: Austin Poore
 *
 * This word ladder program takes in a pair of words from the user and finds the
 * shortest word ladder connecting the two, if one exists. The user can then enter
 * a new pair of words to find a new ladder. The program is not case sensitive.
 *
 * I used lecture slides.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "strlib.h"
#include "queue.h"
#include "stack.h"

using namespace std;

void printIntroduction();
void readDictionary(Lexicon& dictionary);
bool readWordsFromUser(Lexicon& dictionary, string& firstWord, string& secondWord);
void findLadder(Lexicon& dictionary, string& firstWord, string& secondWord);
Stack<string> examineNeighbors (Queue<Stack<string>>& queue, Lexicon& dictionary, Lexicon& usedWords, string& secondWord);
string ladderString(string start, Stack<string>& ladderStack);
bool validInput(Lexicon& dictionary, string& firstWord, string& secondWord);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    Lexicon dictionary;
    readDictionary(dictionary);
    printIntroduction();
    string firstWord;
    string secondWord;
    while(true) {
        if (readWordsFromUser(dictionary, firstWord, secondWord)) {
            findLadder(dictionary, firstWord, secondWord);
            cout << "" << endl;
        } else {
            break;
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * This function gets called at the start of the program, and prints the introductory
 * lines of text, including a blank line for formatting purposes.
 */

void printIntroduction() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;
}

/*
 * This function reads the dictionary file into a Lexicon called dictionary.
 */

void readDictionary(Lexicon& dictionary) {
   dictionary.addWordsFromFile("dictionary.txt");
}

/*
 * This function reads in the two words from the user and calls a helper function
 * to determine whether the input is valid. If the words are a valid input, the function
 * returns true and stores the words in the variables firstWord and secondWord. If the user
 * enters a blank line, the function immediately returns false.
 */

bool readWordsFromUser(Lexicon& dictionary, string& firstWord, string& secondWord) {
    while (true) {
        firstWord = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (firstWord == "") {
            return false;
        }
        secondWord = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (secondWord == "") {
            return false;
        }
        if (validInput(dictionary, firstWord, secondWord)) {
            break;
        }
    }
    return true;
}

/*
 * This function checks to ensure that the user's words make up a valid input. If
 * they do, it returns true. If they are invalid, a message to that effect gets printed
 * to the screen and the method returns false.
 */

bool validInput(Lexicon& dictionary, string& firstWord, string& secondWord) {
    while(true) {
        if (firstWord.length() != secondWord.length()) {
            cout << "The two words must be the same length." << endl;
            cout << "" << endl;
            return false;
        }
        if (firstWord == secondWord) {
            cout << "The two words must be different." << endl;
            cout << "" << endl;
            return false;
        }
        if (!dictionary.contains(firstWord) || !dictionary.contains(secondWord)) {
            cout << "The two words must be found in the dictionary." << endl;
            cout << "" << endl;
            return false;
        }
        return true;
    }
}

/*
 * This method creates a queue of stacks to search through all possible word ladders
 * until it reaches the desired word or has exhausted all possible partial ladders without
 * finding the second word, in which case there is no ladder between the two of them.
 */

void findLadder(Lexicon& dictionary, string& firstWord, string& secondWord) {
    Queue<Stack<string>> q; // Used to store the stacks of partial ladders
    Stack<string> w1; // The first partial word ladder, which will contain the first word
    Lexicon usedWords; // Used to keep track of which words have already been used
    usedWords.add(firstWord);
    w1.push(firstWord);
    q.enqueue(w1);
    string noLadder = "1"; // Indicator to check and ensure that a ladder between the words exists.
    bool ladderPresent = false; // If a successful ladder is found, this gets changed to true.
    while (!q.isEmpty()) {
        Stack<string> ladder = examineNeighbors(q, dictionary, usedWords, secondWord);
        string firstEntry = ladder.peek();
        if (firstEntry != noLadder) { // Makes sure a ladder was found.
            string shortLadder = ladderString(secondWord, ladder); // Converts the ladder to a string to be printed.
            ladderPresent = true;
            cout << "A ladder from " << secondWord << " back to " << firstWord << ":" << endl;
            cout <<  shortLadder << endl;
            break;
        }
    }
    if(!ladderPresent) { // Ensures that no ladder was found.
        cout << "No ladder found from " << secondWord << " back to " << firstWord << "." << endl;
    }
}

/*
 * This function gets called to create the partial word ladders. It returns a stack of strings containing the string "1" if
 * word 2 is not found, or containing the successful ladder if word 2 is found.
 */

Stack<string> examineNeighbors (Queue<Stack<string>>& queue, Lexicon& dictionary, Lexicon& usedWords, string& secondWord) {
    Stack<string> currStack = queue.dequeue();
    Stack<string> noLadder;
    string noLadderExists = "1";
    noLadder.push(noLadderExists);
    string currWord = currStack.peek();
    string tempWord = currWord; // Make a copy of the word to change it without worrying about messing up the original.
    for (int i = 0; i < currWord.length(); i++) {
        tempWord = currWord; // Reset tempWord to currWord every time you move on to a new index to change.
        for (int j = 0; j < 26; j++) {
            tempWord[i] = 'a' + j;
            if (dictionary.contains(tempWord)) { // See if it's a valid word.
                if (!usedWords.contains(tempWord)) { // Make sure the word has not been used in a partial word ladder yet.
                    if (tempWord == secondWord) {
                        return currStack; // We have completed the word ladder and need to return a stack with the successful ladder.
                    } else {
                        Stack<string> newStack = currStack; // Copy the stack and add the word to be checked later as another partial ladder.
                        newStack.push(tempWord);
                        usedWords.add(tempWord); // Add the word to the list of used words.
                        queue.enqueue(newStack);
                    }
                }
            }
        }
    }
    return noLadder; // If the ladder is unsuccessful, this stack gets returned as an indicator.
}

/*
 * This function returns a string version of the stack containing the successful word ladder to be printed on the console.
 */

string ladderString(string start, Stack<string>& ladderStack) {
    string result = start;
    result += " ";
    while(!ladderStack.isEmpty()) {
        result += ladderStack.pop();
        result += " ";
    }
    return result;
}
