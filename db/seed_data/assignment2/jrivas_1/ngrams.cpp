// This program will receive text from a file and
// reads the file, while creating a large data structure
// of group words called "N-grams" as a basis for randomly
// generating new text that sounds like it came from the same
// author as that file.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "gwindow.h"
#include "simpio.h"
#include "map.h"
#include "set.h"
#include "queue.h"
#include "vector.h"
#include "strlib.h"
#include "hashmap.h"
#include "random.h"

using namespace std;

//declaring functions
void GiveIntroduction();
void ReceiveFile(ifstream & infile, int & n);
void GenerateText(int words, HashMap<Queue<string>, string> & map);
void CreateMap(ifstream & infile, int & n, HashMap<Queue<string>, string> & map);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    ifstream infile;
    int n;
    HashMap<Queue<string>, string> map;
    GiveIntroduction();
    ReceiveFile(infile, n);
    CreateMap(infile, n, map);
    string prompt = "# of random words to generate (0 to quit)? ";
    int words = getInteger(prompt);
    while (words != 0) {
        while (words < (n - 1)) {
            cout << "Must be at least " << n - 1 << " words." << endl;
            cout << endl;
            words = getInteger(prompt);
        }
        GenerateText(words, map);
        words = getInteger(prompt);
    }
    cout << "Exiting." << endl;
    return 0;
}

void GiveIntroduction() {
    cout << "Welcome to CS 106B Random Writer ('N Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

void ReceiveFile(ifstream & infile, int & n) {
    string prompt1 = "Input file? ";
    while (true) {
        string filename = getLine(prompt1);
        infile.open(filename.c_str());
        if (!infile.fail()) break;
        cout << "Unable to open that file. Try again." << endl;
        infile.clear();
    } string prompt2 = "Value of N? ";
    n = getInteger(prompt2);
    while (true) {
        if (n > 2) break;
        cout << "N must be 2 or greater." << endl;
        n = getInteger(prompt2);
    } cout << endl;
}

void CreateMap(ifstream & infile, int & n, HashMap<Queue<string>, string> & map) {
    Queue<string> key;
    string word;
    string value;
    Queue<string> finalwords;
    infile >> word;
    key.enqueue(word);
    finalwords.enqueue(word);
    for (int i = 0; i < n - 2; i++) {
        infile >> word;
        if (infile.fail()) break;
        key.enqueue(word);
        finalwords.enqueue(word);
    } while (true) {
        infile >> value;
        if (infile.fail()) break;
        map[key] = value;
        key.enqueue(value);
        key.dequeue();
    } for (int j = 0; j < n - 1; j++) {
        value = finalwords.dequeue();
        map[key] = value;
        key.enqueue(value);
        string kickedout = key.dequeue();
    }
}

void GenerateText(int words, HashMap<Queue<string>, string> & map) {
    cout << "... ";
    string tempword;
    Vector<Queue<string>> keys1 = map.keys();
    int random = randomInteger(0,keys1.size());
    Queue<string> queue = keys1.get(random);
    int size = queue.size();
    for (int i = 0; i < size; i++) {
        tempword = queue.dequeue();
        cout << tempword << " ";
        queue.enqueue(tempword);
    } string value = map.get(queue);
    cout << value << " ";
    for (int j = 0; j < words - size; j++) {
        queue.enqueue(value);
        queue.dequeue();
        value = map.get(queue);
        cout << value << " ";
    } cout << "..." << endl;
}
