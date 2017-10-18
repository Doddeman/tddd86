// This is the .h file you will edit and turn in.


#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <lexicon.h>
#include "grid.h"
#include "vector"

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    // TODO: decide the public member functions and declare them
    Lexicon dictionary;
    vector<string> foundWords;
    vector<string> computerWords;
    int humanScore;
    int computerScore;
    bool validWord;

    void randomBoard();
    void customBoard();
    void humanTurn();
    bool alreadyGuessed(string word);
    bool humanGuess(string word);
    void computerTurn();
    void goComputer(string prefix, int y, int x, Grid<bool> visited);
    void reset();
    void createBoard(string chosenLetters, bool random);
    void printBoard();
    void findHumanWord(string word, string prefix, int y, int x, Grid<bool> visited);

private:
    // TODO: decide the private member variables/functions and declare them
    Grid<char> b_board;
};

#endif
