/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Word Ladder
 * This program intakes two words and then finds the minimum
 * word length ladder between the two of them.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "set.h"
#include "queue.h"
#include "console.h"

using namespace std;

Set<string> constructDictionary();
void intro();
string wordLadder(Set<string>& dictionary);
bool isRealWord(Set<string>& dictionary, string& word);
bool isUsedWord(Set<string>& used, string& word);
bool checkValidity(Set<string>& dictionary, string& word1, string& word2);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    Set<string> dictionary = constructDictionary();
    intro();
    string finalLadder;
    finalLadder = wordLadder(dictionary);
    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * Prints the introductory message.
 */
void intro() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

/*
 * Builds the minimal length word ladder by intaking two words and
 * looping through its neighbors (words that are one letter different).
 */
string wordLadder(Set<string>& dictionary) {
    Queue< Stack<string> > wordQueue;
    Stack<string> wordStack;
    string output;
    Stack<string> tempStack;
    string tempWord;
    string topWord;
    Set<string> used; //Creates a set of words already used in word ladders

    while (true) {
        string word1;
        string word2;
        cout << "Word #1 (or Enter to quit): " << endl;
        cin >> word1;
        if (word1=="") break;
        word1 = toLowerCase(word1);
        cout << "Word #2 (or Enter to quit): " << endl;
        cin >> word2;
        if (word2=="") break;
        word2 = toLowerCase(word2);

        if (checkValidity(dictionary, word1, word2)) {

                wordStack.push(word1);
                used.add(word1);
                wordQueue.enqueue(wordStack);

                while (!wordQueue.isEmpty()) {
                    tempStack = wordQueue.dequeue();
                    if(tempStack.peek() == word2) {

                        while (tempStack.size() > 1) {
                            output.append(tempStack.pop() + " ");
                        }

                        output.append(tempStack.pop());
                        cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
                        return output;
                        break;
                    } else {
                        topWord = tempStack.peek();
                        int length = topWord.length();

                        for (int n = 0; n < length; n++) {
                            for (int i = 'a'; i <= 'z'; i++) {
                                tempWord = topWord;
                                tempWord[n] = char(i);
                                if (!isUsedWord(used, tempWord)) {
                                    used.add(tempWord);
                                    Stack<string> copyStack;
                                    copyStack = tempStack;
                                    copyStack.push(tempWord);
                                    wordQueue.enqueue(copyStack);
                                }
                            }
                        }
                    }
                }
        } else {
            if (!isRealWord(dictionary, word1) || !isRealWord(dictionary, word2)) {
                cout << "The two words must be found in the dictionary." << endl;
                cout << endl;
            }
            if (word1.length() != word2.length()) {
                cout << "The two words must be the same lenght." << endl;
                cout << endl;
            }
            if (word1 == word2) {
                cout << "The two words must be different." << endl;
                cout << endl;
            }
        }
    }
    return output;
}


/*
 * Constructs the dictionary.
 */
Set<string> constructDictionary() {
    Set<string> dictionary;
    ifstream input;
    input.open("dictionary.txt");
    if (input.fail()) error("Can't open dictionary.txt");
    string newWord;
    while (getline(input, newWord)) {
        dictionary.add(newWord);
    }
    input.close();
    return dictionary;
}

/*
 * Checks whether the two words inputted match the requirements.
 */
bool checkValidity(Set<string>& dictionary, string& word1, string& word2) {
    if (!isRealWord(dictionary, word1) || !isRealWord(dictionary, word2)) return false;
    else if (word1.length() == word2.length()) return false;
    else if (word1 == word2) return false;
    else return true;
}

/*
 * Checks to see if the word in question is a valid English word.
 */
bool isRealWord(Set<string>& dictionary, string& word) {
    return dictionary.contains(word);
}

/*
 * Checks to see if the word in question has already been used in a word ladder.
 */
bool isUsedWord(Set<string>& used, string& word) {
    return used.contains(word);
}
