// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "Hashmap.h"
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "random.h"

using namespace std;

//Display the introduction words
void intro(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you" << endl;
    cout << endl;
}

// Get the input file name
// if the file doesn't exist, it will return error
string fileName() {
    string fileName = getLine("Input file name? ");
    while (fileExists(fileName) == false) {
        cout << "Unable to open that file. Try again " << endl;
        fileName = getLine ("Input file name?");
    }
     return fileName;
}

// store the words of the file in the map (from {prefix} --> {suffixes})
// Use hashmap, the keys are queue of string, values are vectors of string.
void storeWord(string fileName, int n, HashMap<Queue<string>, Vector<string> >& map){
    ifstream input;
    openFile(input, fileName);
    string next;
    Queue<string> window;
    Queue<string> firstWord; // the first N-1 words, creat it for store the first words in queue
    for (int i=0; i< n-1; i++){ // initialize the first window
          input >> next;
          window.enqueue(next);
          firstWord.enqueue(next);
        }
     while(input >> next){ // creat windows and keys, and the corresponding value
      map[window] += next;
      window.dequeue();
      window.enqueue(next); // update window. the last window includes the last word of the file.
    }
     // wrap around
     for (int i = 0; i< n-1; i++){ // warp around for the first N-1 words
         next  = firstWord.dequeue(); //get word from the first N-1 store.
         map[window] += next;  // for the first loop, set the first word to be the value of the last window from last loop
         window.dequeue();
         window.enqueue(next); //update window
     }
}

void textOutput(int m, int n, HashMap<Queue<string>, Vector<string> > map,Queue<string>& text){
    Vector<Queue<string> > keys;
    keys = map.keys(); // get all the keys in a vector
    int max = keys.size();
     max = max - 1;
     int min = 0;
     int rdm = randomInteger(min, max); // make a random integer
     Queue<string> firstWords = keys[rdm]; // get a random words to start from the windows.
     for (int i = 0; i < n-1; i++){
        string word = firstWords.dequeue();
        text.enqueue(word); // put the firs N-1 words in the text
     }
     // m words text, begin with n-1 words, still needs m-n+1 words to finish
     Queue<string> key = keys[rdm];
     for (int i = 0; i < m - n + 1; i++){
         Vector<string> value = map.get(key);
         int max = value.size();
         max = max-1;
         int min = 0;
         int rdm = randomInteger(min,max); // find a random word from the value vector to continue
         string word = value[rdm];
         text.enqueue(word);
         key.dequeue();
         key.enqueue(word); //update the key
     }
}

// print the text
void printText(Queue<string>& text){
   cout << "... ";
   while(!text.isEmpty()){
       cout << text.dequeue() << " ";
   }
   cout << "..." << endl;
 }


int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    // TODO: Finish the program!
    intro();
    string fname  = fileName();
    int n;
    // ********** get the N from the console. It will report error if N is less than 2
    while(true){
        n = getInteger("Value of N? ");
        if(n <= 1){
            cout << "N must be 2 or greater." << endl;
        } else{
            break;
        }
    }
    // ********** store the words in a map
    HashMap<Queue<string>, Vector<string> > map;
    storeWord(fname, n, map);
    // ********** display the text of # words
    Queue<string> text;
    while(true){
        cout << endl;
        int num = getInteger("# of words to generate (0 to quit)? ");
        if (num != 0){
            if(num < n){ // # must be at least n, if not, report error
                cout << "must be at least " << n << " words" << endl;
            } else if (num >= n){ // # correct, do display
                textOutput(num, n, map,text);
                printText(text);
            }
        } else if (num == 0){ // 0 to quit
            break;
        }
    }
    cout << "Exiting." << endl;
    return 0;
}
