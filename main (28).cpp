/******************************************************************************

/////////////////////////////////////////////////////////////////////
// Your name: Yexcinia Cermeno
// Date: 11.11.26
// Class: CSCI 1470.04
// Semester: Fall 2026
// CSCI 1470 Instructor: Dr. Jonatan Reyes
// Program Description: Program checks spelling of words using 3 different types of array searches.
///////////////////////////////////////////////////////////////////// 

*******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <chrono>

using namespace std;

const int MAX_WORDS = 466551;
const string LINEAR_SEARCH_UNSORTED = "LINEAR_UNSORTED";
const string LINEAR_SEARCH_SORTED = "LINEAR_SORTED";
const string BINARY_SEARCH = "BINARY";
const bool DEBUGGING = true;


void initializeArray(string words[ ]); // makes array empty string uses array as parameter
void Dictionary2Array(string words[ ], bool& fileOpened); // second void func loads dictionary into array 
bool checkSpelling(string words[ ], string target, string searchType);


int wordCount = 0;

int main(){
string* dictionary = new string[MAX_WORDS]; 

int maxMistakes = 0;
int mistakes = 0;
string searchType, word, answer;
bool fileOpened = false;
bool keepPlaying = true;

    
initializeArray(dictionary);

ifstream settingsFile("settings.txt");
    if (!settingsFile){
        cout << "Error!  Settings cannot be opened." << endl;
        delete[] dictionary;
        return 1;
    }
    if (!(settingsFile >> maxMistakes >> searchType)){
    cout << "Error reading settings.txt!" << endl;
        
    return 1;
    }
    settingsFile.close();

    if (maxMistakes <= 0 ||(searchType != LINEAR_SEARCH_UNSORTED && searchType != LINEAR_SEARCH_SORTED && searchType != BINARY_SEARCH)){
        cout << "Invalid settings!" << endl;
        delete[ ] dictionary;
        return 1;
    }

    
    Dictionary2Array(dictionary, fileOpened);
    if (!fileOpened || wordCount == 0){
        cout << "Error" << endl;
        delete[ ] dictionary;
        return 1;
    }

    
if (searchType == LINEAR_SEARCH_SORTED || searchType == BINARY_SEARCH){
        sort(dictionary, dictionary + wordCount);
    } cout << "*****Welcome to the Spell Checker!*****" << endl;
    
while (keepPlaying && mistakes < maxMistakes){
        cout << "Enter a word: ";
        if (!(cin >> word))
            break;

        
transform(word.begin(), word.end(), word.begin(), ::tolower);

        
auto start = chrono::high_resolution_clock::now();
bool correct = checkSpelling(dictionary, word, searchType);
auto end = chrono::high_resolution_clock::now();
auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();

if (correct){
cout << "Yay, you spelled '" << word << "' correctly!" << endl;
mistakes = 0; //counter reset
        }
    else{
        
            mistakes++;
            cout << "Sorry, '" << word << "' isn't a word, try again!" << endl;
        }

        if (DEBUGGING){
            if (searchType == BINARY_SEARCH)
                cout << "Binary search took ";
            else
                cout << "Linear search took ";
            cout << elapsed << " microseconds." << endl;
        }

        if (mistakes >= maxMistakes){
            cout << "Maximum mistakes ("<< maxMistakes << ") has been reached." << endl;
            break;
        }
if (correct){
    cout << "Would you like to continue: Y for yes and N for no ";
        if (!(cin >> answer) || answer == "N" || answer == "n")
                keepPlaying = false;
        }
    }

    cout << "Thank you for using Spell Checker!" << endl;
    delete[] dictionary; 
    return 0;
}


void initializeArray(string words[]){
    for (int i = 0; i < MAX_WORDS; i++){
        words[i] = "";
    }
}


void Dictionary2Array(string words[], bool& fileOpened){
    ifstream inputFile("dictionary.txt"); // regular size was not working; changing it back to original dictionary did work with small one
    if (!inputFile){
        fileOpened = false;
        return;
    }
    fileOpened = true;
    string currentWord;
    wordCount = 0;

while (wordCount < MAX_WORDS && getline(inputFile, currentWord)){
    if (!currentWord.empty() && currentWord.back() == '\r') // on Mac I need this because of \r 
            currentWord.pop_back(); // this removes the \r  

    if (!currentWord.empty()){transform(currentWord.begin(), currentWord.end(),currentWord.begin(), ::tolower);
    words[wordCount] = currentWord;
    wordCount++;
        }
    }
  
}


bool checkSpelling(string words[], string target, string searchType){
    if (searchType == LINEAR_SEARCH_UNSORTED){
    for (int i = 0; i < wordCount; i++){
            if (words[i] == target)
                return true;
        }
        return false;
    }
    else if (searchType == LINEAR_SEARCH_SORTED){
        for (int i = 0; i < wordCount; i++){
            if (words[i] == target)
                return true;
            if (words[i] > target)
                return false;
        }
        return false;
    }
    else if (searchType == BINARY_SEARCH){
        int first = 0;
        int last = wordCount - 1;
        while (first <= last){
            int middle = (first + last) / 2;
            if (words[middle] == target)
                return true;
        else if (words[middle] < target)
            first = middle + 1;
        else
            last = middle - 1;
        }
        return false;
    }
    return false;
}
