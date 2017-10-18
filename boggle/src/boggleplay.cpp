// You will edit and turn in this CPP file.


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
// TODO: include any other header files you need

//Lexicon dictionary(DICTIONARY_FILE);





bool randomOrCustom(){
    string result;
    cout << "Do you want a r)andom or c)ustom board?" << endl;
    getline(cin, result);
    //cin >> result;
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
    std::system("clear");
#endif
}

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    if (randomOrCustom()) {
        boggle.Boggle::randomBoard();
    }
    else {
        boggle.Boggle::customBoard();
    }
    boggle.Boggle::reset();
    boggle.Boggle::humanTurn();
    boggle.Boggle::computerTurn();

}
