// recursionproblems.cpp
// Contains the functions: convertStringToInteger, isBalanced, drawSierpinskiTriangle, floodFill, and grammarGenerate.

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "hashmap.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
#include "filelib.h"
using namespace std;

/*Function Prototypes*/
void drawTriangle(GWindow& gw, double x, double y, int size);
void floodFillHelper(int x, int y, int width, int height, int color, int originalColor);
void getMapFromFile (HashMap<string, Vector< Vector<string> > >& grammarMap, istream& input);
void grammarGenerateHelper(string symbol, HashMap<string, Vector < Vector <string> > >& grammarMap, string& temp);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*
 * Converts a string into an integer recursively.
 * Pre-Condition: The string is a valid string containing digits from '0' to '9' or '-' at the beginning.
 *
 * Base Case: When the length of the string is equal to 1, the integer value of that string will be returned.
 *
 * Recursive Case: The function calls itself on the string excluding the last character and adds on the integer value of the last character.
 *
 * Negative Case: If the string contains a dash, then the function calls omits the dash and calls itself
 * on the numbers and returns the negative integer version of that string.
 *
 */
int convertStringToInteger(string exp) {
    if(exp[0] == '-') {
        return -convertStringToInteger(exp.substr(1));
    }
    if(exp.length() == 1) {
        return exp[0] - '0';
    } else {
        int lastChar = exp.substr(exp.length()-1)[0];
        string next = exp.substr(0, exp.length()-1);
        return 10 * convertStringToInteger(next) + lastChar;
    }
}

/*
 * Returns true if a string is balanced.
 * If the string is empty, the function will return true. Otherwise, it will check the three types of brackets (), [] and {}.
 * If the string contains any of those, the first one checked and found will be erased from the string. Then the string will be passed back into the function.
 * At any point if the string does not contain (), [] or {}, then the string is not balanced and the function returns false.
 */
bool isBalanced(string exp) {
    if(exp == "") {
        return true;
    } else if(exp.find("()") != string::npos) {
        return isBalanced(exp.erase(exp.find("()"), 2));
    } else if(exp.find("[]") != string::npos) {
        return isBalanced(exp.erase(exp.find("[]"), 2));
    } else if (exp.find("{}") != string::npos) {
        return isBalanced(exp.erase(exp.find("{}"), 2));
    } else {
        return false;
    }
}

/*
 * Draws the Sierpinski recursively. Base case is when the order is 1, which just draws one large triangle with "size" side length.
 * Recursive case draws three triangles at the edges of the large triangle for every triangular space. There will always be three triangular spaces to fill,
 * which is why the function calls itself three times.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order == 1) {
        drawTriangle(gw, x, y, size);
    } else {
        drawTriangle(gw, x, y, size/2);
        drawTriangle(gw, x + size/2, y, size/2);
        drawTriangle(gw, x + size/4, y + (sqrt(3)*size)/4, size/2);
        drawSierpinskiTriangle(gw, x, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x + size/2, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x + size/4, y +(sqrt(3)*size)/4, size/2, order-1);
    }
}

/*
 * Draws a triangle given the coordinates of the upper left corner and the side length of the triangle.
 */
void drawTriangle(GWindow& gw, double x, double y, int size) {
    gw.drawLine(x, y, x+size, y);
    gw.drawLine(x + size, y, x + (size/2), y + (sqrt(3)*size)/2);
    gw.drawLine(x, y , x + (size/2), y + (sqrt(3)*size)/2 );
}

/*
 * Fills a certain area with the specified color. Passes the original color of the (x, y) unit that the user initially clicks on
 * to the floodFillHelper function.
 */
void floodFill(int x, int y, int width, int height, int color) {
    floodFillHelper(x, y, width, height, color, getPixelColor(x,y));
}

/*
 * Helper function for floodFill that takes in an extra parameter (original color of the coordinate the user clicks on).
 * If the (x, y) unit is in bounds and the color of that (x, y) pixel is the same as the original color, we will change
 * the color of (x, y) to the new color. Then the function recurses to the coordinates to the left, top, bottom and right
 * of (x, y).
 */
void floodFillHelper(int x, int y, int width, int height, int color, int originalColor) {
    if(!(x < 0 || x > width - 1) && !(y < 0 || y > height - 1) && (getPixelColor(x,y) == originalColor)) {
        setPixelColor(x,y,color);
        floodFillHelper(x - 1, y, width, height, color, originalColor);
        floodFillHelper(x + 1, y, width, height, color, originalColor);
        floodFillHelper(x, y + 1, width, height, color, originalColor);
        floodFillHelper(x, y - 1, width, height, color, originalColor);
    }
}

/*
 * Returns a vector where every index is a randomly generated sentence based on the grammar from the input file.
 * The input file will be read into a map. Then grammarGenerateHelper will be called for as many times as the user specified.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap<string, Vector< Vector<string> > > grammarMap;
    Vector <string> result;
    getMapFromFile(grammarMap, input);
    string sentence = "";
    for (int i = 0; i < times; i++) {
        grammarGenerateHelper(symbol, grammarMap, sentence);
        result.add(sentence);
        sentence = "";
    }
    return result;
}

/*
 * Recursively builds a sentence based on the grammarMap and the symbol that the user enters.
 * Takes in a string by reference so that it can be added into the vector of the original grammarGenerate function.
 * If the symbol is a terminal symbol, this means that it will not be a key in the map, so just concatenate the symbol onto
 * the sentence we have so far.
 * Otherwise, get the value associated with the symbol from the grammarMap and randomly choose a vector from the vector of vectors.
 * For every string in that randomly chosen vector, recurse through it.
 */
void grammarGenerateHelper(string symbol, HashMap<string, Vector < Vector <string> > >& grammarMap, string& sentence) {
    if(!grammarMap.containsKey(symbol)) {
        sentence += symbol + " ";
    } else {
        Vector < Vector <string> > allRules = grammarMap[symbol];
        int randomIndex = randomInteger(0, allRules.size() - 1);
        Vector <string> randomVec = allRules[randomIndex];
        for(string next: randomVec) {
            grammarGenerateHelper(next, grammarMap, sentence);
        }
    }
}

/*
 * Reads the input file into a hashmap where the string before "::=" is the key and the strings after "::=" are broken up by "|" and split
 * into individual vectors.
 * If multiple non-terminal keys appear in a file, a string exception is thrown.
 */
void getMapFromFile (HashMap<string, Vector< Vector<string> > >& grammarMap, istream& input) {
    string str;
    while(getline(input, str)) {
        int index = str.find("::=");
        string nTerminal = str.substr(0, index);
        str.erase(0, index+3);
        Vector<string> rules = stringSplit(str, "|");
        Vector < Vector <string> > allRules;
        for(string s: rules) {
            Vector<string> temp = stringSplit(s, " ");
            allRules.add(temp);
        }
        if(grammarMap.containsKey(nTerminal)) {
            throw "Grammar file may not contain more than one line for the same non-terminal";
        } else {
            grammarMap.put(nTerminal, allRules);
        }
    }
}
