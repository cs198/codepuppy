/* Name: Austin Poore
 *
 * This program creates random text based on a given input file by creating a database of
 * "N-grams" and the words that follow them in the source text. The program accepts user
 * input for the file name, length of the "N-grams", and desired number of words of
 * random text.
 *
 * I got the isspace function from string lecture overflow slides. Otherwise, I used regular
 * lecture slides.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "hashmap.h"
#include "vector.h"
#include "random.h"

using namespace std;

void printIntroduction();
void getInputFromUser(int& n, string& fileName);
void readFileIntoMap(int n, string fileName, HashMap<Vector<string>, Vector<string>>& grams);
Vector<string> initializeWindow(ifstream& input, Vector<string>& window, int n);
void storeRestOfFile(ifstream& input, Vector<string>& window, Vector<string>& windowCopy, int n, HashMap<Vector<string>, Vector<string>>& grams);
void addToMap(string& nextWord, Vector<string>& window, HashMap<Vector<string>, Vector<string>>& grams);
void updateWindow(string& nextWord, Vector<string>& window, int n);
string generateOutput(HashMap<Vector<string>, Vector<string> > &grams, int n, int words);
string appendNextWord(Vector<string>& newWindow, HashMap<Vector<string>, Vector<string>>& grams, string& result);
bool getNumWords(int& words, int n);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    printIntroduction();
    int n;
    int words;
    string fileName;
    HashMap<Vector<string>, Vector<string>> grams;
    getInputFromUser(n, fileName);
    readFileIntoMap(n, fileName, grams);
    while(true) { // User can generate as many instances of random text as desired.
        if(getNumWords(words, n)) {
            string output = generateOutput(grams, n, words);
            cout << output << endl;
            cout << " " << endl; // Formatting
        } else {
            break; // User types a 0, program exits.
        }
    }
    cout << "Exiting." << endl;
    return 0;
}

/*
 * This function gets called at the start of the program and displays the
 * introductory text. This text is the first thing the user will see on the
 * console.
 */

void printIntroduction() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

/*
 * This function reads in the file name and N-value from the user, ensuring
 * that both meet the criteria for a valid input (namely that the file exists
 * and that N is an integer greater than or equal to 2. The values are stored
 * in the reference parameters int n and string fileName.
 */

void getInputFromUser(int& n, string& fileName) {
    while (true) {
        fileName = getLine("Input file name? ");
        if (fileExists(fileName)) {
            break;
        }
        cout << "File not found; please try again." << endl;
    }
    while (true) {
        n = getInteger("Value of N? ");
        if (n >= 2) {
            break;
        }
        cout << "N must be 2 or greater." << endl;
    }
    cout << " " << endl;
}

/*
 * This function reads the input file provided by the user into a HashMap.
 */

void readFileIntoMap(int n, string fileName, HashMap<Vector<string>, Vector<string>>& grams){
    ifstream input;
    Vector<string> window;
    input.open(fileName.c_str());
    Vector<string> windowCopy = initializeWindow(input, window, n); // Stores first n words of the file in the vector.
    storeRestOfFile(input, window, windowCopy, n, grams); // Finishes storing the file, including wrapping back to the front.
}

/*
 * This function reads an integer from the user to determine how many words of random text
 * he/she wants to generate. If the input is 0, the method returns false to break the loop
 * in the main method. Otherwise, it checks to make sure the integer is greater than n. If it
 * is not, the user is reprompted. After the validity of the input is confirmed, it returns true.
 */

bool getNumWords(int& words, int n) {
    while (true) {
        words = getInteger("# of random words to generate (0 to quit)? ");
        if (words == 0) {
            return false;
        }
        if (words >= n) {
            break;
        }
        cout << "Must be at least " << n << " words." << endl;
        cout << " " << endl;
    }
    return true;
}

/*
 * This function sets up the vector which stores the current "window" of text while the file gets
 * read in. It uses a counter to store the first "N" words of the file. It uses the fact that
 */

Vector<string> initializeWindow(ifstream& input, Vector<string>& window, int n) {
    string word;
    int counter = 0;
    char curr;
    char prev;
    while (true) {
        curr = input.get();
        if (!isspace(curr)) {
            word += curr;
        } else if (isspace(curr) && !isspace(prev)){ // For places in text file with weird spacing
            window.add(word);
            word = "";
            counter++;
        }
        prev = curr;
        if (counter == (n - 1)) break;
    }
    return window;
}

/*
 * This function stores the rest of the file (after the first "N" words) in the HashMap. It also iterates an additional "N" times to loop back
 * around to the start of the file.
 */

void storeRestOfFile(ifstream& input, Vector<string>& window, Vector<string>& windowCopy, int n, HashMap<Vector<string>, Vector<string>>& grams){
    string nextWord;
    char curr; // Character most recently read in.
    char prev; // Previous character.
    while (!input.fail()) {
        curr = input.get();
        if (!isspace(curr)) {
            nextWord += curr;
        } else if (isspace(curr) && !isspace(prev)){ // For places in text file with weird spacing
            addToMap(nextWord, window, grams);
            updateWindow(nextWord, window, n);
        }
        prev = curr;
    }
    string lastWord = nextWord.substr(0, nextWord.length() - 1); // Get rid of character at end of file.
    if(lastWord != "") { // Store the last word if not a blank line.
        addToMap(lastWord, window, grams);
        updateWindow(lastWord, window, n);
    }
    for (int j = 0; j < (n-1); j++) { // Loop back to the start of the file to avoid getting "stuck"
        string word = windowCopy[j];
        addToMap(word, window, grams);
        updateWindow(word, window, n);
    }
}

/*
 * This function adds a word to the list of possible suffixes to a given string of words.
 */

void addToMap(string& nextWord, Vector<string>& window, HashMap<Vector<string>, Vector<string>>& grams){
    Vector<string> suffixes = grams[window];
    suffixes.add(nextWord);
    grams[window] = suffixes;
}

/*
 * This function shifts all of the words forward a spot in the "Window" vector and then adds the
 * next word. It also resets the string nextWord to the empty string, ready to read the next word.
 */

void updateWindow(string& nextWord, Vector<string>& window, int n) {
    for (int i = 0; i < (n-2); i++) {
        window[i] = window[i + 1];
    }
    window[n - 2] = nextWord;
    nextWord = "";
}

/*
 * This function builds the string of randomly generated text by selecting a random starting
 * point and then appending words one at a time to the result string. It makes use of the
 * updateWindow function from earlier, because a different window is used to track the output being
 * generated.
 */

string generateOutput(HashMap<Vector<string>, Vector<string>>& grams, int n, int words){
    string result = "... ";
    Vector<string> newWindow;
    Vector<Vector<string>> gramsKeys = grams.keys();
    newWindow = gramsKeys[randomInteger(0, grams.size() - 1)];
    for (int j = 0; j < n - 1; j++) {
        result += newWindow[j];
        result += " ";
    }
    for (int i = 0; i < words - (n-1); i++) {
        string word = appendNextWord(newWindow, grams, result);
        updateWindow(word, newWindow, n);
    }
    result +=  "...";
    return result;
}

/*
 * This function pulls the vector of possible next words out of the HashMap and then randomly selects one to append
 * to the result string, followed by a space. It returns the word most recently added.
 */

string appendNextWord(Vector<string>& newWindow, HashMap<Vector<string>, Vector<string>>& grams, string& result) {
    Vector<string> possibilities = grams[newWindow];
    int numPoss = possibilities.size();
    int next = randomInteger(0, numPoss - 1);
    string nextOutput = possibilities[next];
    result += nextOutput;
    result += " ";
    return nextOutput;
}
