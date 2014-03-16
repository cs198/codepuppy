// This is the ngrams file. It has commented out code
// that is almost functional, however it has a unidentified
// bug that causes the program to go out of bounds. With the
// code commented out the program functions near perfectly, except
// it treats any punctuation as its own word.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "strlib.h"
#include "console.h"
#include "vector.h"
#include "map.h"
#include "hashmap.h"
#include "filelib.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "random.h"

using namespace std;
void introMessage();
void fileReader(Vector<string>&);
HashMap<Vector<string>, Vector<string>> buildMap(int &N, Vector<string> &wordSource);
void runProgram(HashMap<Vector<string>, Vector<string>>& KeyMap, int& N);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    Vector<string> wordSource;
    introMessage();
    int N;
    fileReader(wordSource);
    N = getInteger("Value of N: ");
    HashMap<Vector<string>, Vector<string>> KeyMap = buildMap(N, wordSource);
    runProgram(KeyMap, N);
    cout << "Exiting." << endl;
    return 0;
}
/**
 * @brief runProgram
 * @param KeyMap
 * @param N
 * runProgram() runs the program using recursion to loop through
 * the program until the user quits.
 */
void runProgram(HashMap<Vector<string>, Vector<string>>& KeyMap, int& N) {
    int numWords;
    numWords = getInteger("# of random words to generate (0 to quit)? ");
    if(numWords != 0) {
        Vector<string> key = KeyMap.keys()[randomInteger(0, KeyMap.keys().size() - 1)];
        for(int a = 0; a < N; a++) {
            cout << key[a] << " ";
        }
        for(int i = 0; i < numWords - 2; i++) {
            if(KeyMap.containsKey(key)) {
                Vector<string> nextValue = KeyMap.get(key);
                string nextWord = nextValue[randomInteger(0, nextValue.size() - 1)];
                cout << nextWord << " ";
                key.remove(0);
                key.add(nextWord);
            }else {
                key = KeyMap.keys()[randomInteger(0, KeyMap.keys().size() - 1)];
            }
        }
        cout << " " << endl;
        cout << " " << endl;
        runProgram(KeyMap, N);
    }
}
/**
 * @brief buildMap
 * @param N
 * @param wordSource
 * @return
 * buildMap() builds the map that the program runs off of.
 * it is supposed to have a check for punctuation built in,
 * however that was causing it to go out of bounds, therefore
 * it is commeneted out. The program, when not commented out works
 * excepted when there is a lot of empty space, which causes it to
 * go out of bounds.
 */
HashMap<Vector<string>, Vector<string>> buildMap(int& N, Vector<string>& wordSource) {
    HashMap<Vector<string>, Vector<string>> mapBeingBuilt;
    Vector<string> key;
    while(true) {
        if(N >= wordSource.size() - 1)break;
        key.clear();
        string hold;
        int a = N;
        for(int i = 0; i <  a; i++){
            hold = wordSource.get(i);
            key += hold;
//            if(hold == " "){
//                if(i == 0) {
//                    a = 0;
//                }else {
//                    a++;
//                }
//            }else{
//                if(hold.length() == 1) {
//                    if(ispunct(stringToChar(hold))) { //deals with punctuation
//                        if(i == 0){
//                           a = 0;
//                           if(wordSource.get(1) != " "){
//                               wordSource.remove(1);
//                           }
//                        } else {
//                            string temp;
//                            temp = key.get(key.size() - 1);
//                            temp += hold;
//                            if(wordSource[i + 1] != " "){
//                                temp += wordSource[i + 1];
//                                a++;//add i++
//                            }
//                            key.insert(key.size() - 1, temp);

//                        }
//                    }else {
//                        key += hold; //deals with single letter words.
//                    }
//                }else {//deals with regular words.
//                    key += hold;
//                    if(wordSource[i + 1] != " " && i == a - 1){
//                        string temp;
//                        temp = key.get(i - 1);
//                        temp += wordSource[i + 1];
//                    }
//                }
//            }
        }
        if(!mapBeingBuilt.containsKey(key) && a != 0) {
            Vector<string> value = mapBeingBuilt.get(key);
            value += wordSource[a + 1];
            mapBeingBuilt.put(key, value);
        }else if (a != 0) {
            Vector<string> value = mapBeingBuilt.get(key);
            value += wordSource.get(a + 1);
        }
        wordSource.remove(0);
    }
 //   cout << mapBeingBuilt.toString() << endl; //for testing purposes
    return mapBeingBuilt;
}
/**
 * @brief fileReader
 * @param wordSource
 * fileReader() opens a file that the user specifies and
 * reads all of the words into a vector of strings.
 */
void fileReader(Vector<string>& wordSource) {
    ifstream input;
    promptUserForFile(input, "Input file name: ");
    TokenScanner scanner;
    string line;
    string word;
    scanner.ignoreWhitespace(); // take out when the punctuation checker is not commented out.
    while(getline(input, line)){
        scanner.setInput(line);
        while(scanner.hasMoreTokens()) {
            word = scanner.nextToken();
            wordSource += word;
        }
//        wordSource += " "; //adds a space to the end of each line. put in for punct. checker.
    }
    input.close();
 //   cout << wordSource.toString() << endl; // for testing purposes.
}
/**
 * @brief introMessage
 * introMessage() prints out the starting text.
 */
void introMessage() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}
