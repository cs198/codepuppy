// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "hashmap.h"
#include "stdlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"
#include "random.h"

using namespace std;

void printWelcomeMessage();
int askForInteger();
string getFileName();
void getWords(string , Vector <string>&);
void putWords(int , Vector<string>& , HashMap<Vector<string> , Vector<string> >&);
void printNgrams(int , int , HashMap<Vector<string> , Vector<string> >&);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    HashMap<Vector<string> , Vector<string> > nmap; //initializes the hashmap of ngrams

    printWelcomeMessage(); //prints the wecome message

    //opens the file
    ifstream file;
    string filename = getFileName();
    string line;
    openFile(file , filename);
    Vector<string> allWords; //initializes a vector to store all the words
    while(getline(file , line)){
        //gets all the words from the file and puts them into a vector of strings
        getWords(line , allWords);
    }

    int n = askForInteger(); //gets the value of n

    putWords(n , allWords , nmap); //puts all the words form the vector into the hasmap (with the key value pairings)

    while (true){
        //starts the main part of the program - printing out the ngrams
        //first asks for the number of words while checking for validity of input
        int numWords = getInteger("# of random words to generate (0 to quit)? ");
        if (numWords == 0){
            break;
        }
        while (numWords < 4){
            cout << "Must be at least 4 words." << endl << endl;
            numWords = getInteger("# of random words to generate (0 to quit)? ");
        }
        cout << "... ";
        printNgrams(n , numWords , nmap); //prints the string of ngrams
        cout << "..." << endl << endl;
    }

    cout << "Exiting." << endl;
    return 0;
}

void printWelcomeMessage(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you." << endl << endl;
}

string getFileName(){
    //asks for a filename and checks for correct input
    string filename;
    while(true){
        filename = getLine("Input file name? ");
        if (fileExists(filename)){
            //if it exists
            break;
        } else{
            //if the file doesn't exist
            cout << "Unable to open that file. Try again." << endl;
        }
    }
    return filename;
}

void getWords(string line , Vector <string>& allWords){
    //gets the words from a file and stores them in a vector
    //goes through each line and when it hits a space it recognizes it and stores the word before that into a vector of strings
    int startOfWord = 0; //where the current word is starting
    int size = line.size(); //total size of the line we're reading
    for (int i = 0 ; i < size ; i++){ //iterates over that line
        if (line[i] == ' ' || i == (size-1)){ //if we're at a space or the end of the line
            string word; //initializes a string
            if (i == (size-1)){ //if we're at the end of the line
                word = line.substr(startOfWord , (i-startOfWord+1));
            } else{ //if we've reached a space
                word = line.substr(startOfWord , (i-startOfWord));
            }
            if (word != "" && word != " "){ //if it's not an empty string or just a space
                allWords += word; //adds the word
            }
            startOfWord = (i+1); //moves the current beginning of the word to the next word
        }
    }
}

int askForInteger(){
    //asks for the integer N
    int n = getInteger("Value of N? ");
    while (n < 2){
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N? ");
    }
    cout << endl;
    return n;
}

void putWords(int n , Vector<string>& allWords , HashMap<Vector<string> , Vector<string> >& nmap){
    //puts the words into the hashmap
    for (int i = 0 ; i < allWords.size() ; i++){
        //iterates over all the words in the vector of strings
        Vector<string> key;
        Vector<string> value;
        if (i+n < allWords.size()){
            //for the basic cases (when the indices are in bounds)
            for (int j = i ; j < (i+n-1) ; j++){
                key += allWords[j]; //adds n-1 words to the key vector
            }
            value += allWords[i+n-1]; //adds the nth word to the value vector
        } else{
            //for the loop around cases (excuse the very long and unelegant code)
            for (int j = i ; j < (i+n-1) ; j++){
                if (j >= allWords.size()){ //if the index is out of bounds
                    key += allWords[j-allWords.size()]; //goes back to the beginning of the text
                } else{ //otherwise just adds them like normally
                    key += allWords[j] + " ";
                }
            }
            if (i+n-1 >= allWords.size()){ //if the index of the value is out of bounds
                value += allWords[i+n-1-allWords.size()]; //loops back around
            } else{ //just adds it normally
                value += allWords[i+n-1];
            }
        }
        if (!nmap.containsKey(key)){ //if the key doesn't already exist
            nmap.put(key , value); //adds the pair
        } else{
            nmap[key] += value; //adds the value to the values vector for the key
        }
    }
}

void printNgrams(int n , int numWords , HashMap<Vector<string> , Vector<string> >& nmap){
    //prints out the text of ngrams
    Vector<Vector<string> > keys = nmap.keys(); //starts a vector with all the keys from the map
    int rand = randomInteger(0 , keys.size()-1);
    Vector<string> current = keys[rand]; //gets a random key from that vector
    for (string s : current){ //prints out all the words in that key
        cout << s << " ";
    }
    for (int i = 0 ; i < numWords - n + 1 ; i++){ //iterates for the amount of words the user wants
        //the next 3 lines get a random value from the values vector corresponding to the key we just printed
        int valueSize = nmap.get(current).size();
        int random = randomInteger(0 , valueSize-1);
        string nextWord = nmap.get(current)[random];
        cout << nextWord << " "; //prints out that value
        //the next 2 lines shift the "window" over
        current.remove(0);
        current.add(nextWord);
    }
}

