/* Filename: Ngrams by Matthew Herrero
 * ---------------------------------------------------------------------------
 * This program reads in a .txt file and uses it to generate a set of words
 * called an Ngram. This set of words analyzes the occurances of each set of
 * "N" words in the .txt file, and records the words that follow them. With this
 * information, it can randomly generate a new set of text by checking the last set
 * of N-1 words in the text being generated in order to determine what the Nth word
 * should be. By doing this repeatedly, the program can generate the number of
 * words that the user asks for. The program asks the user for a .txt file to
 * read in, recieves a value for N, then repeated generates new sets of text based
 * on the amount of text the user asks for until the user inputs 0, and the program
 * quits.
 *
 * CITATION:
 * in line 199, the .eof method was found here: //http://www.cplusplus.com/reference/ios/basic_ios/eof/
 * in lines 311-314, the stringstream object and tokenization algorithm
 *       was found here: //http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "filelib.h"
#include "strlib.h"
#include "console.h"
#include "map.h"
#include "vector.h"
#include "random.h"

using namespace std;

//These are the prototypes of the methods
void printIntro();
string getFilename();
int getNValue();
bool isInteger(string str);
void createMap(Map<string, Vector<string> >& prefSufMap, Vector<string>& window,
               string filename, int nVal, Vector<string>& wrapAround);
void fillMap(Vector<string>& window, Map<string, Vector<string> >& prefSufMap,
             ifstream& input, Vector<string>& wrapAround, int nVal);
string getStringFromVector(Vector<string>, int startInd, int endInd);
void finishWrapAround(Vector<string>& window, Vector<string>& wrapAround,
                      Map<string, Vector<string> >& prefSufMap, int nVal);
bool recieveValidWordCount(int& wordCount, int nVal);
void generateRandomText(Map<string, Vector<string> >& prefSufMap, int wordCount, int nVal);
void initWindowAndString(Map<string, Vector<string> >& prefSufMap, Vector<string>& printWindow,
                           string& finalString);
string getNextWord(Map<string, Vector<string> >& prefSufMap, Vector<string> printWindow);

/*
 * This is the main method for the program. It has the functionality described in this
 * program's header.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    Map<string, Vector<string> > prefSufMap;
    Vector<string> window;
    string filename;
    int nVal;
    int wordCount;

    printIntro();
    filename = getFilename();
    nVal = getNValue();

    Vector<string> wrapAround((nVal-1)*2);

    createMap(prefSufMap, window, filename, nVal, wrapAround);
    while(true) {
        cout << endl;
        if(!recieveValidWordCount(wordCount, nVal)) break;
        generateRandomText(prefSufMap, wordCount, nVal);
    }
    cout << "Exiting." << endl;
    return 0;
}

/*
 * This method welcomes the user to the program and explains what the program does
 */
void printIntro() {
    cout << "Welcome to the CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

/*
 * This method prompts the user to input the name of a file that it can read in and
 * analyze and displays an error message if the user inputs an invalid filename.
 * Once the user has put in a valid filename, the method returns the string.
 */
string getFilename() {
    string filename;
    while(true) {
        cout << "Input file name? ";
        getline(cin, filename);
        if(fileExists(filename)) {
            break;
        } else {
            cout << "Unable to open that file. Try again." << endl;
        }
    }
    return filename;
}

/*
 * This method prompts the user to input an integer value for N. It reads in the
 * user input as a string, then checks if the string is a valid value of N. If it
 * is not, it tells the user why their input is not usable, and waits for a new
 * input. Once a valid value has been inputed, the method returns that value.
 */
int getNValue() {
    string nString;
    while(true) {
        cout << "Value of N? ";
        getline(cin, nString);
        if(isInteger(nString)) {
            if(stringToInteger(nString) >= 2) {
                break;
            } else {
                cout << "N must be 2 or greater." << endl;
            }
        } else {
            cout << "Illegal integer format. Try again." << endl;
        }
    }
    return stringToInteger(nString);
}

/*
 * This method reads in a string and checks if the string is a positive integer.
 * If it is, the method returns true. If it contains non-number chars, the method
 * returns false. A special if statement is made for empty strings, which are not
 * numbers, but would return true because their lack of dimension would not trigger
 * the for loop.
 */
bool isInteger(string str) {
    bool isInt = true;
    if(!(str == "")) {
    for(int i = 0; i < (int)str.size(); i++) {
        if( !(str[i] >= '0' && str[i] <= '9')) {
            isInt = false;
        }
    }
    } else {
        isInt = false;
    }
    return isInt;
}

/*
 * This method creates the Map that stores each "prefix" (set of N-1 words that
 * appear sequentially in the text) as the keys and a Vector of every "suffix" (Nth
 * word) that follow each prefix. It starts by opening an ifstream to read in the
 * file, then it places the first N-1 words of the file into the last N-1 places of
 * a Vector titled wrapAround. This array will be used in matching the last words of
 * the file with the first words of the file, thus "wrapping" the file around. These
 * words are also read into a vector called window, which will be used to keep track
 * of the N words that need to be added to the Map. Once the wrapAround, window, and
 * Map collections have been declared and formated, they are all (as well as the
 * ifstream) passed to a method titled fillMap that actually does the bulk of the
 * word analysis and Map creation.
 */
void createMap(Map<string, Vector<string> >& prefSufMap, Vector<string>& window,
               string filename, int nVal, Vector<string>& wrapAround){
    ifstream input;
    input.open(filename.c_str());
    for(int i = nVal-1; i < wrapAround.size(); i++) {
        string word;
        input >> word;
        window.add(word);
        wrapAround.set(i, word);
    }
    fillMap(window, prefSufMap, input, wrapAround, nVal);

}

/*
 * This method recieves the window vector (with the first N-1 words already added),
 * the prefix and suffix map, the ifstream reading the file, the wrapAround vector,
 * and the user-generated value of N. It uses these to read in each word of the file,
 * add it to the end of the window, store a string of the first N-1 words of the
 * window as a prefix in the Map, then store the last word of the window (the suffix)
 * in the Vector value for the prefix key. Then, the first word in the window is
 * removed so that the window vector has shifted for the next iteration of the loop.
 * After this loop has executed, the wrapping around of the first and last words in
 * the file is done.
 */
void fillMap(Vector<string>& window, Map<string, Vector<string> >& prefSufMap,
             ifstream& input, Vector<string>& wrapAround, int nVal) {
    string newWord;
    while(!input.eof()) {
        input >> newWord;
        window.add(newWord);
        string prefix = getStringFromVector(window, 0, window.size()-2);
        prefSufMap[prefix].add(window.get(window.size()-1));
        window.remove(0);
    }
    finishWrapAround(window, wrapAround, prefSufMap, nVal);
}

/*
 * This method is passed a Vector of strings as well as a starting and ending index,
 * and the method returns a string of each word from the starting to the ending index
 * in string form with one space between each word.
 */
string getStringFromVector(Vector<string> vect, int startInd, int endInd) {
    string finalString = "";
    finalString += vect[startInd];
    for(int i = startInd+1; i <= endInd; i++) {
        finalString += " ";
        finalString += vect[i];
    }
    return finalString;
}

/*
 * This method adds the final N-1 words of the file into the first N-1 places of the
 * wrapAround vector. Then, a for loop iterates over the wrapAround vector and adds
 * prefixes and suffixes to the Map as they were being added in the fillMap method.
 */
void finishWrapAround(Vector<string>& window, Vector<string>& wrapAround,
                      Map<string, Vector<string> >& prefSufMap, int nVal) {

    for(int i = 0; i < window.size(); i++) {
        wrapAround.set(i, window.get(i));
    }
    for(int i = 0; i < nVal-1; i++) {
        string prefix = getStringFromVector(wrapAround, i, i+(nVal-2));
        prefSufMap[prefix].add(wrapAround.get(i+(nVal-1)));
    }
}

/*
 * This method prompts the user to input a valid number of words for the program to
 * generate or to enter '0' to exit the program. Then, it checks if the user has
 * inputed a 0, and sets the boolean tracking if the user hasn't inputed a '0' to false.
 * If not, the program checks if it is a valid positive integer, and if this integer
 * is greater than or equal to the value of N. If the user has not, the program tells
 * the user why their number is invalid and reprompts the user until a valid number
 * has been inputed. The method returns the boolean true if the user is not quitting,
 * and false if they are.
 */
bool recieveValidWordCount(int& wordCount, int nVal) {
    string countString;
    bool countNonZero = true;
    while(true) {
        cout << "# of random words to generate (0 to quit)? ";
        getline(cin, countString);
        if(isInteger(countString)) {
            wordCount = stringToInteger(countString);
            if (wordCount == 0) {
                countNonZero = false;
                break;
            } else if (wordCount < nVal) {
                cout << "Must be at least " << nVal << " words." << endl;
            } else {
                break;
            }
        } else {
            cout << "Illegal integer format. Try Again." << endl;
        }
    }
    return countNonZero;
}

/*
 * This method generates random new texted using the map. It declares a vector that will
 * be used as the window of size N for generating the words, then initializes the vector
 * and final string after randomly choosing a prefix to start the word generation
 * process. Then, the method begins randomly generating suffixes to the prefixes in the
 * printWindow vector based on the suffixes that are stored in the map and adding these
 * suffixes to the final string. After the loop has generated the number of words the
 * user has requested, the method prints the final string.
 */
void generateRandomText(Map<string, Vector<string> >& prefSufMap,
                        int wordCount, int nVal){
    string finalString = "... ";
    Vector<string> printWindow;
    initWindowAndString(prefSufMap, printWindow, finalString);
    for(int i = nVal-1; i < wordCount; i++) {
        string nextWord = getNextWord(prefSufMap, printWindow);
        finalString += " " + nextWord;
        printWindow.remove(0);
        printWindow.add(nextWord);
    }
    finalString += " ...";
    cout << finalString << endl;
}

/*
 * This method randomly chooses a prefix from the keyset of the map and adds it to
 * the window vector as well as the final string of generated words.
 */
void initWindowAndString(Map<string, Vector<string> >& prefSufMap, Vector<string>& printWindow,
                           string& finalString) {
    Vector<string> keys = prefSufMap.keys();
    string prefix = keys.get(randomInteger(0, keys.size()-1));
    finalString += prefix;

    stringstream stream(prefix.c_str());
    string word;
    while(getline(stream, word, ' ')){
        printWindow.add(word);
    }
}

/*
 * This method returns a random suffix from the vector list stored as the value for the
 * prefix words of the window vector.
 */
string getNextWord(Map<string, Vector<string> >& prefSufMap, Vector<string> printWindow) {
    string prefix = getStringFromVector(printWindow, 0, printWindow.size()-1);
    Vector<string> possibleNexts = prefSufMap[prefix];
    return possibleNexts.get(randomInteger(0, possibleNexts.size()-1));
}
