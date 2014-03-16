// This program will receive text from a file and
// reads the file, while creating a large data structure
// of group words called "N-grams" as a basis for randomly
// generating new text that sounds like it came from the same
// author as that file. This is the commented version, and one
// where I attempted to fix the bug in the previous version
// where I think it only chooses the last key. It doesn't
// work as well as the previous version.

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
void GenerateText(int words, HashMap<Queue<string>, Queue<string>> & map);
void CreateMap(ifstream & infile, int & n, HashMap<Queue<string>, Queue<string>> & map);
string getRandomString(Queue<string> queue);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    ifstream infile;
    int n;
    HashMap<Queue<string>, Queue<string>> map;
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

/*
 * Introduces the program and gives the instructions.
 */
void GiveIntroduction() {
    cout << "Welcome to CS 106B Random Writer ('N Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

/*
 * Opens the file and receives the value of N.
 */
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

/*
 * Creates a map of two queues, one that N-1 strings, and the other one that
 * collects all of the possible values for the key.
 */
void CreateMap(ifstream & infile, int & n, HashMap<Queue<string>, Queue<string>> & map) {
    Queue<string> key;
    Queue<string> value;
    string word;
    string value1;
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
        infile >> value1;
        if (infile.fail()) break;
        if (map.containsKey(key)) {
            value = map.get(key);
        }
        else value.clear();
        value.enqueue(value1);
        map[key] = value;
        key.enqueue(value1);
        key.dequeue();
        value.clear();
    }
    for (int j = 0; j < n - 1; j++) {
        value1 = finalwords.dequeue();
        if (map.containsKey(key)) {
            value = map.get(key);
        } else value.clear();
        value.enqueue(value1);
        map[key] = value;
        key.enqueue(value1);
        value.clear();
        string kickedout = key.dequeue();
    }
}

/*
 * Generates the text for the output. It first chooses a random key
 * and then outputs the key, and then chooses a random value in the
 * string of values. It repeats this until it has done all the number of
 * words.
 */
void GenerateText(int words, HashMap<Queue<string>, Queue<string>> & map) {
    cout << "... ";
    Queue<string> valuequeue;
    string tempword;
    Vector<Queue<string>> keys1 = map.keys();
    int random = randomInteger(0,keys1.size());
    Queue<string> queue = keys1.get(random);
    int size = queue.size();
    for (int i = 0; i < size; i++) {
        tempword = queue.dequeue();
        cout << tempword << " ";
        queue.enqueue(tempword);
    } valuequeue = map.get(queue);
    string value = getRandomString(valuequeue);
    cout << value << " ";
    for (int j = 0; j < words - size; j++) {
        queue.enqueue(value);
        queue.dequeue();
        valuequeue = map.get(queue);
        value = getRandomString(valuequeue);
        cout << value << " ";
    } cout << "..." << endl;
}

/*
 * This chooses a random string in the queue
 * of possible string values.
 */

string getRandomString(Queue<string> queue) {
    string temp;
    int size = queue.size();
    int random = randomInteger(0, size - 1);
    for (int i = 0; i < random; i++) {
        temp = queue.dequeue();
        queue.enqueue(temp);
    }
    return temp;
}
