/*
 * CS 106B Recursion Problems
 * This file declares necessary function prototypes, partly so that other files
 * are able to call the recursive functions you will write.
 * You don't need to modify this file.
 * (If you want to declare your own helper function prototypes,
 *  do that near the top of your .cpp file, not here.)
 */

#ifndef _recursionproblems_h
#define _recursionproblems_h

#include <iostream>
#include "gwindow.h"
#include "vector.h"

// colors for flood fill
#define COLOR_BLACK      0x000000
#define COLOR_DARK_GRAY  0x595959
#define COLOR_GRAY       0x999999
#define COLOR_LIGHT_GRAY 0xBFBFBF
#define COLOR_WHITE      0xFFFFFF
#define COLOR_RED        0xFF0000
#define COLOR_YELLOW     0xFFFF00
#define COLOR_GREEN      0x00FF00
#define COLOR_CYAN       0x00FFFF
#define COLOR_BLUE       0x0000FF
#define COLOR_MAGENTA    0xFF00FF
#define COLOR_ORANGE     0xFFC800
#define COLOR_PINK       0xFFAFAF

// function prototype declarations
// (If you want to declare your own helper function prototypes, do that near
//  the top of your .cpp file, not here.  You shouldn't modify this file.)
int countKarelPaths(int street, int avenue);
int convertStringToInteger(std::string exp);
/*
 * The recursive function convertStringToInteger. This function
 * takes in an integer and if the string is empty, it returns 0;
 * if the string is not empty, the program converts the last
 * character in the string to a number, because that will be the
 * number in the "ones" place. It then removes that character and
 * returns that number plus the function times 10. This continues
 * until the string is zero. If there is a negative sign in the
 * beginning, it removes it and makes the number negative.
 */
bool isBalanced(std::string exp);
/*
 * This function checks to see if parentheses are balanced. It takes
 * the designated string of parentheses and removes the innermost balanced
 * parentheses, then passes the string again until it either reaches zero or
 * it can't remove anything more.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order);
/*
 * This function draws a triangle of a certain order. If the order is zero,
 * the function returns zero; if the order is one, it draws the triangle
 * at the designated spot; otherwise, it divides according to math rules and
 * draws three triangles, calling the function again.
 */
void floodFill(int x, int y, int width, int height, int color);
/*
 * FloodFill takes in a pixel and saves its initial color; it then changes
 * the color of that pixel, then checks the surrounding four pixels to see if
 * they are the same initial color as the original. If they are, then it calls
 * FloodFill on that pixel as well.
 */
Vector<std::string> grammarGenerate(istream& input, string symbol, int times);
/*
 * GrammarGenerate first creates a map based on the input file,and then
 * calls recursiveMaker for "times" number of times, sending the map and
 * the symbol, to make a string that will be added to the vector. Once
 * this process is done, it will return the vector of strings.
 */

// helpers for floodFill
int getPixelColor(int x, int y);
void setPixelColor(int x, int y, int color);

// testing function prototype declarations
void test_karelGoesHome();
void test_convertStringToInteger();
void test_balancingParentheses();
void test_sierpinskiTriangle();
void test_floodFill();
void test_grammarSolver();

#endif
