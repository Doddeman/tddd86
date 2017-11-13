#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

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

string Boggle::randomBoard(){
    //CHOSING THE RANDOM LETTERS
    string chosenLetters;
    string currentWord;
    char dice[CUBE_SIDES];
    for (int i = 0; i < NUM_CUBES; i++){
        currentWord = CUBES[i];
        for (unsigned int j = 0; j < currentWord.length(); j++) {
            dice[j] = currentWord[j];
        }
        shuffle(dice,CUBE_SIDES);
        char letter = dice[0]; //Just takes the first letter of the shuffled word
        chosenLetters += letter;
    }
    for(unsigned int i = 0; i < chosenLetters.length(); i++){
    }
    return chosenLetters;
}

//FILLING THE GRID
void Boggle::createBoard(string chosenLetters, bool random){
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

bool Boggle::alreadyGuessed(string word){
    bool alreadyGuessed;
    for (string str : foundWords) {
        if (str == word) {
            alreadyGuessed = true;
            break;
        }
    }
    return alreadyGuessed;
}

bool Boggle::humanGuess(string guess){
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

void Boggle::findHumanWord(string guess, string prefix, int y, int x, Grid<bool> visited){

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
        findHumanWord(guess, prefix, y+1, x, visited); //S
        findHumanWord(guess, prefix, y+1, x-1, visited); //SW
        findHumanWord(guess, prefix, y, x-1, visited); //W
        findHumanWord(guess, prefix, y-1, x-1, visited); //NW
        findHumanWord(guess, prefix, y-1, x, visited); //N
        findHumanWord(guess, prefix, y-1, x+1, visited); //NE
        findHumanWord(guess, prefix, y, x+1, visited); //E
        findHumanWord(guess, prefix, y+1, x+1, visited); //SE
    }
    else{
        return;
    }
}

void Boggle::goComputer(string prefix, int y, int x, Grid<bool> visited){

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
            goComputer(prefix, y+1, x, visited); //S
            goComputer(prefix, y+1, x-1, visited); //SW
            goComputer(prefix, y, x-1, visited); //W
            goComputer(prefix, y-1, x-1, visited); //NW
            goComputer(prefix, y-1, x, visited); //N
            goComputer(prefix, y-1, x+1, visited); //NE
            goComputer(prefix, y, x+1, visited); //E
            goComputer(prefix, y+1, x+1, visited); //SE
        }
    }
    else if (prefix.length() == 1) {
        goComputer(prefix, y+1, x, visited); //S
        goComputer(prefix, y+1, x-1, visited); //SW
        goComputer(prefix, y, x-1, visited); //W
        goComputer(prefix, y-1, x-1, visited); //NW
        goComputer(prefix, y-1, x, visited); //N
        goComputer(prefix, y-1, x+1, visited); //NE
        goComputer(prefix, y, x+1, visited); //E
        goComputer(prefix, y+1, x+1, visited); //SE
    }
    else{
        return;
    }
}



