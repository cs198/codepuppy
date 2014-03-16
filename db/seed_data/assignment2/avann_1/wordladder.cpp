/* By: Alyssa Vann
 * This the Word Ladder Assignment 2 for CS106B. This program
 * accepts two words from the user (which are checked for validity),
 * and it outputs a ladder of words connecting the two words, with
 * character differences of one.
 * For this assignment, I referenced lecture notes and received
 * help from my section leader.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "set.h"
#include "hashset.h"
#include "simpio.h"
#include "strlib.h"
#include "stack.h"
#include "queue.h"

using namespace std;

void greeting();
void readFile(HashSet<string>& dictionary);
bool getWords(string& word1, string& word2, HashSet<string>& dictionary, bool& play);
void createLadder(string& word1, string& word2, HashSet<string>& dictionary);
void repeatCheck(Stack<string> ladder, HashSet<string>& checkrepeats);
void printLadder(Stack<string>& ladder, string& word1,string& word2);
void findNeighbors(string& topword, HashSet<string>& dictionary, HashSet<string>& checkrepeats,
                   bool& solution, Stack<string>& ladder, Queue<Stack<string> >& partials,
                   string& word1, string& word2);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    greeting();

    HashSet<string> dictionary;
    string word1;
    string word2;

    readFile(dictionary);

    bool play = true;

    while(true){
        getWords(word1, word2, dictionary, play);

        if(!play) {
            cout << "Have a nice day!" << endl;
            break;
        }

        createLadder(word1, word2, dictionary);
    }

    return 0;
}

/* The greeting function outputs a greeting to the user at the start
 * of the program.
 */
void greeting(){
    cout << "Welcome to CS106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
}

/* The readFile function reads in the dictionary file, and inputs
 * all its entries into a HashSet, so that later functions can
 * see if a word is contained in the dictionary
 */
void readFile(HashSet<string>& dictionary){
    ifstream input;
    input.open("dictionary.txt");
    string line;
    while(true){
        bool success = getline(input,line);
        if(!success){
            break;
        }
        dictionary.add(line);
    }

    input.close();
}

/* The getWords function asks the user for two words to create a
 * word ladder. It stores these words in temporary strings until
 * the user gives a valid input. If the user clicks Enter, then
 * then the function returns a boolean that causes the program
 * to stop.
 */
bool getWords(string& word1, string& word2, HashSet<string>& dictionary, bool& play){
    string temp1;
    string temp2;

    do{
        temp1 = getLine("Word #1 (or Enter to quit): ");
        temp1 = toLowerCase(temp1);
        if(temp1==""){
            play = false;
            return play;
        }

        if(!dictionary.contains(temp1)){
            cout << "Not a valid input." << endl;
        }

    }while(!dictionary.contains(temp1));

    word1 = temp1;

    int length = word1.length();

    do{
        temp2 = getLine("Word #2 (or Enter to quit): ");
        temp2 = toLowerCase(temp2);
        if(temp2==""){
            play = false;
            return play;
        }

        if(!dictionary.contains(temp1) || length!=temp2.length()){
            cout << "Not a valid input." << endl;
        }

    }while(!dictionary.contains(temp2) || length!=temp2.length());

    word2 = temp2;
}

/* The createLadder function is primarily responsible for creating the
 * word ladder, using a queue of partial ladders. It loops over these
 * partial ladders, checking neighbors, until it finally finds a
 * solution (when the word equals the second word). At which point
 * it stops, allowing the word ladder to be outputted.
 */
void createLadder(string& word1, string& word2, HashSet<string>& dictionary){
    Queue<Stack<string> > partials;
    Stack<string> first;
    first.push(word1);
    partials.enqueue(first);

    HashSet<string> checkrepeats;
    checkrepeats.add(word1);

    bool solution = false;

    while(!partials.isEmpty()){
        Stack<string> ladder = partials.dequeue();

        string topword = ladder.peek();

        findNeighbors(topword, dictionary, checkrepeats,
                      solution, ladder, partials, word1, word2);

        if(solution) break;
    }
}

/* The findNeighbors function is called within createLadder, to check
 * for when a solution has been found. It changes the characters in
 * word1, finding all the words that exist in the dictionary, and it puts
 * these into stacks, which are added to the partial ladders, until a
 * solution is found, when it prints the ladder.
 */
void findNeighbors(string& topword, HashSet<string>& dictionary, HashSet<string>& checkrepeats,
                   bool& solution, Stack<string>& ladder, Queue<Stack<string> >& partials,
                   string& word1, string& word2){
    for(int i=0; i<topword.length(); i++){
        for(char c='a'; c<='z'; c++){
            string neighbor = topword;
            neighbor[i]=c;
            if(dictionary.contains(neighbor) && !checkrepeats.contains(neighbor)){
                if(neighbor == word2){
                    solution = true;
                    printLadder(ladder, word1, word2);
                }else{
                    Stack<string> newladder = ladder;
                    newladder.push(neighbor);
                    partials.enqueue(newladder);
                    checkrepeats+=neighbor;
                }
            }
        }
    }
}

/* The printLadder function is called to output the ladder
 * connecting word1 to word2 when a solution is found.
 */
void printLadder(Stack<string>& ladder, string& word1,string& word2){
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    cout << word2 << " ";

    while(!ladder.isEmpty()){
        string print = ladder.pop();
        cout << print << " ";
    }
    cout << endl << endl;
}
