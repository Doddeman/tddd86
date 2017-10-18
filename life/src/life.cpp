#include <iostream>
#include "grid.h"
#include "lifeutil.h"
using namespace std;

#include <fstream>
#include <sstream>

Grid<char> myGrid;
Grid<char> nextGrid;
int rows;
int columns;

//Prints welcome text
void welcome()
{
    cout << "Welcome to the TDDD86 Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbours dies." << endl;
    cout << "- Locations with 2 neighbours remain stable." << endl;
    cout << "- Locations with 3 neighbours will create life." << endl;
    cout << "- A cell with 4 or more neighbours die" << endl;
    cout << endl;
}

//Loops over myGrid and prints it out
void printState(){
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            cout << myGrid[y][x];
            if (x == columns - 1) cout << endl;
        }
    }
}

//Advances the colony one generation
void advance(){
    int neighbours = 0;

    //loops over grid
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            //loops over each cell's neighbours
            for (int i = y-1; i <= y+1; i++){
                for (int j = x-1; j <= x+1; j++){
                    if (i == y && j == x); //makes sure that the cell itself isn't a neighbour
                    else if (myGrid.inBounds(i,j) && myGrid[i][j] == 'X'){
                        neighbours++;
                    }
                }
            }
            //Set cell to correct value depending on amount of neighbours
            if (neighbours == 2) nextGrid[y][x] = myGrid[y][x];
            else if (neighbours == 3) nextGrid[y][x] = 'X';
            else nextGrid[y][x] = '-';

            neighbours = 0;
        }
    }
    myGrid = nextGrid;
}

//The menu
void interface(){
    char input;
    cout << "a)nimate, t)ick, q)uit?" << endl;
    cin >> input;

    //advance colony 10 generations if 'a'
    if (input == 'a'){
        int counter = 0;
        while(counter < 10){
            pause(100);
            advance();
            clearConsole();
            printState();
            cout << endl;
            counter++;
        }
        interface();
    }
    //advance colony 1 generation if 't'
    else if (input == 't'){
        advance();
        clearConsole();
        printState();
        cout << endl;
        interface();
    }
    //Do nothing (end program) if 'q'
    else if (input == 'q');

    //Else, ask again
    else interface();
}

int main() {

    welcome();

    string filename;
    cout << "Grid input file name? ";
    cin >> filename;
    cout << "You entered: " << filename << endl;
    ifstream infile(filename);
    string line;

    //get number of rows
    getline(infile, line);
    rows = stoi(line);
    cout << rows << endl;

    //get number of columns
    getline(infile, line);
    columns = stoi(line);
    cout << columns << endl;

    //sets grids to the correct size
    myGrid.resize(rows, columns);
    nextGrid.resize(rows, columns);

    int numLines = 0;
    while (getline(infile, line)){
        if (line[0] != '#') cout << line << endl;

        if (numLines < rows){
            for (int x = 0; x < columns; x++){ //iterates over each line...
                myGrid.set(numLines, x, line[x]); //...to set myGrid to the colony from the txt file
            }
        }
        numLines++;
    }

    interface();

    cout << endl << "Have a nice Life!" << endl << endl;
    return 0;
}
