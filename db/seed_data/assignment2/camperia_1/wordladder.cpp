// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "hashset.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"
#include "console.h"
#include "filelib.h"

using namespace std;

Stack<string> getWordLadder(string w1 , string w2 , HashSet<string>& dictionary);
HashSet<string> getNeighbors(string word , HashSet<string>& dictionary);
bool getWords(string& w1 , string& w2 , HashSet<string>& dictionary);
bool checkForValidity(string w1 , string w2 , HashSet<string>& dictionary);
void printLadder(string w1 , string w2 , Stack<string> ladder);
void getDictionary(HashSet<string>& dictionary);
void printWelcomeMessage();

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    printWelcomeMessage(); //prints welcome message

    HashSet<string> dictionary; //initializes the dictionary hashset
    getDictionary(dictionary); //gets the dictionary (puts the words in the hashset

    //initializes the strings
    string w1;
    string w2;

    while(getWords(w1 , w2 , dictionary)){ //gets the words from the user
        //starts the actual program and loops while the user doesn't hit enter (empty input)
        Stack<string> ladder = getWordLadder(w1 , w2 , dictionary); //gets the word ladder
        printLadder(w1 , w2 , ladder); //prints out the ladder with the messages
    }

    cout << "Have a nice day." << endl;
    return 0;
}

void printWelcomeMessage(){
    cout << "Welcome to CS 106B Word Ladder.\n"
            "Please give me two English words, and I will change the\n"
            "first into the second by changing one letter at a time.\n" << endl;
}

void getDictionary(HashSet<string>& dictionary){
    //gets the dictionary and puts all the word sin the hashset passed by reference
    ifstream wordList;
    string filename = "dictionary.txt";
    string line;
    openFile(wordList , filename);
    while(getline(wordList , line)){
        //adds all the words to the hashset
        dictionary.add(line);
    }
}

bool getWords(string& w1 , string& w2 , HashSet<string>& dictionary){
    //gets the words
    //returns true if the input is valid and false if the user entered an empty string

    do{ //iterates while the input is invalid
        w1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (w1 == "")return false;
        w2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (w2 == "") return false;
    } while (!checkForValidity(w1 , w2 , dictionary)); //checks for the validity of the words
    return true;
}

bool checkForValidity(string w1 , string w2 , HashSet<string>& dictionary){
    //checks for the validity of the words

    if (dictionary.contains(w1) && dictionary.contains(w2)){
        //if the words exist
        if (w1.size() == w2.size()){
            //if they're the same length
            if (w1 != w2){
                //if they aren't the same
                return true;
            } else{
                cout << "The two words must be different.\n" << endl;
                return false;
            }
        } else{
            cout << "The two words must be the same length.\n" << endl;
            return false;
        }
    } else{
        cout << "The two words must be found in the dictionary.\n" << endl;
        return false;
    }
}

Stack<string> getWordLadder(string w1, string w2 , HashSet<string>& dictionary){
    //gets the word ladder

    HashSet<string> usedWords; //creates a hashet with the used words
    usedWords.add(w1); //adds the first word to it so it won't loop back
    Queue<Stack<string> > partials; //initiates the queue of partial ladder stacks
    Stack<string> s1; //starts the first stack
    s1.push(w1); //adds the first word to it
    partials.enqueue(s1); //enqueues it to the queue
    while(!partials.isEmpty()){ //while it's not empty
        //it will empty out eventually if there's no ladder
        //because we will run out of neighbors to test
        Stack<string> s = partials.dequeue(); //dequeues the first string
        HashSet<string> neighbors = getNeighbors(s.peek() , dictionary); //gets the neighbors of the first string
        for (string neighbor : neighbors){
            //for each neighbor of the string
            if (!usedWords.contains(neighbor)){
                //if the neighbor hasn't been used
                if (neighbor == w2){
                    //YAY LADDER FOUND
                    s.push(w2);
                    return s;
                } else{
                    //if the word isn't the neighbor
                    Stack<string> copy = s; //creates a copy of the string
                    copy.push(neighbor); //pushes the neighbor in it
                    partials.enqueue(copy); //adds it to the queue
                    usedWords.add(neighbor); //adds the word to the usedwords hashset
                }
            }
        }
    }
    //if we ran out of words to test, it means no ladder exists, and it returns an empty ladder
    Stack<string> empty = Stack<string>();
    return empty;
}

HashSet<string> getNeighbors(string word , HashSet<string>& dictionary){
    //gets the neighbors of the word

    HashSet<string> neighbors; //starts a hashset of strings
    int length = word.length(); //length of the word
    for (int i = 0 ; i < length ; i++){
        //for each letter in the word
        string temp = word; //creates a temporary word
        for (char letter = 'a' ; letter <= 'z' ; letter++){
            //for each character in the alphabet
            temp[i] = letter; //replaces the letter of the word with the current char
            if (dictionary.contains(temp)){
                //if the word exists
                neighbors.add(temp); //adds the word to the neighbors hashset
            }
        }
    }
    return neighbors; //returns the hashset of neighbors
}

void printLadder(string w1 , string w2 , Stack<string> ladder){
    //prints the ladder
    if(ladder.isEmpty()){
        //if it's empty we found no ladder
        cout << "No word ladder found from " << w2 << " back to " << w1 << ".";
    } else{
        cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
        while(!ladder.isEmpty()){
            //prints every word in the ladder stack
            cout << ladder.pop() << " ";
        }
    }
    cout << endl << endl; //skips 2 lines
}







