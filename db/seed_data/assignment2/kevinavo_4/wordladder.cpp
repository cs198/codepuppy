/*
Name: Kevin Vo
TA: Jimmy lee

Summary: The program implements a game called WordLadder which finds a path
from one word to the next by alternating one letter each step. The user inputs
two words and the program prints out the resulting chain/ladder.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "lexicon.h"
#include "queue.h"
#include "stack.h"
#include "strlib.h"


using namespace std;


void BFS(string word1, string word2, Lexicon &dict);
void prompt(Lexicon& dict);
Stack<string> createPartialLadder(string word2, Lexicon& dict, Queue<Stack<string>>& queue);
void ladderPrint(Stack<string> &stack);
void endingScreen(string word1, string word2, Stack<string> success);
void welcomeScreen();

int main() {
    welcomeScreen();
    Lexicon dict;
    dict.addWordsFromFile("dictionary.txt");
    prompt(dict);
    return 0;
}

/* This void function simply sets up the console window and prints out the welcome
 * screen text.
*/
void welcomeScreen(){
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    cout << "Welcome to CS 106B Word Ladder" << endl;
    cout << "Please give me two English word, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

/*
 The prompt function is responsible for taking inputs fron the user and handles
 it also calls BFS which stands for Breadth first search.
*/

void prompt(Lexicon& dict){
    while (true){
        string word1 = getLine("Word #1 (or Enter to quit): ");
        word1 = trim(toLowerCase(word1));
        if (word1 == "") break;

        string word2 = getLine("Word #2 (or Enter to quit): ");
        word2 = trim(toLowerCase(word2));
        if (word2 == "") break;

        if (word1.length() != word2.length()){
            cout<< "Words must be the same length."<<endl;
        } else if (word1 == word2){
            cout << "The two words must be different." << endl;
        } else if (!dict.contains(word1) && !dict.contains(word2)){
            cout << "The two words must be found in the dictionary." << endl;
        }else{
            BFS(word1,word2, dict);
        }
         cout<<endl;
    }
    cout << "Have a nice day." << endl;
}
/*
 * The function puts the first word in the stack and enqueues it to start. It then calls
 * two functions createPartialLadder and endingScreen.
*/
void BFS (string word1, string word2, Lexicon& dict){
    Queue<Stack<string>> queue;
    Stack<string> stack;
    stack.push(word1);
    queue.enqueue(stack);
    Stack <string> success = createPartialLadder(word2, dict, queue);
    endingScreen(word1, word2, success);
}

/*
This function is responsible for outputting the successful ladder. It also catches
the edge case where there is no possible ladder.
*/

void endingScreen(string word1, string word2, Stack<string> success){
    string output;
    while (!success.isEmpty()){
        output += success.pop();
        output += " ";
    }
    if (output != ""){
        cout << "A ladder from " << word1 << " back to " << word2 << ":" <<endl;
        cout<<output<<endl;
    } else {
        cout << "No word ladder found from " << word1 << " back to " << word2 <<endl;
    }
}

/*
    This function implements BFS by creating neighbors that are 1 letter apart then
    2 letters apart until it hits the right word. It also keeps
    track of the words that were already used, using a Lexicon as a data structure.
*/

Stack<string> createPartialLadder(string word2, Lexicon& dict, Queue<Stack<string>>& queue){
    Lexicon usedWords;
    while (!queue.isEmpty()){
        Stack<string> partialLadder = queue.dequeue();
        string word1 = partialLadder.peek();
        usedWords.add(word1);
        if (word1 == word2){
            return partialLadder;
        }
        for (int j = 0; j < word1.length(); j++){
            string word = word1;
            for (char ch = 'a'; ch <= 'z'; ch++){
                word[j] = ch;
                if (dict.contains(word) && !usedWords.contains(word)){
                    Stack<string> copy = partialLadder;
                    copy.push(word);
                    queue.enqueue(copy);
                    usedWords.add(word);
                }
           }
        }
    }
    //if the algorithm does not find any possible word ladder it returns an empty stack
    Stack <string> empty;
    return empty;
}






