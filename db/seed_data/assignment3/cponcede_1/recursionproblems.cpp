/* Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * Program: Recursion Problems
 * -----------------------------
 * This program writes the implementation for a number of functions
 * that utilize recursion. These functions are used in the recursionmain.cpp
 * file.
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

//Function prototypes
int convertStringToInteger(string exp, int value, double counter);
void floodFill(int x, int y, int width, int height, int color, int initialColor);
string generateLine (string symbol, Map <string, Vector < Vector <string> > >, string line);
void generateMap (    Map < string,Vector< Vector <string> > > &map, istream& input);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/* Converts a string that the user enters to an integer data type. It uses a helper
 * function of the same name to perform this task.
 */
int convertStringToInteger(string exp) {
    return convertStringToInteger(exp,0,0.0);
}

/* The helper function for the convertStringToInteger function above. It has
 * an additional parameter that is the total value of the number so far and a
 * counter that is used to make some of the arithmetic easier for the power function.
 * The function starts at the far right character of the string and recursively moves
 * left with each call to itself. When the length of the string is zero, it returns
 * the value that it calculated.
 */
int convertStringToInteger(string exp, int value, double counter) {
    if (exp.length() == 0) {
        return value;
    }
    char ch = exp[exp.length() - 1];
    if (ch == '-') {
        return -1*value;
    } else {
        exp.erase(exp.length() - 1);
        value += (ch-'0')*pow(10.0,counter);
        return convertStringToInteger(exp,value,counter+1);
    }
}

/* Determines if a string that includes parentheses, brackets, and braces
 * is balanced. It does this by removing pairs that are next to each other.
 * If the function is able to eventually remove everything from the string
 * with this method, then the string is balanced.
 */
bool isBalanced(string exp) {
    if (exp.length() == 0) {
        return true;
    }
    int index;
    if (exp.find("()") != -1) {
        index = exp.find("()");
    } else if (exp.find("[]") != -1) {
        index = exp.find("[]");
    } else if (exp.find("{}") != -1) {
        index = exp.find("{}");
    } else {
        return false;
    }
    exp.erase(index,2);
    return isBalanced(exp);

}

/* Draws a Sierpinksi Triangle of a certain order by recursively drawing the lines for each
 * order, changing the size of the triangles each time.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order==0) {
        return;
    }
    int height = size*sqrt(3)/2;
    gw.drawLine(x,y,x+size,y);
    gw.drawLine(x,y,x+size/2,y+height);
    gw.drawLine(x+size/2,y+height,x+size,y);
    drawSierpinskiTriangle(gw,x,y,size/2,order-1);
    drawSierpinskiTriangle(gw,x+size/2,y,size/2,order-1);
    drawSierpinskiTriangle(gw,x+size/4,y+height/2,size/2,order-1);
}

/* Creates a fill functionality using the floodFill helper function defined below */
void floodFill(int x, int y, int width, int height, int color) {
    int initialColor = getPixelColor(x,y);
    floodFill(x,y,width,height,color,initialColor);
}

/* The helper function to floodFill that also includes an integer parameter that
 * is the initial color that the user first clicked. This is necessary so that the
 * program knows to stop if the color at the pixel it is looking at is not the same
 * as the initial color. If it is the same color, it sets the pixel to the new color
 * and then explors above, below, to the left, and to the right of that pixel.
 */
void floodFill(int x, int y, int width, int height, int color, int initialColor) {

    if (x < 0 || x > width-1 || y < 0 || y > height-1) {
        return;
    }
    if (getPixelColor(x,y) != initialColor) {
        return;
    }
    setPixelColor(x,y,color);
    floodFill(x,y-1,width,height,color,initialColor);
    floodFill(x,y+1,width,height,color,initialColor);
    floodFill(x+1,y,width,height,color,initialColor);
    floodFill(x-1,y,width,height,color,initialColor);

}

/* Looks at a the input from a textfile that outlines the grammar of a language
 * and then inputs that information into a map. The function then creates a randomly generated
 * expression that is of the type symbol for the amount of times that the user inputs. If
 * the symbol is terminal, it simply returns a vector of that string. If the symbol is non-terminal,
 * it returns a vector of randomly generated strings that stem from that non-terminal symbol.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "") {
        error("The user entered an improper symbol.");
    }
    Map < string,Vector< Vector <string> > > map;
    generateMap(map, input);
    Vector<string> v;
    for (int i = 0 ; i < times ; i++) {
        if (!map.containsKey(symbol)) {
            v.add(symbol);
        } else {
            string line = "";
            line = generateLine(symbol,map,line);
            v.add(line);
        }
    }
    return v;
}

/* Looks at the text file to create a map to be used to randomly generate
 * gramatically correct strings.
 */
void generateMap (    Map < string,Vector< Vector <string> > >& map, istream& input) {
    string str;
    while (getline(input,str)) {
        int index = str.find("::=");
        string key = str.substr(0,index);
        if (map.containsKey(key)) {
            error("There was an error with the input file's format.");
        }
        str.erase(0,index+3);
        Vector< Vector <string> > values;
        while (str.find("|") != -1) {
            int breakIndex = str.find("|");
            string possibleOption = str.substr(0,breakIndex);
            Vector<string> words = stringSplit(possibleOption, " ");
            values.add(words);
            str.erase(0,breakIndex+1);
        }
        Vector<string> words = stringSplit(str, " ");
        values.add(words);
        map.put(key,values);
    }
}

/* Uses the map that was created previously to randomly generate a line that is
 * valid according to the grammmar rules outlined in the textfile that the user
 * entered.
 */
string generateLine (string symbol, Map <string, Vector < Vector <string> > > map, string line) {
    Vector< Vector <string> > values = map.get(symbol);
    int random = randomInteger(0,values.size()-1);
    Vector<string> chosenValue = values.get(random);
    for (int i = 0 ; i < chosenValue.size() ; i++) {
        string s = chosenValue.get(i);
        if (!map.containsKey(s)) {
            line = line + s + " ";
            if (chosenValue.size() == 1) {
                return line;
            }
        } else {
            line = generateLine(s,map,line);
        }
    }
    return line;
}

