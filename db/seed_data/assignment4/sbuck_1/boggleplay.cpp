// This is the cpp file that plays one game of boggle.
// It is designed to do little work other than to call the
// functions of the boggle class.

#include "lexicon.h"
#include "Boggle.h"
#include "console.h"
#include "simpio.h"
#include "bogglegui.h"


void playOneGame(Lexicon& dictionary) {

    string line;
    Boggle game(dictionary, line);
    cout << game << endl;
    cout << "It's your turn!" << endl;
    while(true){
        line = getLine("Type a word (or Enter to stop):");
        if(line == "")break;
        line = toUpperCase(line);
        if(game.checkWord(line, dictionary)){
            if(game.humanWordSearch(line) == true){
                cout << "You found a new word! " << "\""<< line << "\""<<  endl;
                cout << "Your words (" << game.foundByPlayer.size() << "): "
                     << game.foundByPlayer.toString() << endl;
                cout << "Your score: "<<  game.humanScore() << endl;
                cout << endl;
            }
        }
    }
    cout << endl;
    cout << "It's my turn!" << endl;
    Set<string> compSet = game.computerWordSearch(dictionary);
    cout << "My words (" << compSet.size() << "): " << compSet.toString() << endl;
    cout << "My score: " << game.getScoreComputer(compSet) << endl;
    cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
}
