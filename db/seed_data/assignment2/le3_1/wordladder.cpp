// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

// print the introduction words
void intro () {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time" << endl;
    cout << endl;
}

// print the wordds in stack
void printStack(Stack<string>& pstack) { // print from top to bottom
    while(!pstack.isEmpty()){
        cout << pstack.pop() << " ";
    }
    cout << "" << endl;
    cout << endl;
}

// input old and new words and dictrionary. get the word ladder
Stack<string> solutionStack (string word1, string word2, Lexicon& dictionary) {
    Stack<string> oldWord;
    oldWord.push(word1);
    Queue<Stack<string>> q; // q to store all the word ladder steps
    q.enqueue(oldWord); // put the first word
    Set<string> set;
    set.add(word1); // build a set to check whether a word exists or not
    Stack<string> stackFront;

    while (!q.isEmpty()){
        stackFront = q.dequeue();
        string NeighTopStack = stackFront.peek();
        int wordSize = NeighTopStack.size();
        for (int i = 0 ; i < wordSize; i++ ){ // from the first char to last
            for (char c = 'a'; c <= 'z'; c++){ // change from a to z
                string newWord = NeighTopStack;
                newWord[i] = c;
                if (dictionary.contains(newWord)){
                    if (!set.contains(newWord)){
                      if (newWord == word2)  { // get the word2
                          stackFront.push(newWord);
                          return stackFront;
                      } else if(newWord != word2) { // put the new word in the ladder
                          Stack<string> stackCopy = stackFront;
                          stackCopy.push(newWord);
                          q.enqueue(stackCopy);
                      }
                       set.add(newWord);
                    }
                }
            }
        }
    }
}

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    // TODO: Finish the program!
    intro();
    Lexicon dict;
    dict.addWordsFromFile("dictionary.txt");
    while(true){
        // get the word1. lower case it and trim it
        string word1 = getLine("Word #1 (or Enter to quit):");
        int l1;
        if (word1 == ""){
            break;
        } else{
            word1 = toLowerCase(word1);
            word1 = trim(word1);
            l1 = word1.size();
        }
        // get the word2. lower case it and trim it
        string word2 = getLine("Word #2 (or Enter to quit):");
        int l2;
        if (word2 == ""){
            break;
        }else{
            word2 = toLowerCase(word2);
            word2 = trim(word2);
            l2 = word2.size();
        }
        if (!dict.contains(word1) || !dict.contains(word2)){ // check word1&2 are in the dict or not
            cout << "The two words must be found in the dictionary." << endl;
            cout << endl;
        }else{
            if (l1 != l2){ // check word1&2 length same or not
                cout << "The two words must be the same length." << endl;
                cout << endl;
            } else if(l1 == l2){
                if (word1 == word2){
                    cout << "The two words must be different." << endl; // check the two words are same or not
                    cout << endl;
                }else{
                    Stack<string> finalStack = solutionStack(word1, word2, dict);
                    cout << "A ladder from " << word1 << " back to " << word2 << endl;
                    printStack(finalStack);
                }
            }
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}
