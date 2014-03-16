/*
 * Katie Hahm; Section leader: Jimmy Lee
 * Word Ladder
 * This method asks the user for two words, and if they are:
 * 1. both words in the dictionary
 * 2. both the same length
 * 3. not the same word,
 * the method returns the shortest sequence of words from
 * the second to first word obtained by changing one letter of the previous word.
*/

#include <string>
#include "console.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"
#include "lexicon.h"
using namespace std;

Set<string> neighborList(string& word, Lexicon& dictionary);
Stack<string> makeWordLadder(string& firstWord, string& secondWord, Lexicon& dictionary);
void printIntro();
bool conditionsAreTrue(string& firstWord, string& secondWord, Lexicon& dictionary);
string makeLowerCase(string& word);
void performWordLadder(string &firstWord, string &secondWord, Lexicon &dictionary);

/*
 * Prints the instructions to the user,
 * Makes a dictionary
 * while the user doesn't press Enter,
 * it asks the user for two words, and produces a word ladder if the conditions are met.
 * if the user presses Enter,
 * the program quits.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    Lexicon dictionary("dictionary.txt");
    while (true) {
        string firstWord = getLine("Word #1 (or Enter to quit): ");
        if (firstWord == "") {
            break;
        }
        string secondWord = getLine("Word #2 (or Enter to quit): ");
        if (secondWord == "") {
            break;
        }
        if (conditionsAreTrue(firstWord, secondWord, dictionary)) {
            performWordLadder(firstWord, secondWord, dictionary);
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

// prints the introductory messages
void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
}

/*
 * computes the word ladder sequence and prints it
 * parameter: the first and second word inputted by the user, and the dictionary
 * it converts the words to all lowercase,
 * and if a word ladder exists,
 * it prints the word ladder from the second word to the first.
 * if a word ladder doesn't exist, it prompts the user that it doesn't exist.
 */
void performWordLadder(string& firstWord, string& secondWord, Lexicon& dictionary) {
    firstWord = makeLowerCase(firstWord);
    secondWord = makeLowerCase(secondWord);
    if (!makeWordLadder(firstWord, secondWord, dictionary).isEmpty()) {
        cout << "A ladder from data back to code:" << endl;
        Stack<string> wordLadder = makeWordLadder(firstWord, secondWord, dictionary);
        string current = secondWord + " ";
        while(!wordLadder.isEmpty()){
            current += wordLadder.pop() + " ";
        }
        cout << current << endl << endl;
    }
    else {
        cout << "No word ladder found from " << secondWord << " back to " << firstWord << "." << endl << endl;
    }
}

/*
 * checks the conditions for the word ladder
 * parameter: the first and second word inputted by the user, and the dictionary
 * returns: true if the conditions are met, false if the conditions aren't met
 * it checks the conditions:
 * 1. both the words must be a valid word in the dictionary
 * 2. both words must be the same size
 * 3. the two words must not be the same word
 */
bool conditionsAreTrue(string& firstWord, string& secondWord, Lexicon& dictionary) {
    if (!dictionary.contains(firstWord) || !dictionary.contains(secondWord)) {
        cout << "The two words must be found in the dictionary." << endl << endl;
        return false;
    }
    if (firstWord.size() != secondWord.size()) {
        cout << "The two words must be the same length." << endl << endl;
        return false;
    }
    if (firstWord == secondWord) {
        cout << "The two words must be different." << endl << endl;
        return false;
    }
    return true;
}

/*
 * creates a set of possible neighboring words
 * parameter: word to get neighbors for, and the dictionary
 * returns: the set of strings that contain all valid neighboring words
 * it iterates through each letter in the word,
 * and tries all alphabets for that letter and checks if it is a valid word.
 */
Set<string> neighborList(string& word, Lexicon& dictionary) {
    Set<string> neighborlist;
    for (int i = 0; i < word.size(); i++) {
        for (char j = 'a'; j <= 'z'; j++) {
            string newWord = word;
            newWord[i] = j;
            if (dictionary.contains(newWord)) {
                neighborlist.add(newWord);
            }
        }
    }
    return neighborlist;
}

/*
 * converts the word to all lowercase
 * parameter: the word to be converted
 * returns: the word in all lowercase letters
 */
string makeLowerCase(string& word) {
    for (int i = 0; i < word.size(); i++) {
        char c = word[i];
        c = tolower(c);
        word[i] = c;
    }
    return word;
}

/*
 * makes a word-ladder using breadth-first search
 * parameter: the first and second word inputted by the user, and the dictionary
 * returns: the stack of strings that contains the shortest possible word-ladder
 */
Stack<string> makeWordLadder(string& firstWord, string& secondWord, Lexicon& dictionary) {
    Queue<Stack<string> > ourqueue;
    Stack<string> ourstack;
    Set<string> usedWords;
    ourstack.push(firstWord);
    ourqueue.enqueue(ourstack);
    while (!ourqueue.isEmpty()) {
        Stack<string> currentStack = ourqueue.dequeue();
        string topWord = currentStack.peek();
        for (string word : neighborList(topWord, dictionary)) {
            if (!usedWords.contains(word)) {
                usedWords.add(word);
                if (word == secondWord) {
                    return currentStack;
                }
                else {
                    Stack<string> copyStack = currentStack;
                    copyStack.push(word);
                    ourqueue.enqueue(copyStack);
                }
            }
        }
    }
    return Stack<string>();
}
