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

//prototypes
void drawSierpinskiTriangleHelper(GWindow& gw, double x, double y, double size, int order);
void floodFill(int x, int y, int width, int height, int color, int clickedColor);
bool inBounds (int x, int y, int width, int height);
void readFile(istream& input, Map<string, Vector<string> > & grammar);
void generate(string symbol, Vector<string> & v,Map<string, Vector<string> > & grammar, string & oneSentence);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

//Checks whether the string has '-' in the front. if so, it coverts the string to the integer by adding  negative
//before the units and also the rest of the string's recursive call results will be multipied by (-10). If it's
//positive, the units are transformed by minus '0', and recursive call to the rest which are multipled by ten.
//the final number is the sum of all the units, tens, hundreds ...
int convertStringToInteger(string exp) {
    if(exp[0] == '-') {
        exp.erase(0,1);
        if(exp.size() ==1) {
            return '0' - exp[0] ;
        } else {
            return ('0' - exp[exp.length()-1]) + (-10) * convertStringToInteger(exp.substr(0, exp.length()-1));
        }
    } else {
        if(exp.size() ==1) {
            return exp[0] - '0';
        } else {
            return (exp[exp.length()-1]- '0') + 10 * convertStringToInteger(exp.substr(0, exp.length()-1));
        }
    }
    return 0;
}

// If the string is empty, return true. Otherwise it goes into recursive part where paired-up patterns were looked up
//and delelted and pass the left-over to the next order calls.
bool isBalanced(string exp) {
    if(exp == "") {
        return true;
    } else {
        if(exp.find("()") != string::npos) exp = exp.erase(exp.find("()"),2);
        if(exp.find("[]") != string::npos) exp = exp.erase(exp.find("[]"),2);
        if(exp.find("{}") != string::npos) exp = exp.erase(exp.find("{}"),2);
        if(exp.find("()") != string::npos || exp.find("[]") != string::npos||exp.find("{}") != string::npos || exp =="") {
            return isBalanced(exp);
        }else {
            return false;
        }
    }
    return false;
}
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order>1) {
        //draw three lines to form a triangle
        gw.drawLine(x,y,x+size,y);
        gw.drawLine(x+size,y, x+size/2, y+size*sqrt(3)/2);
        gw.drawLine(x,y,x+size/2, y+size*sqrt(3)/2);
        //recursively call itself three times to generate three triangles of the lower order
        drawSierpinskiTriangle(gw, x,y,size/2,order-1);
        drawSierpinskiTriangle(gw, x+size/2,y,size/2,order-1);
        drawSierpinskiTriangle(gw, x+size/4,y+size*sqrt(3)/4,size/2,order-1);
    }
}

//get the color of the orignal click and pass it on as a parameter for the
//same function.
void floodFill(int x, int y, int width, int height, int color) {
    int clickedColor = getPixelColor(x,y);
    floodFill(x,y, width, height, color, clickedColor);
}

// The helper function with one extra parameter about the clicked color
void floodFill(int x, int y, int width, int height, int color, int clickedColor) {
    if(!inBounds(x,y,width,height)) return;
    //Get the current color that it's exploring
    int currentColor = getPixelColor(x,y);

    //If current pixel explored is in the same color as the clicked one,
    //it recursively call to exspand color-changing and exploring
    if(currentColor == clickedColor) {
    setPixelColor(x,y, color);
    //explore four directions recursively
    floodFill(x+1,y, width, height, color, clickedColor);
    floodFill(x-1,y, width, height, color,clickedColor);
    floodFill(x,y+1, width, height, color, clickedColor);
    floodFill(x,y-1, width, height, color, clickedColor);
    }
}

//Helper function for floodFill to check bounds
bool inBounds (int x, int y, int width, int height) {
    return x>=0 && x < width && y>=0 && y < height;
}


Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    Map<string, Vector<string> > grammar;
    readFile(input,grammar);
    //For each sentence generated the given times, it added to the vector to be returned.
    for(int i=0; i<times;i++) {
        string oneSentence;
        generate(symbol, v, grammar, oneSentence);
        v += oneSentence;
    }
    return v;
}

//Read the file given from the input stream line by line. Each line is cut into non-terminal and terminals.
//terminals are splitted and stored into a vector. Each line's non-terminals and terminals are paired up and stored
//into a map.
void readFile(istream & input, Map<string, Vector<string> > & grammar) {
    string line;
    while(!input.fail()) {
        getline(input, line);
        string nonterminal = trim(line.substr(0,line.find(":")));
        string rest = trim(line.substr(line.find("=")+1));
        Vector<string> terminals = stringSplit(rest,"|");
        grammar[nonterminal] = terminals;
    }
}

void generate(string symbol, Vector<string> & v,Map<string, Vector<string> > & grammar,  string & oneSentence) {
    if(!grammar.containsKey(symbol)) {
        //if it is not a non-terminal, it is added to the sentence.
         oneSentence += symbol +  " ";
    } else {
        Vector<string> terminals = grammar[symbol];
        //Randomly selects one possible outcome from the terminals
        string selected = terminals[randomInteger(0,terminals.size()-1)];
        //split the selected outcome into elements that are separated by space, and put them into a vector
        Vector<string> elements = stringSplit(selected, " ");
        //for each element in the selected terminal, it recursively calls itself to determine next level expansion.
        for(string element: elements){
            generate(element, v, grammar, oneSentence);
        }

    }

}


