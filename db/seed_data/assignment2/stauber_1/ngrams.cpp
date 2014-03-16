//Author: Mark Stauber
//Date: 1/20/14
//Course: CS 106B Win' 14
//File ngrams.cpp
//Description: Program takes a user chosen input file and creates a hashmap
//of N-grams in the file, with N chosen by user as well. Program can then output
//any number words (greater than N) in patterns that appear to be coming from
//the author of the input file.
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <hashmap.h>
#include <vector.h>
#include <queue.h>
#include <random.h>
#include <simpio.h>
#include <filelib.h>
#include <console.h>
#include <tokenscanner.h>

using namespace std;

void getMapInput(string& inputFile, int& N);
void loadGramMap(string inputFile, HashMap<Queue<string>, Vector<string>>& gramMap,
                 int N);
int getNumWords(int N);
void printWords(int numWords, HashMap<Queue<string>, Vector<string>>& gramMap);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    string inputFile; //File containing author input
    int N;  //Number of words in our N-Grams
    int numWords = 1; //Number of words to print out
    HashMap<Queue<string>, Vector<string>> gramMap; //Our N-Gram Map

    //Get input file and N from User
    getMapInput(inputFile, N);
    //Create the N-Gram map from input file
    loadGramMap(inputFile, gramMap, N);

    //While user has not set N to 0 (indicating exit).
    while( (numWords = getNumWords(N)) ){
        //Print numWords using our N-Gram map
        printWords(numWords, gramMap);
    }

    cout << "Exiting." << endl;
    return 0;
}

//Gets inputFile name and N of N-Grams from user
void getMapInput(string& inputFile, int& N){
    cout<<"Welcome to CS 106B Random Writer ('N-Grams')."<<endl
        <<"This program makes random text based on a document."<<endl
        <<"Give me an input file and an 'N' value for groups"<<endl
        <<"of words, and I'll create random text for you. "<<endl<<endl;

    //Until user requests an existing file.
    while(1){
        cout<<"Input file name? ";
        getline(cin, inputFile);
        //If fileExists returns true, file was found.
        if(fileExists(inputFile)) break;
        else cout<<"Unable to open that file.  Try again."<<endl;
    }

    //Until user inputs N an integer >=2.
    while(1){
        N = getInteger("Value of N? ");
        if(N < 2){
            cout<<"N must be 2 or greater."<<endl;
        }else break; //Success
    }
}

//Loads in our N-Gram Map using the User input found by getMapInput
void loadGramMap(string inputFile, HashMap<Queue<string>, Vector<string>>& gramMap, int N){
    //Open author text file
    ifstream gramFile;
    gramFile.open(inputFile.c_str());

    //Initialize a token scanner (set to include punctuation in words)
    TokenScanner gramScan(gramFile);
    gramScan.ignoreWhitespace();
    gramScan.addWordCharacters("!\"#$%&'()*+,-./:;<=>?@[\\]^_'{|}");

    //This is the moving window of size N-1 as gramScan is scans the file
    Queue<string> window;
    //Initialize moving window to first N-1 tokens in file
    for(int i = 0; i < N-1; i++){
        window.enqueue(gramScan.nextToken());
    }

    //While there are still tokens in file
    while(gramScan.hasMoreTokens()){
        //Add next token to gramMap[window]'s vector of tokens
        string nextToken = gramScan.nextToken();
        gramMap[window].add(nextToken);

        //Shift window
        window.dequeue();
        window.enqueue(nextToken);
    }

    //Wrap to beginning of file by reopening file and resetting gramScan
    gramFile.close();
    gramFile.open(inputFile.c_str());
    gramScan.setInput(gramFile);

    //For the last N-1 words create more N grams.
    for(int i = 0; i < N - 1; i++){
        string nextToken = gramScan.nextToken();
        gramMap[window].add(nextToken);
        window.dequeue();
        window.enqueue(nextToken);
    }



}

//Get the number of words to print from user.
int getNumWords(int N){
    //Until user inputs a numWords >=N or 0 for exit.
    while(1){
        cout<<endl;
        int numWords = getInteger("# of random words to generate (0 to quit)? ");
        if((numWords < N) && (numWords != 0)){
            cout<<"Must be at least "<<N<<" words."<<endl;
        }else return numWords; //Success
    }
}

//Print out numWords using our N-Gram map.
void printWords(int numWords, HashMap<Queue<string>, Vector<string> > &gramMap){
    //Create a vector of all our N-Gram keys
    Vector<Queue<string> > keys = gramMap.keys();
    //Pick a random key to start window.
    int randKey = randomInteger(0, keys.size() - 1);
    Queue<string> window = keys[randKey];

    //Print the numWords
    cout<<"... ";
    for(int i = 0; i < numWords; i++){
        //Select a random Nth word from our window N-Gram.
        int randWord = randomInteger(0, gramMap[window].size() - 1);
        //Add new word to window.
        window.enqueue(gramMap[window].get(randWord));
        //Dequeue and print first word in window.
        cout<<window.dequeue()<<" ";
    }
    cout<<" ..."<<endl;
}
