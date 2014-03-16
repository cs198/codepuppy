// Min Cheol Kim
// CS106B
// Section Leader Jimmy Lee
// Section time 5:15 Wed
// Word Ladder

/* The assignment was to find the shortest word ladder between two words, where a word
 * ladder is defined to be a connection to one word to another by changing a single letter
 * at a time. The assignment was supposed to implement a breath-first search algorithm using
 * stacks and queues to find the word ladder. */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "simpio.h"
#include "console.h"
#include "hashset.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "filelib.h"

using namespace std;
bool inputWords(string& word1, string& word2);
Stack<string> getWordLadder (string word1, string word2, HashSet<string> &dictionary);
void setUpDictionary(HashSet<string>& dict, string filename);
Vector<string> getNeighbors(HashSet<string>& dictionary, string word);
void printWordLadder(Stack<string>& wordLadder);
bool isValid(string& word1, string& word2, HashSet<string>& dictionary);
void printIntro();

/* The main sets up the console and runs the while loop that runs until the user presses "Enter" key
 * when prompted for a word. Once two valid words have been entered, an attempt is made to find the shortest
 * word ladder, and if there is no word ladder connecting those words, the program informs the user of the fact.
 * When the shortest word ladder is found, it displays the word ladder from the second word to the first. */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    while (true) {
        string word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (word1 == "")
            break;
        string word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (word2 == "")
            break;
        HashSet<string> dictionary;
        setUpDictionary(dictionary, "dictionary.txt");
        if(isValid(word1, word2, dictionary)) {
                Stack<string> wordLadder = getWordLadder(word1, word2, dictionary);
                if (wordLadder.size() == 0)
                    cout << "No word ladder found from " << word2 << " back to " << word1 << "."
                         << endl << endl;
                else {
                cout << "A ladder from " << word1 << " back to " << word2 << ":" << endl;
                printWordLadder(wordLadder);
                }
        }

    }
    cout << "Have a nice day." << endl;
    return 0;
}

/* This method takes no parameters and prints the appropriate introductory message of the program. The
 * introduction includes a very brief description of the program and a one-sentence instruction. */
void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." << endl <<
        "Please give me two English words, and I will change the" << endl <<
        "first into the second by changing one letter at a time." << endl << endl;
}

/* This method takes in references to two strings (words) and a reference to the dictionary hashset. It
 * returns true if:
 * 1. The two words are both found in the dictionary.
 * 2. They are of the same length.
 * 3. They are not the same word.
 * Otherwise, the method returns true. */
bool isValid(string& word1, string& word2, HashSet<string>& dictionary) {
    if (!(dictionary.contains(word1) && dictionary.contains(word2))) {
        cout << "The two words must be found in the dictionary." << endl << endl;
        return false;
    }
    else if (word1.length() != word2.length()) {
        cout << "The two words must be the same length." << endl << endl;
        return false;
    }
    else if (word1 == word2) {
        cout << "The two words must be different." << endl << endl;
        return false;
    }
    else
        return true;
}
/* This method returns a vector of all the neighbor words (words that are one letter change away from the
 * current word) of a given string word. This method takes in a reference to the dictionary hashset and a string,
 * and tests if the words created from a single letter change from the given word exists in the dictionary.
 * If yes, the new word is added to the vector to be returned. */
Vector<string> getNeighbors(HashSet<string>& dictionary, string word) {
    Vector<string> temp;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int n = 0; n < word.length(); n++) {
        string s = word;
        for (int m = 0; m < 26; m++) {
            s[n] = alphabet[m];
            if (dictionary.contains(s))
                temp.add(s);
        }
    }
    return temp;
}

/* This method takes in a reference to a word ladder stack and prints the elements from the top, essentially
 * from the second word to the first word. */
void printWordLadder(Stack<string> &wordLadder) {
    while (!wordLadder.isEmpty())
        cout << wordLadder.pop() << " ";
    cout << endl << endl;

}

/* This method take in two strings (first and second inputted words) and a reference to the dictionary
 * hashset. The method then implements a breath first search algorithm to return the word ladder between the
 * two given words; if the queue runs out of elements, an empty stack is returned to signify that
 * there is no valid word ladder between the two words. */
Stack<string> getWordLadder(string word1, string word2, HashSet<string>& dictionary) {
    Queue<Stack<string>> ladder;
    Stack<string> first;
    first.push(word1);
    ladder.enqueue(first);
    HashSet<string> usedWords;
    usedWords.add(word1);
    while (!ladder.isEmpty()){
        Stack<string> partial_ladder = ladder.dequeue();
        for (string s : getNeighbors(dictionary, partial_ladder.peek())) {
            if (!usedWords.contains(s)){
                if (s == word2) {
                 partial_ladder.push(s);
                 return partial_ladder;
                }
                else {
                 Stack<string> temp = partial_ladder;
                 temp.push(s);
                 ladder.enqueue(temp);
                 usedWords.add(s);
                }
            }

        }
    }
    if (ladder.isEmpty()) {
        Stack<string> none;
        return none;
    }
}

/* This method takes in a filename and a reference to a hashset. THis method then opens the file and
 * adds every single word in the dictionary to the hashset. */
void setUpDictionary(HashSet<string>& dict, string filename) {
    ifstream input;
    string line;
    if(fileExists(filename))
        openFile(input, filename);
    while (input.good()){
    getline(input, line);
    dict.add(line);
    }
}

