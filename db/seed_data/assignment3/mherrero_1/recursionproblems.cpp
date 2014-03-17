/*Filename: RecursionProblems
 *-------------------------------------------------------------------------
 * This program consists of a series of different excercises in recursion. These excercises include
 * convertStringToInteger, isBalanced, drawSierpinskiTriangle, floodFill, and grammarGenerate.
 * These methods are built to have the functionality described in the assignment handout.
 */

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

// prototyped methods
int convertStringToIntegerHelper(string exp);
bool isBalancedHelper(string exp, Stack<char>& unclosed);
void floodFillHelper(int x, int y, int width, int height, int color, int fillColor);
bool pixelInBounds(int x, int y, int width, int height);
void generateGrammarMap(istream& input, Map<string, Vector<string> >& grammarMap);
string getNextGrammar(string symbol, Map<string, Vector<string> >& grammarMap);

int countKarelPaths(int street, int avenue) {
    //I didn't do this :P
    return 0;
}

/*
 * This method accepts a string that contains a valid integer and converts it to an integer form. First,
 * it checks if the number is negative or positive. If it is positive, it calls the recursive helper
 * method to get the int version of the number and returns it. If the number is negative, it calls the
 * helper function and returns the value passed from the helper function * -1.
 */
int convertStringToInteger(string exp) {
    if (exp[0] == '-') {
        int absVal = convertStringToIntegerHelper(exp.substr(1));
        return absVal*-1;
    } else {
        return convertStringToIntegerHelper(exp);
    }
}

/*
 * This method recursively converts a string containing an integer into its int value. If the string
 * passed is only one char long, then the char is typecast as an int and then 48 is subtracted to get
 * the char as an int. If the string is longer than 1 char, then the first char is converted to an
 * int and multiplied by the power of 10 that puts the digit in the correct digit's place.
 * The helper function is then called on the rest of the string.
 */
int convertStringToIntegerHelper(string exp) {
    if (exp.size() == 1) {
        return (int)exp[0] - 48;
    } else {
        int value = ((int)exp[0] - 48) * pow(10.0,(double)exp.size()-1);
        return value + convertStringToIntegerHelper(exp.substr(1));
    }
}

/*
 * This method determines if a string contains a balanced number of opened and closed brackets,
 * parenthesis, and braces and if the order of the opening and closing punctuations is balanced.
 * This starter method creates a stack that will store the unclosed punctuation points, and passes
 * this stack to the helper function, which returns true if the string is balanced and false if the
 * string is unbalanced.
 */
bool isBalanced(string exp) {
    Stack<char> unclosed;
    return isBalancedHelper(exp, unclosed);
}

/*
 * This is the recursive helper function that is used by the "isBalanced" method. It reads in a string
 * and determines if the brackets, braces, and parenthesis are balanced. First, if the string passed
 * to the method is empty (base case), the method returns true. Then, if the first character is an opening
 * punctuation, the char is pushed on to the stack so that the program can check for its equivalent closing
 * punctuation later. If this the last char in the string, false is returned. If the char is a piece of
 * closing punctuation, then the method pops the first piece of punctuation off of the stack to see if
 * it is the opening eqivalent of the punctuation just encountered. If this is not true (ASCII values differ
 * by more than 2) or the stack is empty, then false is returned. If not, the method is recursed using
 * the substring of every char except the first one.
 */
bool isBalancedHelper(string exp, Stack<char>& unclosed) {
    if(exp == "") {
        return true;
    } else if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {
        if(exp.length()==1) return false;
        unclosed.push(exp[0]);
    } else if (exp[0] == ')' || exp[0] == ']' || exp[0] == '}') {
        if (unclosed.isEmpty() || exp[0] - unclosed.pop() > 2) return false;
    }
    return isBalancedHelper(exp.substr(1), unclosed);
}

/*
 * This method draws a Sierpinski Triangle of the order indicated by the user. If the method is passed
 * an order of 1, then the method draws a triangle using the (x,y) coordinates as the top-left corner
 * of the triangle and the size as the length of one side. If the order is greater than 1, than the method
 * is recursively called in three places. The first uses the given (x,y) coordinate, the second uses a
 * point half a size distance to the right, and the third half a size distance below and right of the
 * (x,y) point, so that the three triangles will connect to form a larger triangle (one that would have
 * been drawn if order == 1). In each call, size is halved and order is decremented by 1.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order == 1) {
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x+size*0.5, y+ (sqrt(3)*0.5*size));
        gw.drawLine(x+size, y, x+size*0.5, y+ (sqrt(3)*0.5*size));
    } else {
        drawSierpinskiTriangle(gw, x, y, size*0.5, order-1);
        drawSierpinskiTriangle(gw, x+size*0.5, y, size*0.5, order-1);
        drawSierpinskiTriangle(gw, x+size*0.25, y+ (sqrt(3)*0.25*size), size*0.5, order-1);
    }

}

/*
 * This method fills a region of the screen of a certain color to be a different color. It is passed
 * the (x,y) coordinate of the pixel that was clicked, the width and hight of the console, and the
 * color that will be used to fill the screen. First, it gets the color of the pixel that was selected
 * and checks to see if the pixel is already the fill color. If it is not, the method calls the recursive
 * helper class, which accepts the same parameters as well as the color that is being replaced.
 *
 */
void floodFill(int x, int y, int width, int height, int color) {
    int fillColor = getPixelColor(x,y);
    if( fillColor != color) {
        floodFillHelper(x, y, width, height, color, fillColor);
    }
}

/*
 * This method recursively fills in all the touching pixels of a certain color with another specified
 * color. First, it checks if the pixel it is examining is of the color the user wants to replace and
 * if it exists in the console. If these are true, the pixel is set to the desired color and the method
 * is called on the pixels immediately above, below, to the left, and to the right of the pixel just
 * changed.
 */
void floodFillHelper(int x, int y, int width, int height, int color, int fillColor) {
    if(getPixelColor(x,y) == fillColor && pixelInBounds(x , y, width, height)) {
        setPixelColor(x,y,color);
        floodFillHelper(x+1, y, width, height, color, fillColor);
        floodFillHelper(x-1, y, width, height, color, fillColor);
        floodFillHelper(x, y-1, width, height, color, fillColor);
        floodFillHelper(x, y+1, width, height, color, fillColor);
    }
}


/*
 * This method checks if the (x,y) coordinates of a pixel are inside the console window.
 */
bool pixelInBounds(int x, int y, int width, int height) {
    if( x >= 0 && x < width && y >=0 && y < height) {
        return true;
    } else {
        return false;
    }
}

/*
 * This method reads in a file that contains the rules for a formal language and generates a set of random
 * grammars based on the rules. First, this method creates a vector to store the generated grammars and
 * a Map to store the non-terminals and their possible rules. The method passes the map and the istream
 * (input as a parameter) to a method that fills the map with the grammar rules. Then the method passes
 * the symbol that must be generated and the map to a recursive method that returns a randomly generated
 * grammar of the correct symbol. The method adds this grammar to the vector of grammars. It repeats this
 * the number of times specified by the variable times. The method then returns the vector.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    Map<string, Vector<string> > grammarMap;
    generateGrammarMap(input, grammarMap);
    for(int i = 0; i < times; i++) {
        v.add(trim(getNextGrammar(symbol, grammarMap)));
    }
    return v;
}

/*
 * This method recieves an istream reading a file and a map and fills the map, storing the non-terminals
 * as keys and all their possible rules in a vector as the value. Each line of the file contains one key
 * and value that must be stored. The method breaks these lines up with a series of stringSplit calls. If
 * there are multiple definitions of a single non-terminal (more than one line) the method throws an
 * exception error.
 */
void generateGrammarMap(istream& input, Map<string, Vector<string> >& grammarMap) {
    while(!input.eof()){
        string currentLine;
        getline(input, currentLine);
        Vector<string> fullLine = stringSplit(currentLine, "::=");
        if(grammarMap.containsKey(fullLine[0])) {
            throw "Invalid File: Multiple Definitions for Single Non-Terminal";
        } else {
            Vector<string> splitRules = stringSplit(trim(fullLine[1]), "|");
            grammarMap[fullLine[0]] =splitRules;

        }
    }
}

/*
 * This method recursively generates a new random grammar based on the symbol and the map passed to
 * the method. First, if the symbol passed to the method is not a key in the map, then it is a terminal
 * and the method returns it with a space in front of it (with a space for formatting purposes). If this
 * is not true, then the method chooses a random rule from the vector of rules stored as the key's value,
 * stores it as a vector, then recursively calls the method on each of the elements inside of the chosen
 * rule. At the end of this recursion, the method returns the full grammar.
 */
string getNextGrammar(string symbol, Map<string, Vector<string> >& grammarMap){
    if(!grammarMap.containsKey(symbol)){
        return " " + symbol;
    } else {
        Vector<string> possiblePaths = grammarMap[symbol];
        Vector<string> nextPathRules = stringSplit(possiblePaths[randomInteger(0,possiblePaths.size()-1)], " ");
        string finalString = "";
        for(int i = 0; i < nextPathRules.size(); i++) {
            finalString += getNextGrammar(nextPathRules[i], grammarMap);
        }
        return finalString;
    }
}
