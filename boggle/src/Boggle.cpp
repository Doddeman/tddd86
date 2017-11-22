#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include <algorithm>

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = { // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

void Boggle::reset(){
    dictionary.addWordsFromFile(DICTIONARY_FILE);
    foundWords.clear();
    computerWords.clear();
    humanScore = 0;
    computerScore = 0;
}

string Boggle::randomBoard() const{
    //CHOSING THE RANDOM LETTERS
    string chosenLetters;
    char dice[CUBE_SIDES];
    for (int i = 0; i < NUM_CUBES; i++){
        for (unsigned int j = 0; j < CUBE_SIDES; j++) {
            dice[j] = CUBES[i][j];
        }
        shuffle(dice,CUBE_SIDES);
        chosenLetters += dice[0];  //takes the first letter of the shuffled word
    }
    return chosenLetters;
}

//FILLING THE GRID
void Boggle::createBoard(const string& chosenLetters, const bool& random){
    b_board.resize(BOARD_SIZE, BOARD_SIZE);
    int x = 0;
    int y = 0;
    for (unsigned int i = 0; i < chosenLetters.size(); i++) {
        b_board[y][x] = chosenLetters[i];
        x++;
        if (x == BOARD_SIZE){
            x = 0;
            y++;
        }
    }
    if (random) {
        shuffle(b_board);
    }
}

bool Boggle::alreadyGuessed(const string& word) const{
    return(find(foundWords.begin(), foundWords.end(), word) != foundWords.end());
}

bool Boggle::humanGuess(const string& guess){
    validWord = false;
    string prefix;
    Grid<bool> visited (BOARD_SIZE, BOARD_SIZE);
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            if (b_board[y][x] == guess[0]){
                findHumanWord(guess, prefix, y, x, visited);
                if (validWord == true){
                    return true;
                }
            }
        }
    }
    validWord = false;
    return false;
}

void Boggle::findHumanWord(const string& guess, string prefix, const int& y, const int& x, Grid<bool> visited){

    //check if in bounds and not visited
    if(!b_board.inBounds(y,x) || visited[y][x] == true){
        return;
    }

    //Initialization
    prefix += b_board[y][x];
    visited[y][x] = true;
    bool correctPath = true;

    if (prefix == guess){
        validWord = true;
        return;
    }

    //See if current word (prefix) is not part of the guess
    for(unsigned int i = 0; i < prefix.size(); i++){
        if(prefix[i] != guess[i]){
            correctPath = false;
            break;
        }
    }
    if(correctPath){
        for(int r = y-1; r <= y+1; r++){
            for(int c = x-1; c <= x+1; c++){
                findHumanWord(guess, prefix, r, c, visited);
            }
        }
    }
}

void Boggle::goComputer(string prefix, const int& y, const int& x, Grid<bool> visited){

    //check if in bounds and not visited
    if(!b_board.inBounds(y,x) || visited[y][x] == true){
        return;
    }

    //Initialization
    prefix += b_board[y][x];
    visited[y][x] = true;

    if(prefix.length() >= 4){
        if(!alreadyGuessed(prefix)) {
            if(dictionary.contains(prefix)){
               computerScore += prefix.length() - 3;
               foundWords.push_back(prefix);
               computerWords.push_back(prefix);
            }
        }
    }

    if(prefix.length() >= 2){
        if(dictionary.containsPrefix(prefix)){
            for(int r = y-1; r <= y+1; r++){
                for(int c = x-1; c <= x+1; c++){
                    goComputer(prefix, r, c, visited);
                }
            }
        }
    }
    else if (prefix.length() == 1) {
        for(int r = y-1; r <= y+1; r++){
            for(int c = x-1; c <= x+1; c++){
                goComputer(prefix, r, c, visited);
            }
        }
    }
}



