/*
 * Michelle S. Lee
 * CS 106B
 * Assignment 2 (NGRAM)
 *
 * This program takes an input of file name, N, and number of words and outputs
 * randomly generated text that sounds like the author.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "lexicon.h"
#include "vector.h"
#include "queue.h"
#include "stack.h"
#include "set.h"
#include "vector.h"
#include "random.h"
#include "simpio.h"

using namespace std;

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    //Read in the file
    string filename;
    ifstream infile;
    Vector<string> text;
    cout << "Input file name? ";
    getline(cin, filename);
    while (!fileExists(filename)) {
        cout << "File not found. Enter file name: " << endl;
        getline(cin, filename);
    }
    infile.open(filename.c_str());

    //Add all words from the text file into a vector of strings
    string line;
    while (getline(infile, line)) {
         while (line.find(" ") != string::npos) {
              string word = line.substr(0, line.find(" "));
              line = line.substr(line.find(" ")+1);
                 if (word != "") {
                    text.add(word);
                 }
        }
    }
    infile.close();

    //Get the value of N from the user; prompt user for an integer if it is not a valid input.
    int N = getInteger ("Value of N? ");

    while (N < 0) {
        N = getInteger ("Value of N? ");
        cout << endl;
        cout << "Enter a positive integer" << endl;
    }

    while (N > text.size()) {
        cout << "Enter a length smaller than the text size" << endl;
        N = getInteger ("Value of N? ");
    }

    int numLoops = -1;

    while (true) {

        //Get the number of random words to generate from the user.
        numLoops = getInteger("Number of random words to generate (0 to quit): ");

        while (numLoops < 0) {
            cout << "Enter a positive integer" << endl;
            numLoops = getInteger("Number of random words to generate (0 to quit): ");
        }

        if (numLoops == 0) {
            cout << "Exiting" << endl;
            return 0;
        }

        //Create a map with the key as N number of text strings and the value as the following words
        Map<string, Vector<string>> myMap;
        int numWords = text.size();
        Queue<string> window;
        for (int i = 0; i < N - 1; i++) {
            window.enqueue(text[i]);
        }
        for (int i = 0; i < numWords; i++) {
            int index = (i+N-2)%numWords;
            if (i != 0) {
                window.dequeue();
                window.enqueue(text[index]);
            }
            Queue<string> copy = window;
            string key;
            while (!copy.isEmpty()) {
                key += copy.dequeue();
                if (!copy.isEmpty()) {
                    key += " ";
              }
           }
           if (!myMap.containsKey(key)) {
               myMap.put(key, Vector<string>());
            }
            Vector<string> value = myMap.get(key);
            value.add(text[(index+1)%numWords]);
            myMap.put(key, value);
        }

        //Print the randomly generated text
        Vector<string> keys = myMap.keys();
        int number = randomInteger(0, keys.size()-1);
        string key = keys[number];
        cout << " ... " << key;
        for (int i = N-1; i < numLoops; i++) {
            Vector<string> values = myMap.get(key);
            int num2 = randomInteger(0, values.size()-1);
            string next = values.get(num2);
            cout << " " << next;

            int pos = key.find(" ");
            key = key.substr(pos+1);
            key.append(" ");
            key.append(next);
        }
        cout << " ... " << endl;
    }
}
