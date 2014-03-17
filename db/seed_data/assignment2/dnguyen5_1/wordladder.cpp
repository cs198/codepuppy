// Prompts user for two words, and then displays a word ladder between the two.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "hashset.h"
#include "set.h"
#include "queue.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"

using namespace std;
void introStatement();
string getWord(int wordNumber);
Stack<string> createWordLadder(string word1, string word2, Set <string> dictionary);
Set<string> storeDictionary();
void playWordLadder(Set<string> dictionary);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    Set<string> dictionary = storeDictionary();
    introStatement();
    playWordLadder(dictionary);
    cout << "Have a nice day." << endl;
    return 0;
}

//Plays wordladder. This method executes the bulk of user interaction. Asks for two words, checks for
//user input errors on both words, and then creates a word ladder to display on the console.
void playWordLadder(Set<string> dictionary) {
    while (true) {
        HashSet<string> wordTracker;
        string word1 = getWord(1);
        if (word1 == "") break;
        string word2 = getWord(2);
        if (word2 == "") break;
        if (word1 == word2) cout << "The two words must be different." << endl;
        else if (word1.length() != word2.length())
            cout << "The two words must be the same length." << endl;
        else if (!dictionary.contains(word1) || !dictionary.contains(word2))
            cout << "The two words must be found in the dictionary." << endl;
        else {
            Stack<string> solution = createWordLadder(word1, word2, dictionary);
            if (solution.isEmpty()) {
                cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
            }
            else {
                cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
                while (!solution.isEmpty()) {
                    cout << solution.pop() << " ";
                }
                cout << endl;
            }
        }
        cout << endl;
    }
}

//Stores dictionary in a set. Returns this set.
Set<string> storeDictionary() {
    ifstream input;
    Set <string> dictionarySet;
    input.open("dictionary.txt");
    string line;
    while (true) {
        bool success = getline(input, line);
        if (!success) {
            break;
        }
        else {
            dictionarySet.add(line);
        }
    }
    return dictionarySet;
}



//Creates a word ladder between word1 and word2 using a Breadth-First search.
Stack<string> createWordLadder(string word1, string word2, Set <string> dictionary) {
    //This HashSet keeps track of all words previously used in the search.
    HashSet<string> wordTracker;
    wordTracker.add(word1);
    //create an empty queue of stacks.
    Queue<Stack<string>> queue;
    //creates first stack
    Stack<string> first;
    first.push(word1);
    queue.enqueue(first);
    while(!queue.isEmpty()) {
        Stack<string> tempStack = queue.dequeue();
        int length = word1.length();
        for (int i = 0; i < length; i++) {
            string currentWord = tempStack.peek();
            for (char letter = 'a'; letter <= 'z'; letter++) {
                string wordToCheck = currentWord;
                wordToCheck[i] = letter;
                if (wordToCheck == word2) {
                    tempStack.push(word2);
                    return tempStack;
                }
                else if (dictionary.contains(wordToCheck) && !wordTracker.contains(wordToCheck)) {
                    wordTracker.add(wordToCheck);
                    Stack<string> copyStack = tempStack;
                    copyStack.push(wordToCheck);
                    queue.enqueue(copyStack);
                }
            }
        }
    }
    return Stack<string>();
}

// Gets user input for word. Converts to lower case. Returns word. Passes in wordNumber
// as a parameter to notify user which word he is choosing (1 or 2).
string getWord(int wordNumber) {
    while (true){
        string word = toLowerCase(getLine("Word #" + integerToString(wordNumber) + " (or Enter to quit): "));
        return word;
    }
}

//intro statements (including directions)
void introStatement(){
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

