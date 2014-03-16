/*Filename: Word Ladder by Matt Herrero
 * -----------------------------------------------------------------------------------
 * This program generates word ladders from 2 words given by the user. A word ladder
 * consists of a set of words that starts at one designated word and, by changing
 * only one letter of the word at a time, ends with another designated word. This
 * program is designed to find the shortest word ladders between words based on the
 * words inputed by the user.
 */


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "queue.h"
#include "stack.h"
#include "set.h"
#include "lexicon.h"
#include "console.h"

using namespace std;

//These are the prototyped methods for the program

void printIntro();
bool retrieveStartWords(string &word1, string &word2, Lexicon englishWords);
bool checkForWordErrors(string& word1, string& word2, Lexicon& englishWords);
void getNeighborsQueue(Stack<string> originLadder, Queue<string>& allNeighbors, Lexicon& englishWords);
void printFinalLadder(Stack<string>& finalLadder, string word1, string word2);

/*
 * This is the main method for the program. It begins by creating a lexicon initialed
 * to an list of english words, then prints the introductory message to the user.
 * Then, collections that will be used in generating the word ladders are declared as
 * well as the strings for the beginning and ending words of the program. These words
 * are retrieved from the user, and if the user has input an empty string, then the
 * program quits. If the user has not quit, then the starting word is put into a
 * stack representing the start of the word ladder, which is then enqueued in a queue
 * tracking all possible word ladders. Also, the word is added to a set tracking which
 * words have already been used in the queue of possible ladders. Then the program
 * iterates over every possible ladder stack in the possibleLadders queue and gets the
 * possible neighbor words for the most recent word in that ladder. This ladder is copied
 * so that all new neighbors are added to the end of new ladders. This process is done
 * to every ladder in the possibleLadders queue until the ending word is found to be
 * a neighbor. When this happend, the ladder is passed to a method that prints the ladder
 * and the program asks the user for new words.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    Lexicon englishWords("dictionary.txt");

    printIntro();
    while(true) {
        Queue<Stack<string> > possibleLadders;
        Stack<string> initialLadder;
        Set<string> usedNeighbors;
        Stack<string> finalLadder;
        Queue<string> allNeighbors;
        string word1;
        string word2;

        bool userQuiting = retrieveStartWords(word1, word2, englishWords);
        if (userQuiting) break;
        initialLadder.push(word1);
        possibleLadders.enqueue(initialLadder);
        usedNeighbors.add(word1);
        while(!possibleLadders.isEmpty()) {
            Stack<string> originLadder = possibleLadders.dequeue();
            getNeighborsQueue(originLadder, allNeighbors, englishWords);
            while(!allNeighbors.isEmpty()) {
                string neighbor = allNeighbors.dequeue();
                if(!usedNeighbors.contains(neighbor)){
                    if(neighbor == word2) {
                        originLadder.push(neighbor);
                        finalLadder = originLadder;
                        possibleLadders.clear();
                        break;
                    } else {
                        Stack<string> nextLadder = originLadder;
                        nextLadder.push(neighbor);
                        possibleLadders.enqueue(nextLadder);
                        usedNeighbors.add(neighbor);
                    }
                }
            }
        }
        printFinalLadder(finalLadder, word1, word2);
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * This method prints the welcome message to the user.
 */
void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
}

/*
 * This method asks retrieves the starting and ending words from the user. First, the
 * program prompts and reprompts for word1 to be entered. When a valid input for word1
 * has been recieved, the program checks if it is an empty string. If so, the bool
 * userQuiting is made true and returned. If not, word2 is retreived and handled
 * similarly as word1.
 */
bool retrieveStartWords(string &word1, string &word2, Lexicon englishWords) {
    bool userQuiting = false;
    while(true){
        cout << endl;
        cout << "Word #1 (or Enter to quit): ";
        getline(cin, word1);
        if (word1 == ""){
            userQuiting = true;
            break;
        }
        cout << "Word #2 (or Enter to quit): ";
        getline(cin, word2);
        if (word2 == ""){
            userQuiting = true;
            break;
        }
        if(checkForWordErrors(word1, word2, englishWords)){
            break;
        }
    }
    return userQuiting;
}

/*
 * This method checks for various errors in the inputed words. If errors are found
 * it returns the bool false and prints the appropraite error message. If no
 * errors are found, it returns the bool true. Also, the two words passed are changed
 * to lower case versions of themselves.
 */
bool checkForWordErrors(string& word1, string& word2, Lexicon& englishWords) {
    bool wordsUsable = true;
    word1 = toLowerCase(word1);
    word2 = toLowerCase(word2);

    if(!(englishWords.contains(word1) && englishWords.contains(word2))) {
        cout << "The two words must be found in the dictionary." << endl;
        wordsUsable = false;
    } else if (word1.size() != word2.size()) {
        cout << "The two words must be the same length." << endl;
        wordsUsable = false;
    } else if(word1 == word2){
        cout << "The two words must be different." << endl;
        wordsUsable = false;
    }
    return wordsUsable;
}
/*
 * This method clears fills the queue allNeighbors with the neighbors for the top word in
 * the ladder passed to the program as the originLadder parameter. It uses this word and
 * changes each letter of the word independently to every other letter in the alphabet,
 * storing the legal english words that it comes across in the allNeighbors queue.
 */
void getNeighborsQueue(Stack<string> originLadder, Queue<string>& allNeighbors, Lexicon& englishWords) {
    allNeighbors.clear();
    string originWord = originLadder.pop();
    for(int i = 0; i < (int)originWord.size(); i++) {
        char currentChar = originWord[i];
        for(char j = 'a'; j <= 'z'; j++) {
            if(j != currentChar) {
                originWord[i] = j;
                if (englishWords.contains(originWord)) {
                    allNeighbors.enqueue(originWord);
                }
            }
        }
        originWord[i] = currentChar;
    }
}

/*
 * This method prints the final word ladder in the console. If the method is passed an
 * empty set (which means no word ladder exists), the method prints out a message telling
 * the user that there is no word ladder between the given words.
 */
void printFinalLadder(Stack<string>& finalLadder, string word1, string word2) {
    if(finalLadder.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to " << word1 << ".";
    } else {
        cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
        while(!finalLadder.isEmpty()) {
            cout << finalLadder.pop() << " ";
        }
    }
    cout << endl;
}
