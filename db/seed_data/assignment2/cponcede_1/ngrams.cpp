/* Program: N-Grams
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * ------------------------------
 * This program reads an input file and uses it to create a map of word groups
 * that serves as a basis for randomly generating text that sounds like it came from
 * the same author as that file.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "hashMap.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"

using namespace std;

//Function prototypes
void displayBeginningMessages();
int determineHashMap (HashMap < Vector<string>, Vector<string> > &map);
void addWordsToVector(Vector<string> &words, string& line);
void createHashMapFromVector(Vector<string> &words, HashMap < Vector<string>, Vector<string> > &map, int n);
void generateText(HashMap < Vector<string>, Vector<string> > &map, int n);
void determineNextWord(Vector <string> &focusPoint , HashMap < Vector<string>, Vector<string> > &map);
void removeTabs(string &word);

//Runs the N-Grams program
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    HashMap < Vector<string>, Vector<string> > map;
    int n = determineHashMap(map);
    generateText(map,n);
    cout << "Exiting." << endl;
    return 0;
}

//Displays the initial messages at the top of the console at the start of the program.
void displayBeginningMessages() {
    cout << "Wecome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

/* Reads a text file that the user inputs and adds all of the words that are separated by
 * whitespace in that file to a Vector of strings. The function will not allow the user
 * to enter anything that is not a valid file. After putting all of the words into a Vector,
 * the function asks the user for an N value that is greater than one but less than the number
 * of words in the file. Using this value for N, the function creates a HashMap that uses a
 * Vector of N-1 consecutive words as the key and maps to a Vector that contains all of the
 * different words that follow those N-1 words in the text file.
 */
int determineHashMap (HashMap < Vector<string> ,Vector<string> > &map) {
    Vector<string> words;
    while (true) {
        try {
            ifstream input;
            string file = getLine ("Input file name? ");
            input.open(file.c_str());
            string line;
            while(getline(input,line)) {
                if (line.length() != 0) {
                    addWordsToVector(words,line);
                }
            }
            break;
        } catch (exception e) {
            cerr << e.what() << endl;
            cout << "That is not a valid file. Try again." << endl;
        }
    }
    int n;
    while (true) {
        n = getInteger("Value of N? ");
        if (n < words.size() && n > 1) {
            break;
        } else {
            cout << "Please choose a value of N that is larder than 1 and smaller than the number of words in the file." << endl;
        }
    }
    cout << endl;
    createHashMapFromVector(words,map,n);
    return n;
}

//Takes in a line and splits the line into the different words that are separated by spaces. It adds
//these words to a Vector that contains all of the words in the file.
void addWordsToVector(Vector<string> &words, string& line) {
    int index = 0;
    while(line.find(" ",index) != string::npos) {
        int endIndex = line.find(" ",index);
        int length = endIndex - index;
        string word = line.substr(index,length);
        removeTabs(word);
        if (word != "") {
            words.add(word);
        }
        index = endIndex + 1;
    }
    string word = line.substr(index);
    removeTabs(word);
    if (word != "") {
        words.add(word);
    }
}

/* Due to an error where tabs would be read as '\t' characters, this method searches through
 * each word before it is added to the Vector of words and removes the \t characters. It also changes
 * quotes, represented as '\042' to quotes.
 */
void removeTabs(string &word) {
    Vector <int> tabLocations;
    int tabCounter = 0;
    for (int i = 0 ; i < word.length() ; i++) {
        if (word[i] == '\t') {
            tabLocations.add(i);
            tabCounter++;
        }
    }
    for (int j = 0 ; j < tabCounter ; j++) {
        int tabIndex = tabLocations.get(j);
        word.erase(tabIndex - j);
    }
}



/* This method goes through the Vector that contains all of the words in the text file in consecutive order
 * and creates a HashMap from the Vector that uses a
 * Vector of N-1 consecutive words as the key that maps to a Vector that contains all of the
 * different words that follow those N-1 words in the text file.
 */
void createHashMapFromVector(Vector<string> &words, HashMap < Vector<string>, Vector<string> > &map, int n) {
    for (int i = 0 ; i < words.size() ; i++) {
        Vector<string> keyVector;
        Vector<string> resultVector;
        for (int j = 0 ; j < n-1 ; j++) {
            int index = (i+j)%words.size();
            keyVector.add(words.get(index));
        }
        int resultIndex = (i+(n-1))%words.size();
        resultVector.add(words.get(resultIndex));
        if (map.containsKey(keyVector)) {
            Vector<string> newResultVector = map.get(keyVector);
            newResultVector.add(resultVector.get(0));
            map.remove(keyVector);
            map.put(keyVector,newResultVector);
        } else {
            map.put(keyVector,resultVector);
        }
    }
}

/* Prints out randomly generated text to the console using the HashMap that was created earlier as
 * a basis for determining which new word to use. The function continues to print out words
 * until the user enters 0 for the number of words to generate, causing the program to exit.
 */
void generateText(HashMap < Vector<string>, Vector<string> > &map, int n) {
    Vector< Vector<string> > keys = map.keys();
    int startingIndex = randomInteger(0,keys.size() - 1);
    Vector <string> focusPoint = keys.get(startingIndex);
    while (true) {
        int numberOfWords;
        while (true) {
            numberOfWords = getInteger("# of random words to generate (0 to quit)? ");
            if (numberOfWords >= n) {
                break;
            } else if (numberOfWords == 0) {
                return;
            } else {
                cout << "Must be at least " << n << " words.";
            }
        }
        cout << "... ";
        for (int i = 0 ; i < numberOfWords ; i++) {
            cout << focusPoint.get(0) << " ";
            determineNextWord(focusPoint,map);
        }
        cout << " ... " << endl << endl;
    }


}


/* Looks at the HashMap to get a Vector of all the possible words that could be used next
 * in the randomly generated text and then chooses one of those words at random to be used.
 */
void determineNextWord(Vector <string> &focusPoint, HashMap < Vector<string>, Vector<string> > &map) {
    Vector<string> possibleResponses = map.get(focusPoint);
    int responseChoice = randomInteger(0,possibleResponses.size()-1);
    string result = possibleResponses.get(responseChoice);
    focusPoint.remove(0);
    focusPoint.add(result);
}
