/*  Name: Kevin Vo
 *  TA: Jimmy Lee
    This program implements N-Grams, which reads in a text file and
    randomly generates a sequence of words. The sequence of words is
    generated using the probability of which word will come next
    the user specifies the number of words that will be displayed.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "tokenscanner.h"
#include "vector.h"
#include "simpio.h"
#include "map.h"
#include "filelib.h"
#include "random.h"


using namespace std;
int readFile(int &N, string &filename);
void genRandom(Map<string, Vector<string> > &map, int randomWords, int N);
string filePrompt();
Map<string, Vector<string> > placeIntoMap(int N, string filename);
void randomPrompt(int N, Map<string, Vector<string> > &map);
void mapProgression(TokenScanner scanner, Vector<string> window, Map <string, Vector<string>>& map, int N, string nCase);
void welcomeScreen();

/* The main method I broke into three overarching tasks: reading the file, setting up the map
 * and then the randomization of the words.
*/
int main() {
    welcomeScreen();
    string filename;
    int N = 0;
    N = readFile(N,filename);
    Map<string, Vector<string>> map = placeIntoMap(N, filename);
    randomPrompt(N,map);
    cout << "Exiting." << endl;
    return 0;
}
/*Sets up the console window and welcome message*/
void welcomeScreen(){
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document" << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

/* This function handles the prompt for reading in a file
*/
string filePrompt(){
    string filename;
    while (true){
        filename = getLine("Input file name:");
        if (fileExists(filename)) break;
        cout << "Unable to open that file.  Try again." << endl;
    }
    return filename;
}
/* This prompts the user for the value of N for the N grams
*/
int promptN(){
    int N = 0;
    while (true){
        N = getInteger("Value of N? ")- 1;
        if (N >= 2) break;
        cout << "N has to be greater than or equal to 2" << endl;
    }
    return N;
}

/*This function is in charge of reading the file and returning the value of N*/
int readFile(int& N, string& filename){
    filename = filePrompt();
    ifstream input;
    N = promptN();
    input.open(filename);
    return N;
}
/* This function sets up a map and then returns the map. The strategy for reading the
 * map is through the use of a token scanner. I also wrote a special case for the
 * wrap around case. This case is represented by the while loop. mapProgression is for
 * the rest of the case.
*/
Map<string,Vector<string>> placeIntoMap(int N, string filename){
    ifstream input;
    Vector <string> window;
    string nCase;
    Map < string, Vector<string>> map;
    input.open(filename);

    TokenScanner scanner (input);
    scanner.ignoreWhitespace();
    scanner.addWordCharacters("',.!?-;:][");

    while (scanner.hasMoreTokens() && window.size() < N){
          string word = scanner.nextToken();
          nCase += (word + " ");
          window.add(word);
    }
    mapProgression(scanner,window,map,N,nCase);
return map;
}
/* This function handles the bulk of setting up the map. I create window with a vector. I then ccnvert the contents
 * into string. I create a window of N size so {to, be, here} I then take the first two convert them into a string
 * and then take the third value and use it as the value. I also create a check to make sure that if the key already
 * exists then I do not overwrite the previous value. The end boolean is to trigger the wrap around.
*/
void mapProgression(TokenScanner scanner, Vector<string> window, Map <string, Vector<string>>& map, int N, string nCase){
     bool reachedEnd = false;
     string grams;
     Vector <string> value;
     while (true){
       while (scanner.hasMoreTokens()){
           string word = scanner.nextToken();
           window.add(word);
           value.add(window[N]);
           for (int i = 0; i < window.size()-1; i++) {
                 grams += window[i];
                 grams += " ";
           }
           window.remove(0);
           if (!map.containsKey(grams)){
                map.put(grams,value);
           }else{
               Vector<string> temp = map.get(grams);
               temp.add(word);
               map.put(grams,temp);
           }
           grams = "";
           value.clear();
         }
       if (reachedEnd) break;
       reachedEnd = true;
       scanner.setInput(nCase);
   }
}

/* This function is in charge of prompting the user for the number of words to print out.
*/
void randomPrompt(int N, Map<string, Vector<string>>& map){
    int randomWords = 0;
    while (true){
        while (true){
            randomWords = getInteger("# of random words to generate (0 to quit)? ");
            if (randomWords >= N + 1) break;
            if (randomWords == 0) return;
            cout << "Must be at least " << N + 1 << " words" << endl;
            cout << endl;
        }
        genRandom(map,randomWords, N);
        cout<<endl;
    }
}
/* This function generates the sequence of random words. The strategy was to
 * generate a value between the map and the size, iterate through the map until
 * I hit the value. Take the value of that key, which is a vector. I generate another
 * integer between the 0 and the size of the vector and perform the same operation.
 * I then concatenate the randomly picked word to the output string. Note: picking
 * the word from the vector is not entirely random because the probabilities of the word
 * is dependent on the number of elements and repeated occurences.
*/
void genRandom(Map<string,Vector<string>>& map, int randomWords, int N){
    int count = 0;
    string output;
    string select = "";
    string slidingWindow;
    string nextword;
    int rando = 0;
    int vrando = 0;
    for (int i = 0; i < randomWords - 1; i++){
        rando = randomInteger(0,map.size());
        for (string str : map.keys()) {
            select = str;
            count++;
            if (count == rando) break;
        }
        if (i < N){
            slidingWindow = select;
            output = select;
        }
        Vector<string> possible = map.get(select);
        vrando = randomInteger(0,possible.size()- 1);
        nextword = possible[vrando];
        output += (nextword + " ");
        slidingWindow += (nextword + " ");
        // the +1 accounts for the whitespace
        slidingWindow = slidingWindow.substr(slidingWindow.find(" ")+ 1);
        count = 0;
    }
   cout << "..." << output << "..." <<endl;
}



