//Author: Mark Stauber
//Date: 1/20/14
//Course: CS 106B Win' 14
//File: wordladder.cpp
//Description: This program will take any two words in the english
//dictionary and output the shortest word ladder that connects them
//by changing only one letter in the word at a time, with each step itself
//an english word.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"

using namespace std;

bool userInput(string& start, string& end, Lexicon& dictionary);
bool findLadder(Stack<string>& ladder, string end, Lexicon& dictionary);
void printResult(bool ladderFound, string start,
                 string end, Stack<string>& ladder);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    // Start and end of word ladder
    string start;
    string end;
    Stack<string> ladder;

    //Initialize the dictionary with dictionary.txt info.
    Lexicon dictionary("dictionary.txt");

    cout<<"Welcome to CS 106B Word Ladder."<<endl
        <<"Please give me two English words, and I will change the"<<endl
        <<"first into the second by changing one letter at a time."<<endl;

    //Until User chooses to end program (i.e. userInput returns false)
    while(userInput(start, end, dictionary)){
        //Reset ladderFound flag
        bool ladderFound = false;

        //Put start word on ladder.
        ladder.push(start);
        //Complete ladder if exists.
        ladderFound = findLadder(ladder, end, dictionary);
        //Print out ladder.
        printResult(ladderFound, start, end, ladder);
        //Reset ladder.
        ladder.clear();
    }

    cout << "Have a nice day." << endl;
    return 0;
}

//Get the start and end of the ladder.
bool userInput(string& start, string& end, Lexicon& dictionary){
    //Until user enters valid start and end words/
    while(1){
        cout<<endl<<"Word #1 (or Enter to quit): ";
        getline(cin, start);
        //if user presses enter (start is empty) then exit game
        if(!start.length()) return false;

        cout<<"Word #2 (or Enter to quit): ";
        getline(cin, end);
        //if user presses enter (start is empty) then exit game
        if(!end.length()) return false;

        //Check for valid input
        if(!dictionary.contains(start)||!dictionary.contains(end)){
            cout<<"The two words must be found in the dictionary."<<endl;
        }else if(start.length() != end.length()){
            cout<<"The two words must be the same length."<<endl;
        }else if( (start = toLowerCase(start)) == (end = toLowerCase(end))){
            cout<<"The two words must be different."<<endl;
        }else{
            return true; //Success
        }
    }
}

//Find the ladder from start to end.
bool findLadder(Stack<string>& ladder, string end, Lexicon& dictionary){
    //Create Queue
    Queue<Stack<string>> parLadders;
    Set<string> usedWords;
    int length = (int)end.length();

    //Add start to usedWords
    usedWords.add(ladder.peek());

    //Add ladder to Queue
    parLadders.enqueue(ladder);

    // While queue is not empty.
    while(!parLadders.isEmpty()){
        //Deqeue current stack.
        Stack<string> myStack = parLadders.dequeue();
        string myWord = myStack.peek();

        // Search for neighbors.
        for(int i = 0; i < length; i++){
            for(char letter = 'a'; letter <= 'z'; letter++){
                string testWord = myWord;
                testWord[i] = letter;

                //if testWord is a valid word
                if(dictionary.contains(testWord)){
                    //If we have not already used testWord in a stack
                    if(!usedWords.contains(testWord)){
                        //If test word is end of ladder...success!
                        if(testWord == end){
                            myStack.push(testWord);
                            ladder = myStack;
                            return true;
                        }else{
                            // copy current stack
                            Stack<string> newStack = myStack;
                            // put neighbor on top
                            newStack.push(testWord);
                            // add new stack to queue
                            parLadders.enqueue(newStack);
                            // add neigher to set of things we stacked
                            usedWords.add(testWord);
                        }
                    }
                }
            }
        }
    }
    return false;
}

//Print the ladder if one was found.
void printResult(bool ladderFound, string start,
                 string end, Stack<string>& ladder){
    if(ladderFound){
        cout<<"A ladder from "<<end<<" back to "<<start<<":"<<endl;
        while(!ladder.isEmpty()){
            cout<<ladder.pop()<<" ";
        }
        cout<<endl;
    }else{
        cout<<"No word ladder found from "<<end
           <<" back to "<<start<<"."<<endl;
    }
}

