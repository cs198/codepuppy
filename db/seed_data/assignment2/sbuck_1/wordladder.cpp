// This is the CPP file that I edited and turned in.
// This file runs a word ladder that connects the second
// inputed word to the first.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"


using namespace std;
void welcomeMessage();
void ladder(string& first, string& second, Lexicon &dictionary);
void finalLadder(string& first, string& second, Lexicon& dictionary);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    welcomeMessage();
    Lexicon dictionary;
    dictionary.addWordsFromFile("dictionary.txt");
    string first;
    string second;
    ladder(first, second, dictionary);
    cout << "Have a nice day." << endl;
    return 0;
}
/**
 * @brief victory
 * @param finishedLadder
 * victory() prints out the final ladder that the
 * program found.
 */
void victory(Stack<string>& finishedLadder) {
    int a = finishedLadder.size();
    for(int i = 0; i < a; i++){
        cout << finishedLadder.pop() << " ";
    }
    cout << " " << endl;
}
/**
 * @brief finalLadder
 * @param first
 * @param second
 * @param dictionary
 * finalLadder() takes in the two strings and the dictionary
 * and finds the actual ladder.
 */
void finalLadder(string& first, string& second, Lexicon& dictionary) {
    Lexicon test;
    Queue<Stack<string>> queue;
    Stack<string> stack;
    stack.push(first);
    queue.enqueue(stack);
    int check = 0;
    while (!queue.isEmpty()) {
        Stack<string> currentLadder = queue.dequeue();
        string rung = currentLadder.peek();
        Stack<string> tempStack;
        for(int i = 0; i < rung.length(); i++) {
            for(char ch = 'a'; ch < 'z'; ch++){
                string trial = rung;
                trial = trial.replace(i, 1, charToString(ch));
                if(trial == second) {
                    currentLadder.push(second);
                    victory(currentLadder);
                    while (!queue.isEmpty()) {
                        queue.dequeue();
                    }
                    check = 1;
                    break;
                }else {
                    if(dictionary.contains(trial)==true && trial != rung) {
                        int x = currentLadder.size();
                        tempStack = currentLadder;
                        for(int w = 0; w < x; w++){
                             test.add(tempStack.pop());
                        }
                        if(!test.contains(trial)){
                            tempStack = currentLadder;
                            tempStack.push(trial);
                            queue.enqueue(tempStack);
                        }
                    }
                }
            }
        }
        if(check == 1)break;
    }
}
/**
 * @brief ladder
 * @param first
 * @param second
 * @param dictionary
 * ladder() takes in two blank strings and the dictionary
 * and then takes in the two words from the user and checks
 * them to make sure they are legal.
 */
void ladder(string& first, string& second, Lexicon& dictionary) {
    cout << "" << endl;
    while(true) {
        first = getLine("Word #1 (or Enter to quit): ");
        if(first == "")break;
        if(!dictionary.contains(first)) {
            cout << "Invalid entry: not a word" << endl;
            ladder(first, second, dictionary);
        }
        second = getLine("Word #2 (or Enter to quit): ");
        if(second == "")break;
        if(!dictionary.contains(second)) {
            cout << "Invalid entry: not a word" << endl;
            ladder(first, second, dictionary);
        }
        if(first.length() != second.length() || first == second){
            cout << "Invalid entry. Try again." << endl;
            ladder(first, second, dictionary);
        }
        first = toLowerCase(first);
        second = toLowerCase(second);
        finalLadder(first, second, dictionary);
        cout << "" << endl;
     }
}
/**
 * @brief welcomeMessage
 * welcomeMessage() runs the code to create the text at the
 * beggining of the program describing wordladder.
 */
void welcomeMessage() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;

}
