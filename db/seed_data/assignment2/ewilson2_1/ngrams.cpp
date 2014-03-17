// This program will accept a file and generate random text based upon the language patterns of the author.
//The user will generate the fil, tell the program the length of the word pattern upon which they would like the text to be based, and the number of words they want generated.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include <cctype>
#include "filelib.h"
#include "simpio.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "tokenscanner.h"
#include "hashmap.h"
#include "random.h"

using namespace std;

void setup();
HashMap< Vector<string>, Vector<string> > userInput(Vector<string>& allText, int&n);
HashMap< Vector<string>, Vector<string> > makeMap(int n, Vector<string>& allText);
int generateText(HashMap< Vector<string>, Vector<string> >& map, Vector<string>& allText, int n);

int main() {
    setup();
    Vector<string> allText;
    int n;
    HashMap< Vector<string>, Vector<string> > map=userInput(allText, n);
    generateText(map, allText, n);
    cout << "Exiting." << endl;
    return 0;
}

//This method sets up the console and displays the opening text
void setup() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

//This method receives the user input for filename and value of N, to pass in as parameters to generate the appropriate word map
HashMap< Vector<string>, Vector<string> >  userInput(Vector<string>& allText, int& n) {
    string filename;
    ifstream infile;
    filename=getLine("Input file name? ");
    while(true) {
        openFile(infile, filename);
        if(infile.fail()) {
            cout << "Unable to open that file. Try again." << endl;
            filename=getLine("Input file name? ");
        } else {
            break;
        }
    }
    while(true) {
        n=getInteger("Value of N? ");
        if(n<2) {
            cout << "N must be 2 or greater." << endl;
        } else {
            break;
        }
    }
    string word;
    while(infile >> word) {
        allText.add(word);
    }
    for(int i=0; i<n-1; i++) {
        allText.add(allText[i]);
    }
    HashMap< Vector<string>, Vector<string> > map=makeMap(n, allText);
    return map;
}

//This method takes in the n value and the vector containing the total text as parameters, and generates the appropriate map
HashMap< Vector<string>, Vector<string> > makeMap(int n, Vector<string>& allText) {
    HashMap< Vector<string>, Vector<string> > map;
    for(int i=0; i<(allText.size()-1)-(n-2); i++) {
        Vector<string> prefix;
        Vector<string> suffix;
        for(int j=0; j<n-1; j++) {
            prefix.add(allText[i+j]);
        }
        if(map.containsKey(prefix)==false) {
            suffix.add(allText[i+n-1]);
            map.put(prefix, suffix);
        } else {
            suffix=map.get(prefix);
            suffix.add(allText[i+n-1]);
            map.put(prefix, suffix);
        }
    }
    return map;
}

//This method accepts the user's desired number of words to be generated, and prints out the corresponding random text
int generateText(HashMap< Vector<string>, Vector<string> >& map, Vector<string>& allText, int n) {
    while(true) {
        int numWords;
        while(true) {
            cout << "" << endl;
            numWords=getInteger("# of random words to generate (0 to quit)? ");
            if(numWords==0) {
                return 0;
            } if(numWords>n==false) {
                cout << "Must be at least " << n << " words." << endl;
            } else {
                break;
            }
        }
        int x=randomInteger(0, (allText.size()-1)-(n-1));
        Vector<string> prefix;
        string outputString="... ";
        for(int i=0; i<n-1; i++) {
            prefix.add(allText[x+i]);
            outputString=outputString+allText[x+i]+" ";
        }
        for(int i=0; i<numWords-(n-1); i++) {
            Vector<string> suffix=map.get(prefix);
            int y=randomInteger(0, suffix.size()-1);
            string nextWord=suffix[y];
            outputString=outputString+nextWord+" ";
            prefix.remove(0);
            prefix.add(nextWord);
        }
        outputString+=" ...";
        cout << outputString << endl;
        cout << "" << endl;
    }
}
