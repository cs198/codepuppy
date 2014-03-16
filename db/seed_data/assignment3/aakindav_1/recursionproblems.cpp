/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* This program runs a serious of mini tasks which was
* all coded with the use of recursion.
*/

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

void floodFillHelper(int x, int y, int width, int height, int newColor, int oldColor);
string grammarGenerateHelper(HashMap<string, Vector<Vector<string> > >& rules, string& symbol);

//Optional: could not get working
int countKarelPaths(int street, int avenue) {
    int path = 0;
    if (street==1 || avenue == 1) path = 1;
    else {
        countKarelPaths(street-1, avenue);
        countKarelPaths(street, avenue-1);
    }
    return path;
}

/* Function takes in a string of integers and converts
 * them to actual integers also multiplies the integer
 * by -1 if the number had a - in front of it.
 * Parameter: string of integers
 * Assumes the string passed in will be only integers.
 */
int convertStringToInteger(string exp) {
    if (exp.size() == 1)
        return exp[0] - '0';
    else {
        if (exp[0] == '-') {
            return -1 * convertStringToInteger(exp.substr(1));
        }
        int last = exp[exp.size()-1] - '0';
        return convertStringToInteger(exp.substr(0,exp.size()-1))*10 + last;
    }
}

/* Function checks to see if a series of parans
 * brackets, and or curly braces are balanced meaning
 * if they each have an open and close component.
 * Parameter: string of braces
 */
bool isBalanced(string exp) {
    if (exp == "")
        return true;
    else {
        if (exp.find("()") != string::npos) exp.erase(exp.find("()"),2);
        else if (exp.find("[]") != string::npos) exp.erase(exp.find("[]"),2);
        else if (exp.find("{}") != string::npos)exp.erase(exp.find("{}"),2);
        else return false;
        return isBalanced(exp);
    }
}

/* Function creates a Sierpinski Triangle by using the
 * concept of fractals. Recurses through and continues
 * to draw triangles depending on the given order
 * Parameters: The GWindow for the triangle, the x coor,
 * the y coor, the size of a side and the order.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order == 0) {
    } else if (order == 1) {
        gw.drawLine(x,y,x+size,y);
        gw.drawLine(x,y,x+size/2,y+size*(sqrt(3.0)/2.0));
        gw.drawLine(x+size,y,x+size/2,y+size*(sqrt(3.0)/2.0));
    } else {
        drawSierpinskiTriangle(gw,x,y,size/2,order-1);
        drawSierpinskiTriangle(gw,x+size/2,y,size/2,order-1);
        drawSierpinskiTriangle(gw,x+size/4,y+size*((sqrt(3.0)/4.0)),size/2,order-1);
    }

}

/* Function stores the value of the previous color then
 * calls a helper function to complete te project.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x,y);
    floodFillHelper(x,y,width,height,color,oldColor);
}

/* Function sets the current pixel to the new color then
 * recurses in all 4 directions (up,down,right,left) to find
 * any more colors that needs to be changed. Stops changing
 * colors once it hit a pixel that wasn't the old color.
 * Parameters: x coor, y coor, width and height of the pixel.
 * the new color and the old color made in the actual funtion.
 */
void floodFillHelper(int x, int y, int width, int height, int newColor, int oldColor) {
    if (x <0 || x>=width) return;
    if (y<0 || y>=height) return;
    if (oldColor != getPixelColor(x,y)) return;
    setPixelColor(x,y,newColor);
    floodFillHelper(x+1,y,width,height,newColor,oldColor);
    floodFillHelper(x-1,y,width,height,newColor,oldColor);
    floodFillHelper(x,y+1,width,height,newColor,oldColor);
    floodFillHelper(x,y-1,width,height,newColor,oldColor);
}

/* Function reads in a grammar file then breaks it up into a
 * map that contains a nonterminal key to all its paths. Then
 * iterates through the helper function to make string solution
 * adds the solution to a vector then returns that vector.
 * Throws an exception if a key appears that is already in the map.
 * Parameters: ifstream input for the file, the symbol of the
 * nonterminal and the amount of times the user wants the program
 * to generate random sentences.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap <string, Vector< Vector <string> > > rules;
    string line;
    Vector<string> v;
    while (getline(input,line)) {
        //1. break string into keys and values
        Vector<string> keyVal = stringSplit(line, "::=");
        string key = keyVal[0];
        if (rules.containsKey(key))
            throw "Grammar contains more than one declaration for a single non terminal";
        string values = keyVal[1];
        //2. break values into a vector of value strings
        Vector<string> valVec = stringSplit(values, "|");
        Vector<Vector<string> > actValVec;
        //3. break value strings into vector values which are also strings
        for (string value:valVec) {
            Vector<string> noWhiteSpaceVal = stringSplit(value, " ");
            actValVec.add(noWhiteSpaceVal);
        }
        rules[key] = actValVec;
    }
    for (int i=0; i<times; i++) {
        string solution = grammarGenerateHelper(rules, symbol);
        v.add(solution);
    }
    return v;
}

/* Function checks to see if the symbol is a terminal and
 * returns the string if so else the function gets the values
 * of the string a randomly selects a vector from that value
 * then loops through every terminal in the vector and places
 * them in the recursive step when the helper function is called
 * again then returns the string and adds it to the result with a
 * space only if there isn't a space already there.
 * Parameters: hashmap containing all the nonterminal value pairs
 * and the string which represents the symbol.
 */
string grammarGenerateHelper(HashMap<string, Vector<Vector<string> > >& rules, string& symbol) {
    string result = "";
    if (!rules.containsKey(symbol)) {
        result = symbol;
        return result;
    } else {
        //if symbol is terminal print then return
        //every other case (recursion)
        //use rgen to pick random v from vv
        //that each string from that vector and replace it with symbol
        Vector<Vector<string> > valRules = rules[symbol];
        int ruleIndex = randomInteger(0,valRules.size()-1);
        for (string rule:valRules[ruleIndex]) {
            result += grammarGenerateHelper(rules, rule);
        }
        if (result[result.size()-1] != ' ') result += " ";
        return result;
    }
}
