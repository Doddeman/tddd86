//INITIAL INCLUDES
#include <iostream>
#include <ostream>
#include <string>
using namespace std;

//MY INCLUDES
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <cmath>

//MY GLOBAL VARIABLES
const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
const string FILENAME = "di.txt";
set<string> dictionary;
int wordLength;
int guesses;
bool visibleDictionary;
string madeGuesses;
string currentWord;

void createDictionary()
{
    cout << "Please give us a word length" << endl;
    cin >> wordLength;

    ifstream infile(FILENAME);
    string word;
    while (getline(infile, word))
    {
        //insert words of the right length into mySet
        if (word.length() == wordLength) dictionary.insert(word);
    }
    cout << "size of dictionary: " << dictionary.size() << endl;
    if (dictionary.size() == 0)
    {
        cout << "no words of that length, please try again" << endl << endl;
        createDictionary();
    }
}

void initiateCurrentWord()
{
    for (int i = 0; i < wordLength; i++) currentWord += "-";
}

void determineGuesses()
{
    cout << "Now decide how many guesses you want" << endl;
    cin >> guesses;
    if (guesses < 1)
    {
        cout << "You need at least 1, tardface" << endl << endl;
        determineGuesses();
    }
}

bool debugDictionary()
{
    string yesOrNo;
    cout << "See remainig words in dictionary after each guess?" << endl;
    cout << "type 'yes' or 'no'" << endl;
    cin >> yesOrNo;
    if (yesOrNo == "y" || yesOrNo == "yes") return true;
    else if (yesOrNo == "n" || yesOrNo == "no") return false;
    else
    {
        cout << "You have to type yes or no" << endl << endl;
        return debugDictionary();
    }
}

string printCurrentWord()
{
    return currentWord;
}

void printDictionary()
{
    set<string>::iterator iter;
    cout << "Words left: " << endl;
    for (iter=dictionary.begin(); iter!=dictionary.end(); ++iter) cout << *iter << endl;
    cout << endl;
}

int findWordFamilyNumber(string word, char guess)
{
    int result = 0;
    for (int i=0; i < word.length(); i++)
    {
        if (guess == word[i]) result += pow(2,i);
    }
    return result;
}

queue<string> findBiggestFamily(map<int,queue<string>> allFamilies)
{
    set<string> emptyDictionary;
    queue<string> currentQueue;

    for(auto iter=allFamilies.begin(); iter!=allFamilies.end(); ++iter)
    {
        //cout << iter << endl;
        if(iter->second.size() > currentQueue.size())
        {
            currentQueue = iter->second;
        }
    }

    //This last part updates the dictionary
    //to the words in the biggest word family
    queue<string> copyQueue = currentQueue;
    dictionary = emptyDictionary;
    while(!currentQueue.empty())
    {
        string word = currentQueue.front();
        currentQueue.pop();
        dictionary.insert(word);
    }
    return copyQueue;
}

queue<string> makeFamilies(char& guess)
{
    map<int,queue<string>> allPossibleFamilies;
    queue<string> wordFamily;
    int numberOfFamilies = pow(2,wordLength);
    for (int i=0; i<numberOfFamilies; i++)
    {
        allPossibleFamilies.insert(pair<int, queue<string>>(i,wordFamily));
    }

    set<string>::iterator word;
    for (word=dictionary.begin(); word!=dictionary.end(); ++word)
    {
        int familyNumber = findWordFamilyNumber(*word, guess);
        allPossibleFamilies[familyNumber].push(*word);
    }
    return findBiggestFamily(allPossibleFamilies);
}

void makeGuess()
{
    char guess;
    cout << "Make a guess: ";
    cin >> guess;
    if (!(ALPHABET.find(guess) != string::npos))
    {
        cout << "Must be a letter in the alphabet" << endl;
        makeGuess();
    }
    else if (madeGuesses.find(guess) != string::npos)
    {
        cout << "You already made that guess, try again" << endl;
        makeGuess();
    }
    else
    {
        makeFamilies(guess);
        madeGuesses += guess;
        guesses--;
    }
}

void play()
{
    cout << endl << "Let's play!" << endl << endl;
    while (guesses > 0)
    {
        cout << "Guesses left: " << guesses << endl;
        cout << "You have already guessed on: " << madeGuesses << endl;
        cout << "Current word: " << printCurrentWord() << endl << endl;
        if (visibleDictionary) printDictionary();
        makeGuess();
    }
    cout << "Sorry, you lose!" << endl;
}

int main() {

    cout << "Welcome to Hangman." << endl;
    createDictionary();
    initiateCurrentWord();
    determineGuesses();
    visibleDictionary = debugDictionary();
    play();

    return 0;
}

/*Min tanke var att skapa en queue of sets för partitioneringen.
man kollar varje ordfamilj, och plockar rätt ord från
dictionary till currentSet. När det inte finns fler av
ordfamiljen i dictionary går man till nästa ordfamilj
och nästa set. Detta görs tills dictionary är slut.
Lägg varje set i queue. Kolla igenom varje set och jämför
storleken (while(!queue.empty())) behåll den största.
dictionary = chosenVector */

/*Det blir lite annorlunda. Jag kollar varje ord som finns kvar
i ordlistan och tar fram dess binära tal med avseende på hur
många gånger och på vilka positioner som gissningen finns i
ordet. Detta tal används som key i en map som innehåller
queues som innehåller strängar. Dessa queues är alltså alla
de olika ordfamiljerna som skapas efter en gissning, och mapen
är alla ordfamiljer. När jag tagit reda på vilket tal som tillhör
ordet så sorterar jag det till rätt queue i mappen. När detta är
helt klart så itererar jag över mapen för att se vilken queue som
är störst. Denna blir nu den nya ordlistan, och currentword måste
uppdateras efter den ordfamilj som valts. */


