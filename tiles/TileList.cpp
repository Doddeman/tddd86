// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"
using namespace std;

TileList::TileList(void)
{
    cout << "List is being constructed" << endl;
}

TileList::~TileList(void)
{
    delete[] myList;
    cout << "List is being deleted" << endl;
}

void TileList::addTile(Tile tile)
{
    checkResize();
    myList[tilesInList] = tile;
    tilesInList++;
}

void TileList::drawAll(QGraphicsScene* scene) const
{
    for(int i=0; i<tilesInList;i++) myList[i].draw(scene);
}

int TileList::indexOfTopTile(int x, int y) const
{
    for(int i = tilesInList; i>=0; i--)
    {
        if (myList[i].contains(x, y))
        {
            cout << "index of tile: " << i << endl;
            return i;
        }
    }
    cout << "No tile" << endl;
    return -1;
}

void TileList::raise(int x, int y)
{
    int tileIndex = indexOfTopTile(x, y);
    if(tileIndex != -1 && tileIndex != tilesInList-1)
    {
        Tile temp = myList[tileIndex];
        for(int i = tileIndex; i < tilesInList; i++) myList[i] = myList[i+1];
        myList[tilesInList-1] = temp;
        //cout << temp.toString() << endl;
    }
}

void TileList::lower(int x, int y)
{
    int tileIndex = indexOfTopTile(x, y);
    if(tileIndex != -1 && tileIndex != 0)
    {
        Tile temp = myList[tileIndex];
        for(int i = tileIndex; i > 0; i--) myList[i] = myList[i-1];
        myList[0] = temp;
    }
}

void TileList::remove(int x, int y)
{
    int tileIndex = indexOfTopTile(x, y);
    if(tileIndex != -1)
    {
        for(int i = tileIndex; i < tilesInList; i++) myList[i] = myList[i+1];
        tilesInList--;
    }
}

void TileList::removeAll(int x, int y)
{
    for(int i = tilesInList-1; i>=0; i--)
    {
        if(myList[i].contains(x,y)) remove(x, y);
    }
}

void TileList::checkResize()
{
    if (tilesInList == capacity)
    {
        cout << "Doubling capacity from " << capacity << " to " << capacity*2 << endl;
        Tile* copyList = new Tile[capacity*2];
        for (int i = 0; i < tilesInList;i++) copyList[i] = myList[i];

        delete[] myList;    //Free myList's memory
        myList = copyList;  //Overwrite myList with copyList
        capacity *= 2;
    }
}
