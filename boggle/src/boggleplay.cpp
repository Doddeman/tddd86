#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

bool randomOrCustom(){
    string result;
    cout << "Do you want a r)andom or c)ustom board?" << endl;
    getline(cin, result);
    if (result == "random" || result == "r") return true;
    else if (result == "custom" || result == "c") return false;
    else {
        cout << "Must be random or custom" << endl << endl;
        return randomOrCustom();
    }
}

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("TERM=xterm clear");
#endif
}

string Boggle::customBoard() const{
    //TAKING IN THE LETTERS
    string chosenLetters;
    cout << "Please insert 16 letters with no spaces" << endl;
    getline(cin, chosenLetters);

    for(unsigned int i = 0; i < chosenLetters.size(); i++){
        chosenLetters[i] = (char)toupper(chosenLetters[i]);
    }

    //ERROR CHECKS
    if (chosenLetters.size() != 16) {
        cout << "Must be of length 16" << endl << endl;
        return customBoard();
    }
    for (unsigned int i = 0; i < chosenLetters.size(); i++){
        if(!isalpha(chosenLetters[i])){
            cout << "Must be letters in the alphabet" << endl << endl;
            return customBoard();
        }
    }
    return chosenLetters;
}

//PRINTING THE GRID
void Boggle::printBoard() const{
    cout << endl;
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            cout << b_board[y][x];
            if (x == BOARD_SIZE - 1) cout << endl;
        }
    }
}

void Boggle::humanTurn(){
    printBoard();
    cout << endl << "Type a word (or press ENTER to finish your turn): ";
    string guess;
    getline(cin, guess);
    for(unsigned int i = 0; i < guess.length(); i++){
        guess[i] = (char)toupper(guess[i]);
    }
    if (guess != ""){
        clearConsole();
        cout << "Your guess: " << guess << endl;
        if (guess.length() < 4) {
            cout << "Your guess must be at least 4 letters, try again" << endl;
        }
        else if (!dictionary.contains(guess)) {
            cout << "Not in the dictionary, try again" << endl;
        }
        else if (alreadyGuessed(guess) == true){
            cout << "You already made that guess, try again" << endl;
        }
        else if (humanGuess(guess)){
            cout << "YOU FOUND A NEW WORD! " << "\"" << guess << "\"" << endl << endl;
            foundWords.push_back(guess);
            humanScore += guess.length() - 3;
            cout << "Your words (" << foundWords.size() << "): { ";
            for (string word : foundWords) {
                cout << word << ", ";
            }
            cout << "}" << endl;
            cout << "Your score: " << humanScore << endl;
        }
        else {
            cout << "Not on the board, try again" << endl;
        }
        humanTurn();
    }
}

void Boggle::computerTurn(){
    cout << endl << "Computer's turn!" << endl;
    string prefix;
    Grid<bool> visited (BOARD_SIZE, BOARD_SIZE);
    for(int y = 0; y < BOARD_SIZE; y++){
        for(int x = 0; x < BOARD_SIZE; x++){
            goComputer(prefix, y, x, visited);
        }
    }

    cout << "I found these words (" << computerWords.size() << "): { ";
    for (string word : computerWords) {
        cout << word << ", ";
    }
    cout << "}" << endl;
    cout << "Computer's final score: " << computerScore << endl;
    cout << "Your final score: " << humanScore << endl;
    if (computerScore > humanScore){
        cout << "I win, fool!" << endl;
    }
    else{
        cout << "Wow, you actually won..." << endl;
    }
}

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    string chosenLetters;
    bool random = randomOrCustom();
    if (random){
        chosenLetters = boggle.Boggle::randomBoard();
    }
    else {
        chosenLetters = boggle.Boggle::customBoard();
    }
    cout << "Chosen letters: " << chosenLetters << endl;
    boggle.Boggle::createBoard(chosenLetters, random);
    boggle.Boggle::reset();
    boggle.Boggle::humanTurn();
    boggle.Boggle::computerTurn();

}
