// This program contains the code for several applications whose problems are solved recursively.

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
#include "math.h"
#include "cmath"
#include <cmath>
using namespace std;


//This constant represents the constant PI
const double PI = 3.14159;
//This constant represents the height of an equilaterial triangle in relation to its side length
const double heightRatio = sin(60*PI/180.0);

void convert(int& returnValue, int& counter, string& exp);
bool checkValid(char ch);
string checkString(char ch);
bool checkBalance(string& exp, int index);
void drawSubdivisions(GWindow& gw, int x, int y, int size, int order);
bool pixelInBounds(int x, int y, int width, int height);
void setFloodColor(int x, int y, int width, int height, int color, int pixelColor);
void changeColor(int x, int y, int width, int height, int color, int pixelColor);
void generateSentence(Map<string, string> map, string symbol, string& sentence);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

//This method takes a string representing an integer in as a parameter, and returns the corresponding integer value
//This method acts as a "wrapper function" for the "convert" function below, passing in a counter to keep track of the digit place location and a return value
int convertStringToInteger(string exp) {
    int counter=0;
    int returnValue=0;
    convert(returnValue, counter, exp);
    return returnValue;
}

//This method recursively solves the problem presented in the convertStringToInteger method
//It takes in the aforementioned method's return value in as a parameter, as well as the current string representing the integer
//To keep track of its place in the integer modification process (whether it is at the ones, tens, hundreds digit, etc.), a counter is passed in as well
void convert(int& returnValue, int& counter, string& exp) {
    if(exp!=""){
        char ch=exp[exp.length()-1];
        if(ch=='-') {
            returnValue=returnValue*(-1);
            exp.erase(exp.length()-1, 1);
        } else {
            int next=ch-'0';
            double exp1=counter;
            int exponent=pow(10.0, exp1);
            next=next*exponent;
            returnValue+=next;
            counter++;
            exp.erase(exp.length()-1, 1);
            convert(returnValue, counter, exp);
        }

    }
}

//This method checks to see if a string passed in of parentheses, curly braces, or square brackets is "balanced"
//This method acts as  "wrapper function" for the checkBalance function below, passing in an index value that the below function can modify
bool isBalanced(string exp) {
    int index=0;
    return checkBalance(exp, index);
}

//This method recursively solves the problem presented by the isBalanced method
//It takes in a string, as well as an index to keep track of the location in the string, and returns true if the string is "balanced"
bool checkBalance(string& exp, int index) {
    if(exp=="") {
        return true;
    }
    if(index==exp.length()-1) {
        return false;
    }
    char ch=exp[index];
    if(!checkValid(ch)) {
        return false;
    }
    string check=checkString(ch);
    if(exp.find(check)!=string::npos) {
        exp.erase(exp.find(check), 2);
        index=0;
        if(checkBalance(exp, index)){
            return true;
        }
    } else {
        index++;
        if(checkBalance(exp, index)) {
            return true;
        }
    }
    return false;
}

//This method checks to see if a valid character has been entered into the checkBalance method
bool checkValid(char ch) {
    if(ch!='{' && ch!='(' && ch!='[') {
        return false;
    }
    return true;
}

//This method returns a string representing a pair of parentheses, curly braces, or square brackets to be searched for in the string to determine balance
string checkString(char ch) {
    if(ch=='(') {
        return "()";
    }
    if(ch=='[') {
        return "[]";
    }
    if(ch=='{') {
        return "{}";
    }
}

//This method draws a Sierpinki Triangle. If the order called is greater than zero, it draws a base triangle, then begins the recursive calls to draw the remaining subdivisions
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order>0){
        order--;
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x+size/2.0, y+size*heightRatio);
        gw.drawLine(x+size, y, x+size/2.0, y+size*heightRatio);
        drawSubdivisions(gw, x, y, size, order);
    }

}

//This method recursively draws any subdivisions to a Sierpinski triangle
void drawSubdivisions(GWindow& gw, int x, int y, int size, int order) {
    if(order>0) {
        order--;
        size=size/2.0;
        int x1=x+size;
        int x2=x+size*0.5;
        int x3=x+size*1.5;
        int y2=y+size*heightRatio;
        gw.drawLine(x1, y, x2, y2);
        gw.drawLine(x1, y, x3, y2);
        gw.drawLine(x2, y2, x3, y2);
        drawSubdivisions(gw, x, y, size, order);
        drawSubdivisions(gw, x1, y, size, order);
        drawSubdivisions(gw, x2, y2, size, order);
    }
}


//This method creates the "flood fill" effect by causing all pixels of the same color that are touching to be converted to another chosen color
//It acts as a "wrapper function" to the setFloodColor function below, which recursively solves the problem
//It obtains the color and location of the pixel clicked on, and passes that information in to the below function
void floodFill(int x, int y, int width, int height, int color) {
    int pixelColor=getPixelColor(x, y);
    setPixelColor(x, y, color);
    setFloodColor(x, y, width, height, color, pixelColor);
}

//This method recursively solves the problem presented in the floodFill method
//It determines the x and y coordinates of all of the surrounding pixels, and passes them into the decomposed method changeColor to either alter the color of the surrounding pixels or determine if that does not need to be done
void setFloodColor(int x, int y, int width, int height, int color, int pixelColor) {
    int x1=x+1;
    int x2=x-1;
    int y1=y+1;
    int y2=y-1;
    changeColor(x, y1, width, height, color, pixelColor);
    changeColor(x, y2, width, height, color, pixelColor);
    changeColor(x1, y, width, height, color, pixelColor);
    changeColor(x2, y, width, height, color, pixelColor);
}

//This method will, if the pixel is in bounds and of an appropriate color, change the color of the chosen pixel to match the color selected by the user
void changeColor(int x, int y, int width, int height, int color, int pixelColor) {
    if(pixelInBounds(x, y, width, height)) {
        if(getPixelColor(x, y)==pixelColor) {
            setPixelColor(x, y, color);
            setFloodColor(x, y, width, height, color, pixelColor);
        }
    }
}

//This method checks to see if the chosen pixel is in bounds
bool pixelInBounds(int x, int y, int width, int height) {
    if(x>width-1 || x<0) {
        return false;
    }
    if(y>height-1 || y<0) {
        return false;
    }
    return true;
}

//This method returns a vector of strings that represent gramatically correct phrases based on a grammar provided by an input text file
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    Vector<string> vector;
    Map<string, string> map;
    string line;
    while(getline(input, line)) {
        vector=stringSplit(line, "::=");
        map.put(vector[0], vector[1]);
    }
    if(map.containsKey(symbol)==false) {
        for(int i=0; i<times; i++) {
            v.add(symbol);
        }
        return v;
    }
    for(int i=0; i<times; i++) {
        string sentence;
        generateSentence(map, symbol, sentence);
        v.add(sentence);
    }

    return v;
}

//This method recursively generates a gramatically correct phrase, relying on a map of a grammar passed in
void generateSentence(Map<string, string> map, string symbol, string& sentence) {
    string value=map.get(symbol);
    Vector<string> rules=stringSplit(value, "|");
    int random=randomInteger(0, rules.size()-1);
    string rule=rules[random];
    Vector<string> components=stringSplit(rule, " ");
    for(int i=0; i<components.size(); i++) {
        string component=components[i];
        if(map.containsKey(component)) {
            generateSentence(map, component, sentence);
        } else {
            sentence=sentence+component+" ";
        }
    }

}
