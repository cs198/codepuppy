// wordladder.cpp
// This program finds the shortest word ladder between any two words of the same length.
// A word ladder is defined to be changing one letter of the first word repeatedly until the first word equals the second word.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "hashset.h"

using namespace std;

/*Function Prototypes*/
Lexicon createDictionary();
void printIntro();
void getUserWords(string& word1, string& word2);
Queue<Stack <string> > createQueue(string word);
Stack<string> getShortestLadder(Queue<Stack<string> >& wordLadder, string word2, string word1, Lexicon& dictionary);
Stack <string> flipStack(Stack <string>& ladderStack);
void printLadder(Stack <string>& ladderStack);
bool isValidInput(string word1, string word2, Lexicon& dictionary);
void printError(string word1, string word2, Lexicon& dictionary);
void checkLadder(Stack<string>& ladderStack, string word1, string word2);
void findLadder(string word1, string word2, Lexicon& dictionary);
void addToStack(string& tempWord, Queue<Stack <string> >& wordLadders, HashSet<string>& usedWords, Stack<string>& tempStack);
bool isValidNextWord(string word2, string tempWord, Lexicon& dictionary, HashSet<string>& usedWords);

/*
 * Program main
 * The while true loop runs until the user presses enter on the word #1 or word #2 prompt (an empty string).
 * If the user enters strings other than empty strings, the program will attempt to find the corresponding word ladder.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    Lexicon dictionary = createDictionary();
    string word1;
    string word2;
    while(true) {
        cout << "Word #1 (or Enter to quit): ";
        getline(cin, word1);
        if(word1 != "") {
            cout << "Word #2 (or Enter to quit): ";
            getline(cin, word2);
            if(word2 == "") {
                cout << "Have a nice day." << endl;
                break;
            }
        }
        else {
            cout << "Have a nice day." << endl;
            break;
        }
        findLadder(toLowerCase(trim(word1)), toLowerCase(trim(word2)), dictionary);
        cout << endl;
    }
    return 0;
}

/*
 * Given two words, this function will find the shortest ladder between these two words if the words are valid.
 * It finds the shortest ladder by first creating a queue with 1 stack containing word2 and then passing that to the
 * getShortestLadder function.
 * If the words are invalid, an error message will be printed.
 */
void findLadder(string word1, string word2, Lexicon& dictionary) {
    if(isValidInput(word1, word2, dictionary)) {
        Queue<Stack<string> > wordLadders = createQueue(word2);
        Stack<string> ladderStack = getShortestLadder(wordLadders, word2, word1, dictionary);
        checkLadder(ladderStack, word1, word2);
    }
    else {
        printError(word1, word2, dictionary);
    }
}

/*
 * Checks if the ladder stack contains any elements. If the ladder contains no elements, there is no word ladder for these two words.
 * Otherwise, a word ladder exists and it is printed to the console.
 */
void checkLadder(Stack<string>& ladderStack, string word1, string word2) {
    if(ladderStack.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
    }
    else {
        cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
        printLadder(ladderStack);
    }
}

/*
 * Prints one of 3 error messages based on the words entered. An error will be printed for the following conditions:
 * The two words are not in the dictionary, the two words are the same, the two words are of differnet length.
 *
 */
void printError(string word1, string word2, Lexicon& dictionary) {
    if(!(dictionary.contains(word1) && dictionary.contains(word2))) {
        cout << "The two words must be found in the dictionary." << endl;
    }
    else if(word1 == word2) {
        cout << "The two words must be different." << endl;
    }
    else {
        cout << "The two words must be the same length." << endl;
    }
}

/*
 * Returns true if the two words are in the dictionary, are not the same words and are the same length.
 */
bool isValidInput(string word1, string word2, Lexicon& dictionary) {
    return (dictionary.contains(word1) && dictionary.contains(word2)) && (word1 != word2) &&
            (word1.length() == word2.length());
}

/*
 * Prints the word ladder in the reverse order of the original stack of words.
 */
void printLadder(Stack <string>& ladderStack) {
    Stack<string> flippedStack = flipStack(ladderStack);
    while(flippedStack.size() != 0) {
        if(flippedStack.size() == 1) {
            string word = flippedStack.pop();
            cout << word << " " << endl;
        }
        else {
            string word = flippedStack.pop();
            cout << word << " ";
        }
    }
}

/*
 * Returns a stack in the reverse order of the original stack.
 */
Stack <string> flipStack(Stack <string>& ladderStack) {
    Stack <string> flippedStack;
    while(ladderStack.size() != 0){
        string word = ladderStack.pop();
        flippedStack.push(word);
    }
    return flippedStack;
}

/*
 * Accepts a queue with a single stack with one string, word1, word2 and the dictionary.
 * While the queue is not empty, the first stack will be taken out and the top word will be used. For every index of the word, it will be replaced with
 * every index of the alphabet. If the new word (replaced with a letter) is equal to word1, that means the word ladder is complete and the stack with the new word
 * at the top of the stack will be returned. If the word is a valid next word in the ladder, it will be added to the stack.
 * If the queue is empty (which means no word ladder was found), the function will return an empty stack.
 */
Stack<string> getShortestLadder(Queue<Stack<string> >& wordLadders, string word2, string word1, Lexicon& dictionary) {
    HashSet<string> usedWords;
    char letter  = 'a';
    while(wordLadders.size() != 0) {
        Stack<string> stack = wordLadders.dequeue();
        Stack<string> tempStack = stack;
        word2 = stack.peek();
        string tempWord = word2;
        for(int i = 0; i < word2.length(); i++) {
            for (int j = 0; j < 26; j++) {
                tempWord[i] = (char)(letter + j);
                if(tempWord == word1) {
                    stack.push(tempWord);
                    return stack;
                }      
                if(isValidNextWord(word2, tempWord, dictionary, usedWords)) {
                    addToStack(tempWord, wordLadders, usedWords, tempStack);
                }
                    tempStack = stack;
                    tempWord = word2;
            }
        }
    }
    Stack<string> emptyStack;
    return emptyStack;
}

/*
 * Returns true if tempWord (an alteration of word2) is not equal to word2, is in the dictionary and is not in the UsedWords set.
 * The UsedWords set contains all words that have already been added to a ladder.
 */
bool isValidNextWord(string word2, string tempWord, Lexicon& dictionary, HashSet<string>& usedWords) {
    return (tempWord != word2 && dictionary.contains(tempWord) && !usedWords.contains(tempWord));
}

/*
 * Pre-conditions: tempWord is a valid word next on the ladder.
 * Adds tempWord to tempStack and then places tempStack back into the queue of stacks of word ladders.
 * Adds the tempWord to the usedWords set since it is being used in a ladder.
 */
void addToStack(string& tempWord, Queue<Stack <string> >& wordLadders, HashSet<string>& usedWords, Stack<string>& tempStack) {
    tempStack.push(tempWord);
    wordLadders.enqueue(tempStack);
    usedWords.add(tempWord);
}

/*
 * Creates the original queue with only one stack containing one word.
 */
Queue<Stack <string> > createQueue(string word) {
    Queue<Stack<string> > wordLadders;
    Stack<string> stack;
    stack.push(word);
    wordLadders.enqueue(stack);
    return wordLadders;
}

/*
 * Prints the introductory message at the begining of the program.
 */
void printIntro() {
    cout << "Welcome to the CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

/*
 * Returns a Lexicon that is read from the "dictionary.txt" file.
 */
Lexicon createDictionary() {
    Lexicon dictionary;
    dictionary.addWordsFromFile("dictionary.txt");
    return dictionary;
}
