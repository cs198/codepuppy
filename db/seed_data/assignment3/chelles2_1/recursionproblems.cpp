/*Michelle S. Lee
 * CS106B Recursion Problems
 * February 5, 2014
 *
 * This program runs four recursive functions: 1) converting strings to integers,
 * 2) checking if parentheses, brackets, and curly braces are nested,
 * 3) drawing Sierpinski triangles,
 * 4) "flood fill" on graphical window, and
 * 5) random grammar generator.
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

int countKarelPaths(int street, int avenue) {
    // Optional. Skip.
    return 0;
}


//This program takes in a string of integer and returns the integer.
int convertStringToInteger(string exp) {

    //if empty string, return 0
    if (exp == "") {
        return 0;
    }

    //if it's a negative number, multiply -1 to the recursion of the number
    if (exp[0] == '-') {
        return -1 * convertStringToInteger(exp.substr(1, exp.length() - 1));
    }

    //base case
    if (exp.length() == 1) {
        int integer = exp[0] - '0';
        return integer;
    }

    //recursion on the last digit
    int lastDigit = convertStringToInteger(exp.substr(exp.length() - 1, 1));
    //recursion on the all other digits
    int otherDigits = convertStringToInteger(exp.substr(0, exp.length() - 1));

    //add back in the last digit and return result
    return otherDigits * 10 + lastDigit;
}

//This program checks to see if the parentheses, curly braces, and brackets are nested.
bool isBalanced(std::string exp) {
    //base case: empty string
    if (exp == "") {
        return true;
    }

    //Look for parentheses, if not found, look for curly braces; if not found, look for brackets
    size_t index = exp.find("()");
    if (index == string::npos) {
        index = exp.find("{}");
        if (index == string::npos){
            index = exp.find("[]");
            if(index == string::npos){
                //If none of the above is found, they are not nested. Return false.
                return false;
            }
        }
    }

    //Remove the set of parentheses/braces/brackets found above
    std:: string part1 = exp.substr(0, index);
    std:: string part2 = exp.substr(index+2);
    std:: string newExp = part1 + part2;

    //Recursion with the new string
    return isBalanced(newExp);
}


/*This function draws the Sierpinski Triangle in the given window.
It takes in the position (x,y) of the initial location of the edge of the triangle,
the size (length of one side) of the triangle, and "order" (how many levels of triangles to draw).*/
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    //If order is zero, do nothing.
    if (order == 0) return;

    //Base case. Draw one triangle from point (x,y).
    if (order == 1) {
        double height = sqrt(3)/2*size;
        gw.drawLine(x,y,x+size,y);
        gw.drawLine(x,y,x+size/2,y+height);
        gw.drawLine(x+size/2,y+height,x+size,y);
    }

    //Recursion. Calculate half the size of the previous triangle and draw it on top, right, and left.
    if (order > 1) {
        double halfHeight = (sqrt(3)/2*size)/2;
        double halfSize = size/2;
        order--;
        drawSierpinskiTriangle(gw, x, y, halfSize, order);
        drawSierpinskiTriangle(gw, x + halfSize, y, halfSize, order);
        drawSierpinskiTriangle(gw, x + halfSize/2, y + halfHeight, halfSize, order);
    }

}

/*This is the helper function for floodFill. It takes in the location of the click (x,y),
the width and height of the window, the color to turn the area into, and the color of the initial clicked pixel.*/
void floodFillHelper(int x, int y, int width, int height, int color, int oldColor) {
    //If out of bounds, do nothing
    if (x < 0 || x > width - 1 || y < 0 || y > height - 1) return;

    //If the pixel color is the same as the old color, do nothing
    if (getPixelColor(x, y) != oldColor) {
        return;
    }

    //Change the color at position x,y
    setPixelColor(x, y, color);

    //Recursion for the four neighboring pixels around x,y
    floodFillHelper(x + 1, y, width, height, color, oldColor);
    floodFillHelper(x - 1, y, width, height, color, oldColor);
    floodFillHelper(x, y + 1, width, height, color, oldColor);
    floodFillHelper(x, y - 1, width, height, color, oldColor);
}

//This function fills a marked region with a solid color. It takes in the position of click (x,y),
//heigh and width of the window, and the color that the user wants to use.
void floodFill(int x, int y, int width, int height, int color) {
    //Get the pixel color from where the user clicked
    int oldColor = getPixelColor(x, y);

    //If it's already that color, do nothing
    if (color == oldColor) {
        return;
    }

    //Call helper function and pass on the color of that pixel as "oldColor"
    floodFillHelper(x, y, width, height, color, oldColor);
}


/*This is the helper function for grammarGenerate.*/
Vector<string> grammarHelper(Vector<string> result, string symbol, Map<string, Vector<Vector<string>>> map) {
   //If the symbol is not in the map, add it
    if (!map.containsKey(symbol)) {
        result.add(symbol);
    } else {
        //Store the values from that symbol in vector of vector of strings
        Vector<Vector<string>> results = map.get(symbol);
        //Pick a random value
        int randomnum = randomInteger(0, results.size()-1);
        Vector<string> words = results[randomnum];
        //Loop over the words. Recursion for each randomly picked value
        for (int i = 0; i < words.size(); i++) {
            result = grammarHelper(result, words[i], map);
        }
    }
    return result;
}

/*This function generates random sentences from a grammar, given an input file, a symbol, and number of iterations*/
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    //If symbol is an empty string, throw an exception
    if (symbol == "") throw "Error: symbol is null";
    string line;
    Map<string, Vector<Vector<string>>> map;
    //Read the file line by line
    while (getline(input, line)) {
        //At each "::=", split and store the words in vectors
         if (line.find("::=") != string::npos) {
             Vector<string> v = stringSplit(line, "::=");
             //Split at "|" and store the words in vectors
             Vector<string> v2 = stringSplit(v[1], "|");
             Vector<Vector<string>> values;
             for (int i = 0; i < v2.size(); i++) {
                 //Split at spaces and store each word
                 values.add(stringSplit(v2[i], " "));
             }
             //If the map doesn't have the symbol, throw an exception
             if (map.containsKey(v[0])) {
                throw "Error: Duplicate value found";
             }
             //Store the symbol and values in a map
             map.put(v[0], values);
        }
    }

    Vector<string> sentences;

    //For number of times as specified by user,
    for (int time = 0; time < times; time++){
        Vector<string> result;
        //Get the randomly generated result from the helper function
        result = grammarHelper(result, symbol, map);
        string sentence;
        //Store the results and concatenate them together into a sentence.
        for (int i = 0; i < result.size(); i++) {
            sentence += result[i];
            if(i < result.size() - 1){
                //Add space between strings
                sentence += " ";
            } else {
                //Add a period at the end of the sentence.
                sentence += ".";
            }
        }
        //Store the sentence into a vector of sentences
        sentences.add(sentence);
    }
    //Return all the sentences.
    return sentences;
}


