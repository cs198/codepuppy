/* By: Alyssa Vann
 * This is the NGrams Assignment 2 for CS106B. It reads in a text
 * file according the user's preference, and it allows the user
 * to generate random text (varying with the number of ngrams and
 * the amount of text they select).
 * For this assignment, I used the book, lecture notes, received help
 * at the Lair.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
#include "hashmap.h"
#include "queue.h"

using namespace std;

void greeting();
void readFile(Vector<string>& words);
void createMap(Vector<string>& words, HashMap<Vector<string>, Vector<string> >& ngrams);
void randomGenerator(HashMap<Vector<string>, Vector<string> >& ngrams);
void createFirstKey(HashMap<Vector<string>, Vector<string> >& ngrams, Vector<string>& firstkey,
                    Queue<string>& keyvalue, Vector<string>& output);
void createFirstValue(HashMap<Vector<string>, Vector<string> >& ngrams, Vector<string>& firstkey,
                      Queue<string>& keyvalue, Vector<string>& output);
void shiftKeyValue(HashMap<Vector<string>, Vector<string> >& ngrams,
                   Queue<string>& keyvalue, Vector<string>& output);


int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    greeting();

    Vector<string> words;

    readFile(words);

    HashMap<Vector<string>, Vector<string> > ngrams;

    createMap(words, ngrams);

    randomGenerator(ngrams);

    cout << "Exiting." << endl;
    return 0;
}

/* The greeting function outputs a greeting to the user
 * at the start of the program.
 */
void greeting(){
    cout << "Welcome to CS106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

/* The readFile function accepts user input for which file to read.
 * It reads all the words in the file, and puts them into a vector
 * of words to be used in other functions to create the ngram HashMap.
 */
void readFile(Vector<string>& words){
    ifstream input;
    promptUserForFile(input, "Input file name? ");
    string word;
    while(input >> word){
        words.add(word);
    }
    input.close();
}

/* The createMap function takes the vector of words created by
 * readFile, and produces a HashMap of keys and values, depending
 * on the number of ngrams the user inputs.
 */
void createMap(Vector<string>& words, HashMap<Vector<string>, Vector<string> >& ngrams){

    int n = getInteger("Number of N-Grams: ");

    for(int i=0; i<words.size(); i++){
        Vector<string> key;

        for(int j=i; j<(i+(n-1)); j++){
            int actualindex = j%(words.size());
            key += words[actualindex];
        }

        int valueindex = (i+(n-1))%words.size();
        string value = words[valueindex];

        if(ngrams.containsKey(key)){
            Vector<string> update = ngrams.get(key);
            update.add(value);
            ngrams.put(key, update);
        }else{
            Vector<string> newvec;
            newvec.add(value);
            ngrams.put(key, newvec);
        }
     }
}

/* The randomGenerator uses the HashMap of ngrams to generate
 * random text. It puts all the keys and values into a queue,
 * which is looped over and updated according to the number
 * of times the user inputs.
 */
void randomGenerator(HashMap<Vector<string>, Vector<string> >& ngrams){
    Vector<string> output;
    Vector<string> firstkey;
    Queue<string> keyvalue;

    createFirstKey(ngrams, firstkey,
                        keyvalue, output);

    int n = firstkey.size();

    createFirstValue(ngrams, firstkey,
                        keyvalue, output);

    int usernum;

    while(true){
        usernum = getInteger("# of random words to generate (0 to quit)? ");
        if(usernum == 0) break;

        for(int i=0; i< (usernum-n); i++){
            shiftKeyValue(ngrams, keyvalue, output);
        }

        for(string s: output){
            cout << s << " ";
        }
        cout << "..." << endl << endl;
    }
}

/* The createFirstKey function is called within randomGenerator to
 * obtain random starting text for the randomGenerator queue of
 * keys and values.
 */
void createFirstKey(HashMap<Vector<string>, Vector<string> >& ngrams, Vector<string>& firstkey,
                    Queue<string>& keyvalue, Vector<string>& output){
    Vector<Vector<string> > keys;
    keys = ngrams.keys();

    int keyssize = keys.size();
    int randomkey = randomInteger(0, keyssize-1);

    firstkey = keys.get(randomkey);

    output.add("...");

    for(string s: firstkey){
        output.add(s);
        keyvalue.enqueue(s);
    }
}

/* The createFirstValue function is called within randomGenerator
 * to find a random value corresponding the the random first key
 * used to generate the starting random text.
 */
void createFirstValue(HashMap<Vector<string>, Vector<string> >& ngrams, Vector<string>& firstkey,
                      Queue<string>& keyvalue, Vector<string>& output){
    Vector<string> firstvalues;
    firstvalues = ngrams.get(firstkey);
    int firstvaluessize = firstvalues.size();
    int randomfirstvalue = randomInteger(0, firstvaluessize-1);
    string value = firstvalues[randomfirstvalue];
    output.add(value);
    keyvalue.enqueue(value);
}

/* The shiftKeyValue is called within randomGenerator in a loop whose
 * size is determined by the user. It updates the queue of random keys and
 * values, and it updates the output accordingly.
 */
void shiftKeyValue(HashMap<Vector<string>, Vector<string> >& ngrams,
                   Queue<string>& keyvalue, Vector<string>& output){
    keyvalue.dequeue();

    Queue<string> tempcopy = keyvalue;
    Vector<string> newkey;
    while(!tempcopy.isEmpty()){
        string temp = tempcopy.dequeue();
        newkey.add(temp);
    }

    Vector<string> values = ngrams.get(newkey);
    int valuessize = values.size();
    int randomvalue = randomInteger(0, valuessize-1);
    string value = values[randomvalue];
    keyvalue.enqueue(value);

    output.add(value);
}
