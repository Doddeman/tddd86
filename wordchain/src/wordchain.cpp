#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
using namespace std;

#include <sstream>
#include <set>

//Global stuff
const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
string word1;
string word2;
set<string> mySet; //dictionary
int wordLength;

void welcome()
{
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words of the same length, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;

    cout << "Please type two words with space between them: ";
    cin >> word1;
    cin >> word2;
    wordLength = word1.length();
}

void makeDictionary()
{
    ifstream infile("dictionary.txt");
    string word;
    while (getline(infile, word))
    {
        //insert words of the right length into mySet
        if (word.length() == wordLength) mySet.insert(word);
    }
    mySet.erase(word1);
}

void printStack(stack<string> myStack)
{
    cout << "Chain from " << word2 << " back to " << word1 << ": " << endl;
    while (!myStack.empty())
    {
        cout << myStack.top() << " "; //print every word in the finished word chain
        myStack.pop(); //pop every word in the final stack
    }
}

bool inDict(string word)
{
    if (mySet.find(word) != mySet.end())
    {
        mySet.erase(word); //Makes sure that you cannot use the same word twice
        return true;
    }
}

bool isNeighbour(string& currentWord, string& tempWord)
{
    int counter = 0;
    for (int k = 0; k < wordLength; k++)
    {
        if (currentWord[k] == tempWord[k]) counter++;
    }
    if (counter == wordLength - 1) return true;
}

stack<string> findNeighbours(string word)
{
    string tempWord = word;
    stack<string> result;
    for (int i = 0; i < wordLength; i++)
    {
        tempWord = word;
        for (int j = 0; j < ALPHABET.length(); j++)
        {
            tempWord[i] = ALPHABET[j];

            if (inDict(tempWord))
            {
                if (isNeighbour(word,tempWord)) result.push(tempWord);
            }
        }
    }
    return result;
}

void wordchain()
{
    queue<stack<string> > myQueue;
    stack<string> myStack;
    myStack.push(word1);
    myQueue.push(myStack);

    while (!myQueue.empty())
    {
        myStack = myQueue.front();
        myQueue.pop();

        if (myStack.top() == word2) printStack(myStack);
        else
        {
            stack<string> allNiceNeighbours = findNeighbours(myStack.top());
            while (!allNiceNeighbours.empty())
            {
                string nowThis = allNiceNeighbours.top();
                allNiceNeighbours.pop();
                stack<string> copyStack = myStack;
                copyStack.push(nowThis);
                myQueue.push(copyStack);
            }
        }
    }
}

int main()
{
    welcome();
    makeDictionary();
    cout << "size of dictionary: " << mySet.size() << endl;
    wordchain();
    cout << endl << "Have a nice day." << endl;
    return 0;
}
