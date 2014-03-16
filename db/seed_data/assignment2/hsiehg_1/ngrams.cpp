// ngrams.cpp
// N-Grams allows the user to enter a text file and then takes is able to generate a random sequence of words based on the word
// and sentece patterns in this file. To generate the random sequences, N-Grams builds a map of prefixes (keys) to suffixes (values)
// which is based on the number N that the user inputs. N determines how many words the prefix will contain - the prefix will always be N-1
// length long. After building this map, the program links the prefixes and suffixes from the map together to create sequences of words
// that reflect the original file's sentence structure.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "hashmap.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/*Function Prototypes*/
void printIntro();
void getWordList(Vector<string>& wordsList);
int getN();
int getNRandWord(int n);
void getWordMap (HashMap < Vector <string>, Vector <string> >& wordMap, Vector<string>& wordsList, int n);
void getRandomStart(Vector<string>& firstVec, HashMap < Vector <string>, Vector <string> >& wordMap);
void getNextVec (Vector<string>& nextVec, HashMap < Vector <string>, Vector <string> >& wordMap, string& curWord);
string createSentence(int nRandWords, Vector <string>& curVec, int n, HashMap < Vector <string>,
                      Vector <string> >& wordMap);
void getPrefixVec (int index, int n, Vector<string>& wordsList, Vector<string>& prefixVec);
void getSuffixVec(int index, int n, Vector<string>& wordsList, Vector<string>& prefixVec,
                  HashMap < Vector <string>, Vector <string> >& wordMap, Vector<string>& suffixVec);

/*
 * Program main
 * Prints the introductory message and then creates a vector of the words from the file where every index contains one word.
 * Prompts the user for N before creating the wordMap and then prompts the user repeatedly for the number of random words
 * that they would like to generate.
 * While the number of random words is not equal to 0 (which is the exit value for the program), a random vector will be chosen
 * from the wordMap as the start, a sentence will be created and the user will be reprompted for the number of random words.
 */
int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntro();
    Vector<string> wordsList;
    getWordList(wordsList);
    int n = getN();
    HashMap < Vector <string> , Vector <string> > wordMap;
    getWordMap(wordMap, wordsList, n);
    int nRandWords = getNRandWord(n);
    while(nRandWords!= 0) {
        Vector <string> curVec;
        getRandomStart(curVec, wordMap);
        createSentence(nRandWords, curVec, n, wordMap);
        nRandWords = getNRandWord(n);
    }
    cout << "Exiting." << endl;
    return 0;
}

/*
 * Creates the randomly generated sentence from the wordMap (which was generated from the file).
 * Adds to the result string until the number of random words is less than N, and then concatenates the left-over words (which will be
 * between 0 and n) onto the result.
 * Prints the result at the end of the function.
 */
string createSentence(int nRandWords, Vector <string>& curVec, int n,
                      HashMap < Vector <string> , Vector <string> >& wordMap) {
    string result = "... ";
    while (nRandWords >= n) {
        for(int i = 0; i < curVec.size(); i++) {
            result += curVec[i] + " ";
        }
        nRandWords -= (n - 1);
        getNextVec(curVec, wordMap, wordMap[curVec][0]);
    }

    for(int i = 0; i < nRandWords; i++) {
        result += curVec[i] + " ";
    }

    cout << result + " ..." << endl;
    cout << endl;
}

/*
 * Updates the next vector (passed by reference) to be the next valid vector.
 * Iterates through all the keys in the wordMap to find keys that contain the previous word in the first index and then
 * randomly chooses one key out of that collection as the next starting vector.
 */
void getNextVec (Vector<string>& nextVec, HashMap < Vector <string>, Vector <string> >& wordMap,
                 string& curWord) {
    Vector < Vector <string> > validKeys;
    for(Vector<string> k: wordMap) {
        if(k[0] == curWord) {
            validKeys.add(k);
        }
    }
    int rIndex = randomInteger(0, validKeys.size() - 1);
    nextVec = validKeys[rIndex];
}

/*
 * Updates the first vector (passed by reference) to be a random vector from the wordMap.
 * Creates a vector of keys from the wordMap and then randomly selects one of those keys as the starting vector.
 */
void getRandomStart(Vector<string>& firstVec, HashMap < Vector <string>, Vector <string> >& wordMap) {
   Vector < Vector <string> > keys = wordMap.keys();
   int rIndex = randomInteger(0, keys.size() - 1);
   firstVec = keys[rIndex];
}

/*
 * Takes in the wordMap by reference and updates it so that it contains all the respective prefix keys and suffix values.
 * This function accounts for the wrap-around in the list of words from the file.
 */
void getWordMap (HashMap < Vector <string>, Vector <string> >& wordMap, Vector<string>& wordsList, int n) {
    for(int i = 0; i < wordsList.size() + (n - 1); i++) {
        int tempIndex = i;
        if(i > wordsList.size() - 1){
            tempIndex = (i % wordsList.size());
        }
        Vector <string> prefixVec;
        getPrefixVec(tempIndex, n, wordsList, prefixVec);
        Vector <string> suffixVec;
        getSuffixVec(tempIndex, n, wordsList, prefixVec, wordMap, suffixVec);
        wordMap.put(prefixVec, suffixVec);
    }
}

/*
 * Takes in the suffixVec by reference and adds the suffix associated with the prefixVec to the map.
 * This function accounts for the wrap-around in the list of words from the file.
 * If the map already contains the prefixVec corresponding to the suffixVec, the word will be added into the
 * existing vector value. Otherewise, a new vector will be created to hold the suffix.
 */
void getSuffixVec(int index, int n, Vector<string>& wordsList, Vector<string>& prefixVec,
                  HashMap < Vector <string>, Vector <string> >& wordMap, Vector<string>& suffixVec) {
    int sIndex = (index + (n - 1));
    if(sIndex > wordsList.size() - 1) {
        sIndex %= wordsList.size();
    }
    if(!wordMap.containsKey(prefixVec)){
        suffixVec.add(wordsList[sIndex]);
    }
    else {
        suffixVec = wordMap.get(prefixVec);
        suffixVec.add(wordsList[sIndex]);
    }
}

/*
 * Creates the prefix vector that will be placed into the map as the key.
 * Adds N words to the prefixVec from the wordsList. This function accounts for wrap-arounds by checking to see if the
 * index will be greater than the size of the wordsList vector.
 */
void getPrefixVec (int index, int n, Vector<string>& wordsList, Vector<string>& prefixVec) {
    for(int j = index; j < index + (n - 1); j++) {
        int tempJ = j;
        if(j >= wordsList.size() - 1) {
            tempJ = j % (wordsList.size());
        }
        prefixVec.add(wordsList[tempJ]);
    }
}

/*
 * Returns a valid integer for the value of N.
 * The user is repeatedly prompted for the value of N. The value of N must be 2 or greater.
 */
int getN() {
    int n = getInteger("Value of N? ");
    while(n < 2) {
       cout << "N must be 2 or greater." << endl;
       n = getInteger("Value of N? ");
    }
    cout << endl;
    return n;
}

/*
 * Returns a valid number for the number of random words to generate.
 * Given the value of N, the user will be continuously prompted for a value for the random number of words. The random numbe of words to
 * generate must not be equal to 0 and must be greater or equal to n.
 */
int getNRandWord(int n) {
    int nRandWords = getInteger("# of random words to generate (0 to quit?) ");
    while(nRandWords < n && nRandWords > 0) {
        cout << "Must be at least " << n << " words." << endl;
        cout << endl;
        nRandWords = getInteger("# of random words to generate (0 to quit?) ");
    }
    return nRandWords;
}

/*
 * Reads in the file that the user enters and updates the wordsList vector so that every index contains one word.
 */
void getWordList(Vector<string>& wordsList) {
    ifstream input;
    promptUserForFile(input, "Input file name? ");
    while(!input.fail()){
        string word;
        input >> word;
        if(word != "") {
            wordsList.add(word);
        }
    }
}

/*
 * Prints the introductory message at the beginning of the program.
 */
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}
