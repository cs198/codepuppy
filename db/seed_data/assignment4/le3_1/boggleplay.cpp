// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"



void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4,4);
    string boardLetters;
   if(getYesOrNo("Do you want to generate a random board? ")){
       boardLetters = "";
   }else{
       boardLetters = getLine("Type the 16 letters to appear on the board:");
       while(boardLetters.size() != 16){
               cout << "That is not a valid 16-letter board string. Try again" << endl;
               boardLetters = getLine("Type the 16 letters to appear on the board:");
           }
   }
   Boggle b(dictionary, boardLetters);
   BoggleGUI::labelAllCubes(toUpperCase(b.getString()));
   //first time set up
   clearConsole();
   cout << "It's your turn" << endl;
   cout << b << endl;
   cout << "Your words " << "(0): {} " << endl;
   cout << "Your score: 0" << endl;
   string humanWord  = toUpperCase(getLine("Type a word (or Enter to stop): "));

   // after first time
   //check the word
   while(humanWord != ""){
    Boggle b(dictionary, toUpperCase(boardLetters));
    BoggleGUI::labelAllCubes(b.getString());
   clearConsole();
   // check the word meets "checkWord" & "humanWordSearch"
   if(!b.checkWord(humanWord)){
       cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
   }else{
       if(!b.humanWordSearch(humanWord)){
           cout << "That word can't be formed on this board." << endl;
       }else{
           cout << "You found a new word! " << humanWord << endl;
        }
   }
       // display the grid
       cout << b << endl;
       // display your score ....
       cout << "Your words (" << b.getHumanWordsNum() << "): " << b.getHumanWords() << endl;
       cout << "Your score:" << b.humanScore() << endl;
       humanWord  = toUpperCase(getLine("Type a word (or Enter to stop): "));
    }

   //do computer word
   cout << "It's my turn!" << endl;
   Set<string> computerWordSet = b.computerWordSearch();
   cout << "My words (" << b.getComputerWordsNum() << "): " << computerWordSet << endl;
   cout << "My score: " << b.computerScore() << endl;
   if(b.computerScore() > b.humanScore()){
   cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
   }else{
   cout << "WOW, you defeated me! Congratulations!" << endl;
   }


}

