// Header file containing all game logic for boggle

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include <vector>

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    string randomBoard() const;
    string customBoard() const;
    void createBoard(const string& chosenLetters, const bool& random);
    void printBoard() const;
    void humanTurn();
    bool humanGuess(const string& word);
    void findHumanWord(const string& word, string prefix, const int& y, const int& x, Grid<bool> visited);
    bool alreadyGuessed(const string& word) const;
    void computerTurn();
    void goComputer(string prefix, const int& y, const int& x, Grid<bool> visited);
    void reset();

private:
    Grid<char> b_board;
    Lexicon dictionary;
    vector<string> foundWords;
    vector<string> computerWords;
    int humanScore;
    int computerScore;
    bool validWord;
};

#endif
