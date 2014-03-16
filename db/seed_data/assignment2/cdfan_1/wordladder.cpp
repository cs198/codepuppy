// This program implements WordLadder
// It asks for two words from the user and builds a word ladder between them by changing only one letter per step


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "simpio.h"
#include "console.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "hashset.h"

using namespace std;

// prototypes
void welcomeMsg();
bool seekTwoWords(Set<string> & dictionary, string & word1, string & word2);
void readDictionary(Set<string> & dictionary);
Stack<string> seekShortestLadder(Set<string> & dictionary, string word1, string word2);
void printSolution (Stack<string> s);
Set<string> findNeighbors(Set<string> & dictionary , string word);


int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    welcomeMsg();
    Set<string> dictionary;
    readDictionary(dictionary);

    while(true) { 
        string word1;
        string word2;
        if(!seekTwoWords(dictionary, word1, word2)) break;

        Stack<string> solution = seekShortestLadder(dictionary, word1, word2);
        if(!solution.isEmpty()){
            cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
            printSolution(solution);
        } else {
            cout << "No word ladder found from " << word2 << " back to " << word1 << endl;
            cout<< "" << endl;
        }

    }

    cout << "Have a nice day." << endl;
    return 0;
}

void welcomeMsg() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." <<endl;
    cout << "" << endl;
}

// Read the file word by word into a Set<string> dictonary
void readDictionary(Set<string> & dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while(true) {
        input >> word;
        if(input.fail()) break;
        dictionary += word;
    }
    input.close();
}

// Ask users to enter two valid english words. word2 has to be in same length, different English word from word1.
//if empty string is entered, it quits the program
bool seekTwoWords(Set<string> & dictionary, string & word1, string & word2) {
    word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
    while (!dictionary.contains(word1) && word1 != "")  word1 = getLine("Not valid word. Try again. Word #1 (or Enter to quit): ");
    if(word1 =="") return false;

    word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    while (!dictionary.contains(word2) || word1 == word2 || word2.length() !=word1.length() ) {
        if(word2 == "") return false;
        word2 = getLine("The two words must be found in the dictionary, in same length but different words. Try again. Word #2 (or Enter to quit): ");
    }
    if(word2 =="") return false;
    return true;
}

//creates a queue of stacks. The queue stores all the potential ladders; each stack is a potential word ladder. It
//also creats a set to store all the used words in the ladders. start by examining ladders that are one step away
//from the original word, where only one letter is changed.
Stack<string> seekShortestLadder(Set<string> & dictionary, string word1, string word2) {
    Queue< Stack<string> > possibleLadders;
    Stack<string> s;
    HashSet<string> used;
    s.push(word1);
    used.add(word1);
    possibleLadders.enqueue(s);

    /*
     * while the queue is not empty, dequeue the partial-ladder stack from the front of the queue.for each valid
     * English word that is a "neighbor" (differs by 1 letter) of the word on top of the stack.
     * If that neighbor word has not already been used in a ladder before, and  if the neighbor word is w2,
     * this is the shortest successful ladder and return. Otherwise,create a copy of the current partial-ladder stack.
     * Put the neighbor word on top of the copy stack and add the copy stack to the end of the queue.
     */
    while(!possibleLadders.isEmpty()) {
        Stack<string> s = possibleLadders.dequeue();
        string topWord = s.peek();
        Set<string> neighbors = findNeighbors(dictionary, topWord);

        for(string word: neighbors) {
            if(!used.contains(word)) {
                Stack<string> newStack = s;
                newStack.push(word);
                if(word == word2){
                    return newStack;
                }
                possibleLadders.enqueue(newStack);
                used.add(word);
            }
        }
    }
    // if none succesful ladder is found, return an empty Stack.
    Stack<string> empty;
    return empty;
}

//Finds all possible neighbors of a given word by going through double for loops of the word index and the alphabet
Set<string> findNeighbors(Set<string> & dictionary , string word) {
    Set<string> neighbors;
    string alphabet= "abcdefghijklmnopqrstuvwxyz";
    for(unsigned i=0; i < word.length(); i++){
       for(char ch: alphabet) {
           string copy = word;
           copy[i]=ch;
           if(dictionary.contains(copy)) neighbors += copy;
       }
    }
    return neighbors;
}

//print a stack of works
void printSolution (Stack<string> s) {
    string line;
    while(!s.isEmpty()) {
         line += s.pop() + " ";
    }
    cout << line << endl;
    cout<< "" << endl;
}
