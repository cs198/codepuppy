// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
using namespace std;

void createMap(istream& , Map <string , Vector<string> >&);
string grammarGenerateRecurse(Map <string , Vector<string> >& , string);


int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

int convertStringToInteger(string exp) {
    if (exp.size() == 1){
        //base case: if there's 1 number left it returns that number
        return exp[0] - '0';
    } else{
        //recursive case
        if (exp[0] == '-'){
            //if the first term is a "-" sign it returns -1 times the rest of number
            string temp = exp.substr(1 , exp.size()); //creates a new string without the first character of the old one
            return -1 * convertStringToInteger(temp);
        } else{
            //recursive case: returns the number of the character times a power of 10
            //equvalent to the amount of terms left and then adds the rest of the numbers recusively
            int number = (exp[0] - '0') * pow(10.0 , double(exp.size()-1)); //gets the value of the first character
            string newStr = exp.substr(1 , exp.size()); //creates a new string without the first character of the old one
            return number + convertStringToInteger(newStr); //recurses
        }
    }
}

bool isBalanced(string exp) {
    if (exp == ""){
        //base case: if the string is empty it's balanced
        return true;
    } else{
        //recursive case
        if (exp.find("()") != exp.npos){ //if there is "()" in the string
            exp.erase(exp.find("()") , 2); //erases them
            return isBalanced(exp); //recurses
        } else if(exp.find("[]") != exp.npos){ //if there is "[]" in the string
            exp.erase(exp.find("[]") , 2); //erases them
            return isBalanced(exp); //recurses
        } else if(exp.find("{}") != exp.npos){ //if there is "{}" in the string
            exp.erase(exp.find("{}") , 2); //erases them
            return isBalanced(exp); //recurses
        } else{
            return false; //if none of the previous cases are true, the string is inbalanced
        }
    }
}

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order > 0){
        //base case doesn't need to be written since we don't draw anything if the order is 0
        //recursive case

        double pi = 3.14159265358979323846264338327950288419716939937510582; //value of pi

        //draws the three lines of the larger triangle
        gw.drawLine(x , y , x+size , y);
        gw.drawLine(x , y , x+(size*cos(60*pi/180)) , y+(size*sin(60*pi/180)));
        gw.drawLine(x+size , y , x+(size*cos(60*pi/180)) , y+(size*sin(60*pi/180)));

        //recurses for each of the three smaller triangles nested in the previous one with modified params
        drawSierpinskiTriangle(gw , x , y , size/2 , order-1);
        drawSierpinskiTriangle(gw , x+(size/2) , y , size/2 , order-1);
        drawSierpinskiTriangle(gw , x+((size/2) * cos(60*pi/180)) , y+((size/2) * sin(60*pi/180)) , size/2 , order-1);

    }

}

void floodFillHelper(int x, int y, int width, int height, int color, int originalColor) {
    //we don't need a base case
    if (color != originalColor){
        //if the color we clicked on isn't the same as the color we want to change to
        if (x + 1 < width - 1 && getPixelColor(x+1 , y) == originalColor){
            //if the pixel to the right is in bounds and the same color as the original color
            setPixelColor(x+1 , y , color); //sets it to the original color
            floodFillHelper(x+1 , y , width , height , color , originalColor); //recurses
        }
        if (x - 1 > 0 && getPixelColor(x-1 , y) == originalColor){
            //if the pixel to the left is in bounds and the same color as the original color
            setPixelColor(x-1 , y , color); //sets it to the original color
            floodFillHelper(x-1 , y , width , height , color , originalColor); //recurses
        }
        if (y + 1 < height - 1 && getPixelColor(x , y+1) == originalColor){
            //if the pixel to the top is in bounds and the same color as the original color
            setPixelColor(x , y+1 , color); //sets it to the original color
            floodFillHelper(x , y+1 , width , height , color , originalColor); //recurses
        }
        if (y - 1 > 0 && getPixelColor(x , y-1) == originalColor){
            //if the pixel to the bottom is in bounds and the same color as the original color
            setPixelColor(x , y-1 , color); //sets it to the original color
            floodFillHelper(x , y-1 , width , height , color , originalColor); //recurses
        }
    }
}

void floodFill(int x, int y, int width, int height, int color) {
    int originalColor = getPixelColor(x , y); //gets the original color that we clicked on
    floodFillHelper(x , y , width , height , color , originalColor); //calls the helper function with the original color passed
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    //checks to see if the user actually inputted a symbol
    if (symbol == ""){
        throw "Must input a string";
    }

    //checks to see if the user put in a positive number (or 0) for the amount of times
    while (times < 0){
        cout << "Must input a positive integer for amount to generate." << endl;
        times = getInteger("How many to generate? ");
        cout << endl;
    }

    Map <string , Vector<string> > grammarMap; //initializes the map

    createMap(input , grammarMap); //creates the map with the grammar pairings

    Vector<string> v; //initializes the return vector

    for (int i = 0 ; i < times ; i++){ //creates as many instances of the symbol as we want
        string phrase = grammarGenerateRecurse(grammarMap , symbol); //creates the phrase
        v.add(phrase); //adds it to the vector
    }

    return v; //returns the vector

}

void createMap(istream& input , Map <string , Vector<string> >& grammarMap){
    //creates the map with the pairings
    string line;
    while (getline(input , line)){ //while there are still lines in the file

        //separates the non-terminal from the rules
        Vector<string> grammarLine = stringSplit(line , "::=");
        string nonTerminal = grammarLine[0];
        string rulesString = grammarLine[1];

        //checks to see if the map already contains the key (non-terminal)
        if (grammarMap.containsKey(nonTerminal)){
            throw "Map contains two sets of rules for the same non-termina";
        }

        if (rulesString.find("\r") != rulesString.npos){
            //at the end of each line there's a "\r" character so this erases that
            rulesString.erase(rulesString.find("\r") , 2);
        }

        Vector<string> rules = stringSplit(rulesString , "|"); //separates all the rules and puts them in a vector
        grammarMap.put(nonTerminal , rules); //puts the string non-terminal and the vector of rules pairing into the map
    }
}

string grammarGenerateRecurse(Map <string , Vector<string> >& grammarMap , string symbol){
    //recursion helper function

    if (!grammarMap.containsKey(symbol)){
        //base case: the user inputted a symbol that represents no non-terminal
        return symbol; //we just return the symbol

    } else{
        //recursive case

        //the next three lines are used to determine whether or not the set of rules corresponding to the symbol are
        //non-terminals themselves or words
        Vector <string> rules = grammarMap.get(symbol); //gets the vector of rules for that symbol
        int randomInt = randomInteger(0 , rules.size()-1);
        string randomRule = rules[randomInt]; //gets a random rule (or set of other non-terminals) from it
        Vector<string> setOfRules = stringSplit(randomRule , " "); //splits the element into each individual rule (if they're non-terminals, otherwise this does nothing)

        //this whole next section is necessary because the file expression.txt has rules nested in parens
        int count = 0; //creates a counter
        for (int i = 0 ; i < setOfRules.size() ; i++){
            if (grammarMap.containsKey(setOfRules[i])){ //if any of the terms in the vector is itself a non-terminal
                count ++; //increment the counter
            }
        }

        if (count == 0){ //if any element of that vector is itself a key (a non-terminal)
            //if the rules associated with the non-terminal are words and not other non-terminals
            //we just pick a random word from the vector and print it

            return randomRule + " ";

        } else{
            //if the rules associated with the symbol are other non-terminals
            //we iterate over the non-terminals in each rule and recurse for each

            string phrase; //intializes the return phrase string
            for (string rule : setOfRules){ //for each rule in the vector
                phrase += grammarGenerateRecurse(grammarMap , rule); //recurses and adds it to the return string
            }
            return phrase;
        }
    }
}

