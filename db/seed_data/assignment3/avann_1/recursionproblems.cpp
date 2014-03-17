/* This is CS106B Assignment 3. In this assignment, I have completed the
 * functions provided (convertStringToInteger, isBalanced, drawSierpinskiTriangle,
 * floodFill, and grammarGenerate) using recursion.
 * I received help from section leaders at the Lair.
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "filelib.h"
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
#include "gmath.h"
#include "hashset.h"
using namespace std;

bool checkBounds(int x, int y, int width, int height);
void floodFillHelper(int x, int y, int width, int height, int color, int originalcolor);
void readFile(istream& input, HashMap<string, Vector< Vector<string> > >& rules);
string grammarGeneratorHelper(string symbol, HashMap<string, Vector< Vector<string> > >& rules);

int countKarelPaths(int street, int avenue){
    return 0;
}


/* The function convertStringToInteger takes in a string of integers from
 * the user and returns this string of integers as an integer. The base case
 * for this function is when the user provides a string of integers of length
 * 1 (containing only 1 integer), in which case the program converts this to
 * an integer and returns it.
 * Otherwise, the program first determines if the string of integers is
 * false, removing the minus character. It then creates substrings separating
 * the last integer from the prior integers, and recurses on the prior integers
 * multiplying by 10, so that the final integer will be of the proper value when
 * the last integer is added to it. If it was negative, the final integer is
 * multiplied by -1. The final integer is returned.
 */
int convertStringToInteger(string exp) {
    if(exp.length()==1){
        int value = exp[0] - '0';
        return value;
    }else{
        bool negative = false;
        if(exp[0]=='-'){
            negative = true;
            exp.erase(0,1);
        }
        string firstsub = exp.substr(0,exp.length()-1);
        string secondsub = exp.substr(exp.length()-1);
        int current = convertStringToInteger(firstsub)*10;
        int lastnum = secondsub[0] - '0';
        int final = current + lastnum;
        if(negative) final*=-1;
        return final;
    }
}

/* The isBalanced function takes in a string containing only (, ), {, }, [, ]
 * characters and returns a boolean depending on whether these characters are
 * balanced. The base case occurs when the string is empty "", and the function
 * returns true.
 * Otherwise, the function attempts to find a desired set of paired characters. If
 * it finds these characters, it finds their index, so that this pair of characters
 * may be erased from the original string. The function recurses on this string,
 * returning true if this string eventually becomes an empty string "", or returning
 * false if this string eventually does not contains a paired set of characters.
 */
bool isBalanced(string exp) {
    if(exp==""){
        return true;
    }else{
        int index;
        if(exp.find("()")!=string::npos || exp.find("[]")!=string::npos || exp.find("{}")!=string::npos){
            if(exp.find("()")!=string::npos){
                index = exp.find("()");
            }else if(exp.find("[]")!=string::npos){
                index = exp.find("[]");
            }else{
                index = exp.find("{}");
            }
            exp.erase(index,2);
            return isBalanced(exp);
        }
        return false;
    }
}

/* The drawSierpinskiTriangle function takes in parameters for the starting point
 * of a triangle, and its size and order. Its base case occurs if the order of
 * the triangle is 1, in which case, it must only draw 1 triangle.
 * Otherwise, if the order is greater than 1, the function recurses three times
 * (for each side of the smaller triangles), adjusting the position, size, and order
 * parameters accordingly.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order==1){
        gw.drawLine(x,y,x+size,y);
        gw.drawLine(x,y,x+(size/2),y+(size*sinDegrees(60)));
        gw.drawLine(x+size,y,x+(size/2),y+(size*sinDegrees(60)));
    }
    if(order>1){
        drawSierpinskiTriangle(gw, x, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/2, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/4, y+(size*sinDegrees(60))/2, size/2, order-1);
    }
}

/* The floodFill function allows the user to click on a rectangle of a certain
 * color, and change that rectangles color to the color they select.
 * It first finds the original color of the rectangle, and passes this as
 * a parameter to floodFillHelper, so that the original color will not be changed
 * with each recursive call.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int originalcolor = getPixelColor(x,y);

    floodFillHelper(x, y, width, height, color, originalcolor);
}

/* The floodFillHelper function is responsible for determining whether a pixel
 * is of the original color, and setting that pixel to the desired color if it
 * is of the original color. The base case occurs when a pixel is not of the
 * original color, in which case the program should do nothing to that pixel.
 * If the pixel is of the original color, it changes that pixel's color to
 * the desired color, and checks all of its neighboring pixels. If the neighboring
 * pixel is in the boundary, floodFillHelper is called to change that pixels
 * color (if it's the original color) and continue checking that pixel's neighbors.
 */
void floodFillHelper(int x, int y, int width, int height, int color, int originalcolor){
    if(getPixelColor(x,y)==originalcolor){
        setPixelColor(x,y,color);

        if(checkBounds(x, y+1, width, height)){
            floodFillHelper(x, y+1, width, height, color, originalcolor);
        }

        if(checkBounds(x, y-1, width, height)){
            floodFillHelper(x,y-1, width, height, color, originalcolor);
        }

        if(checkBounds(x+1, y, width, height)){
            floodFillHelper(x+1, y, width, height, color, originalcolor);
        }

        if(checkBounds(x-1, y, width, height)){
           floodFillHelper(x-1, y, width, height, color, originalcolor);
        }
    }
}

/* The checkBounds function is used by floodFillHelper to determine
 * if a pixel is in the bounds of the window.
 */
bool checkBounds(int x, int y, int width, int height){
    if(x>=0 && y>=0 && x<=(width-1) && y<=(height-1)){
            return true;
    }else{
        return false;
    }
}

/* The grammarGenerate function takes in an input file containing
 * grammar rules. It calls readFile to create a HashMap of the files contents.
 * It creates a vector that will be modified by the grammarGeneratorHelper
 * function for a certain number of times, each time the grammarGeneratorHelper
 * outputs a string. It returns this vector of strings.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if(symbol == ""){
        error("Invalid Input");
    }

    HashMap<string, Vector< Vector<string> > > rules;
    readFile(input, rules);

    Vector<string> v;

    if(times <= 0){
        v+="";
        return v;
    }

    for(int i=0; i<times; i++) {
        v.add(grammarGeneratorHelper(symbol, rules));
    }

    return v;
}

/* The grammarGeneratorHelper function takes in the HashMap created by the readFile
 * function and the symbol as its parameters. The base case occurs if the symbol
 * the user input is a not a key in the map, meaning it is not a rule and is terminal.
 * It returns this symbol.
 * Otherwise, the symbol is non-terminal and corresponds to a key/rule in the map. This rule's
 * values are found, and a random value (corresponding to potentially another list of rules
 * is chosen). These become the new parameters for the grammarGenerateHelper, which creates a
 * string output to be returned.
 */
string grammarGeneratorHelper(string symbol, HashMap<string, Vector< Vector<string> > >& rules){
    if(!rules.containsKey(symbol)){
        return symbol;
    }else{
        string output;

        Vector< Vector<string> > ruleslist = rules.get(symbol);

        int randomruleindex = randomInteger(0, ruleslist.size()-1);
        Vector<string> rule = ruleslist[randomruleindex];

        for(string term: rule){
            output += grammarGeneratorHelper(term, rules);
        }

        return output + " ";
    }
}

/* The readFile function takes in the file the user selects and creates a
 * HashMap of strings to vectors of vectors of strings. Thus each rule
 * maps to vectors containing more rules or terminals.
 * First, the function reads the file, splitting its lines by the "::=". It
 * makes everything to left of this a key, ensuring that there are not repeated
 * keys using a HashSet. It then creates the values corresponding to this key
 * by splitting the remaining string by "|" and by " ", creating a vector of
 * vectors of strings. It adds these key, value pairs to the rules map.
 */
void readFile(istream& input, HashMap<string, Vector< Vector<string> > >& rules){
    Vector<string> originalfileLines;

    string line;
    while (true) {
        bool success = getline(input, line);
        if (!success) {
            break;
        }
        originalfileLines.add(line);
    }

    Vector<Vector<string> > fileLines;

    for(string s: originalfileLines){
        Vector<string> temp = stringSplit(s, "::=");
        fileLines.add(temp);
    }

    HashSet<string> checkkeys;

    for(Vector<string> s: fileLines){
        string key = s[0];
        if(checkkeys.contains(key)){
            error("Repeated non-terminal");
        }

        Vector <Vector<string> > listofrules;

        Vector<string> vectorofrules = stringSplit(s[1], "|");

        for(string rule: vectorofrules){
            Vector<string> listnonterm = stringSplit(rule, " ");
            listofrules.add(listnonterm);
        }

        rules.put(key, listofrules);
    }
}
