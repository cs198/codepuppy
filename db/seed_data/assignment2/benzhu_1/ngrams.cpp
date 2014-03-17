//Name: Benzheng Zhu
//CS 106B Assignment 2 Part B
//Citation: lecture slides
//this program that reads an input file and uses it to build a large data structure of word groups called "Ngrams"
//as a basis for randomly generating new text that sounds like it came from the same author as that file.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "queue.h"
#include "simpio.h"
#include "hashmap.h"
#include "filelib.h"
#include "vector.h"
#include "random.h"

using namespace std;

//function prototypes
void welcome();
void readFile(ifstream& input, Queue<string>& text);
void createMap(int& N, Queue<string>& text, HashMap<Queue<string>,Vector<string>>& map);
void run(int& N, int& len, HashMap<Queue<string>,Vector<string>>& map);
void printText(Queue<string>& output);

//it takes long for this program to read big files such as "hamlet.txt"
//please be patient.

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    welcome();
    ifstream input;
    promptUserForFile(input, "Input file name? ");

    int N=0;
    while (true) {
        N = getInteger("Value of N? ");
        if (N<2) {
            cout << "N must be 2 or greater."<<endl; //check if the integer is at least 2
        } else {
            break;
        }
    }

    Queue<string> text;
    readFile(input,text);

    HashMap<Queue<string>,Vector<string>> map;
    createMap(N,text,map);

    while (true) {
        cout << endl;
        int len = getInteger("# of random words to generate (0 to quit)? ");
        if (len == 0) {
            break;
        } else if (len<N) {
            cout << "Must be at least " << N << " words." << endl; //check if len is at least N
        } else {
            run(N,len,map);
        }
    }

    cout << "Exiting." << endl;
    return 0;
}

void welcome() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')."<<endl
         << "This program makes random text based on a document."<<endl
         << "Give me an input file and an 'N' value for groups " <<endl
         << "of words, and I'll create random text for you."<<endl<<endl;
}


void readFile(ifstream& input, Queue<string>& text) {
    //this function takes the ifstream and pre-defined queue of strings as inputs
    //then it reads the file word by word and store them into the queue

    string word;
    while (true) {
        input >> word;   // read one word
        if (input.fail()) {
            break;
        }
        text.enqueue(word);
    }
    input.close();
}

void createMap(int& N, Queue<string>& text, HashMap<Queue<string>,Vector<string>>& map) {
    //this fucntion takes an integer N, the queue of words, and a pred-defined map as inputs
    //it first reads the first N-1 words from the queue and stores them in a queue called window.
    //next it reads the first word of the rest of the queue text, and stores it as the value of the window
    //the same process is repeated for every word in the queue text until it is empty.

    Queue<string> window;

    for (int i=0; i<N-1; i++) {
        string temp = text.dequeue();
        window.enqueue(temp);
        text.enqueue(temp); //move the word in the front to the back of the queue, so that the map wraps around.
    }

    while (!text.isEmpty()) {
        map[window] += text.peek();
        window.dequeue();
        window.enqueue(text.dequeue());
    }
}

void run(int& N, int& len, HashMap<Queue<string>,Vector<string>>& map) {
    //this function takes integer N, integer len, and the map as inputs
    //it first randomly chooses a key in the map as the starting point,
    //next it randomly chooses one of the words in its value vector as the next word
    //and then it shifts to the most recent N-1 words, using it as the new window and repeating the process,
    //until len words are generated.

    Vector<Queue<string>> key = map.keys();
    int rand1 = randomInteger(0, key.size()-1);
    Queue<string> prefix = key[rand1]; //randomly chooses the starting point
    Queue<string> output = prefix; //stores the starting N-1 words

    for (int i=N-1; i<len; i++) {
        Vector<string> value = map[prefix]; //value contains all the candidate suffixes
        int rand2 = randomInteger(0,value.size()-1);
        string suffix = value[rand2]; //randomly chooses one as the next word
        output.enqueue(suffix);
        prefix.dequeue();
        prefix.enqueue(suffix);
    }
    printText(output);
}

void printText(Queue<string>& output) {
    //this function takes a queue of words as input
    //and prints the words out, with "..." at two ends

    cout << "... ";
    while(!output.isEmpty()) {
        cout << output.dequeue()<< " ";
    }
    cout << "..." << endl;
}
