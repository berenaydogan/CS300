// Beren Aydoğan - HW2

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "AvlSearchTree.h"
#include "strutils.cpp"

using namespace std;

// Function to check if a string consists of all alphabetic characters
bool isAlphaString(const string & str) {
    // Enhanced for loop to iterate through all characters in a string
    for (char ch : str) {
        // If any character is not alphabetic, return false
        if (!isalpha(ch)) {
            return false;
        }
    }
    // If all characters are alphabetic, return true
    return true;
}

// Function to preprocess the given file by opening it, selecting 
// and formatting the words (if necessary) in the file according to restrictions
// and placing them in a tree
void preProcessFile(const string & fileName, AvlSearchTree<string, WordItem *> & myTree) {
    
    string word, returnFind;

    // Create an ifstream object
    ifstream inputFile;

    // Open the file using the open method and convert fileName to a C-style string
    inputFile.open(fileName.c_str());
    
    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        cerr << "Failed to open " << fileName << endl;
        exit(1);
    }

    // While possible, read the next word on the document
    while (inputFile >> word){

        // Remove any punctuation before or after the word
        StripPunc(word);

        // If the word is made up of all alphabetic characters
        // (the words containing punctuations or numbers inside are not included in the tree)
        if (isAlphaString(word)) {
            // Turn the word to lowercase
            ToLower(word);
            
            // Search the word, if found on the tree, check the vector details in WordItem
            AvlNode<string, WordItem *> * found = myTree.find(word);
            if (found != nullptr) {
                vector<DocumentItem *> & foundVector = (found->getWordItem())->details;
                bool isDocFound = false;
                for (DocumentItem* doc : foundVector) {
                    // If the document is already listed, update the count of the word
                    if (doc->documentName == fileName) {
                        doc->count++;
                        isDocFound = true;
                        break;
                    }
                }
                // If the document is not listed, add it
                if (!isDocFound) {
                    foundVector.push_back(new DocumentItem(fileName, 1));
                }
            }
            // If not found, insert the tree a node corresponding to the word
            else { 
                myTree.insert(word);
                AvlNode<string, WordItem *> * insertedNode = myTree.find(word);
                vector<DocumentItem *> & insertedVector = (insertedNode->getWordItem())->details;
                insertedVector.push_back(new DocumentItem(fileName, 1)); 
            }
        }
    }

    // Close the file
    inputFile.close();
    
}


// Function to search the preprocessed file and 
// return the number of instances found of word in given file
int searchDoc(string fileName, string word, AvlSearchTree<string, WordItem *> & myTree) {

    // Search the document tree for the word
    AvlNode<string, WordItem *> * found = myTree.find(word);

    // If the word is found
    if (found != nullptr) {
        vector<DocumentItem *> foundVector = (found->getWordItem())->details;
        for (DocumentItem* doc : foundVector) {
            if (doc->documentName == fileName) {
                return doc->count;
            }
        }
    }
    return 0;
}

// Function to search all the file and 
// return the number of instances found of word in files in a vector
vector<int> searchAllDocs(vector<string> fileNames, string word, AvlSearchTree<string, WordItem *> & myTree) {
    vector<int> wordCount;
    for (int i = 0; i < fileNames.size(); i++) {
        wordCount.push_back(searchDoc(fileNames[i], word, myTree));
    }
    return wordCount;
}

int main() {

    // Construct a tree for inserting the words
    AvlSearchTree<string, WordItem *> myTree("");

    int numOfInputFiles;
    string fileName, line, word;

    // Vector for storing the names of the files
    vector<string> fileNames;

    // Take the number of input files to be entered from the user
    cout << "Enter number of input files: ";
    cin >> numOfInputFiles;

    // For numOfInputFiles take a file name as input
    for(int i = 1; i < numOfInputFiles + 1; i++) {

        // Get the file name from user
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;

        // Add the fileName to the fileNames vector
        fileNames.push_back(fileName);

        // Preprocess the file
        preProcessFile(fileName, myTree);

    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Take the words to be queried from the user
    cout << "Enter queried words in one line: ";
    getline(cin, line);

    // Strip the line the user entered from preceeding or succeeding punctuations and whitespace
    StripPunc(line);
    StripWhite(line);

    // While the user doesn't enter the string "ENDOFINPUT" case-insensitively
    while(LowerString(line) != "ENDOFINPUT") {

        istringstream iss(line);

        vector<string> words;
        vector<vector<int> > wordCounts;

        // Take the words in line one by one
        while (iss >> word) {
            
            // Turn the word to lowercase to process case-insensitively
            ToLower(word);

            // Push the obtained word into the words vector
            words.push_back(word);

            // Push the obtained word's count into the wordCounts vector
            wordCounts.push_back(searchAllDocs(fileNames, word, myTree));

        }

        // If the first word in words is "remove", remove the succeeding word
        if (words[0] == "remove") {
            myTree.remove(words[1]);
            cout << words[1] << " has been REMOVED" << endl;
        }

        else {

            // Flag to indicate if any documents contain the words in the query
            bool anyContains = false;

            for(int i = 0; i < fileNames.size(); i++) {
                // Flag to indicate if anything has been printed for the queried word
                bool anyPrinted = false;

                for(int j = 0; j < wordCounts.size(); j++) {

                    // If the word exists in the file and if it is the first word being printed for the document
                    if (wordCounts[j][i] > 0 && !anyPrinted) {
                        cout << "in Document " << fileNames[i] << ", " << words[j] << " found " << wordCounts[j][i] << " times";
                        anyPrinted = true;
                        anyContains = true;
                    }

                    // If the word exists in the file and if it is not the first word being printed for the document
                    else if (wordCounts[j][i] > 0 && anyPrinted) {
                        cout << ", " << words[j] << " found " << wordCounts[j][i] << " times";
                    }

                }

                // If anything has been printed, print . to conclude the sentence
                if(anyPrinted) {
                    cout << "." << endl;
                }
            }
            // If no document contains any of the queried words
            if (!anyContains) {
                cout << "No document contains the given query" << endl;
            }
        }

        // Take the input again
        cout << "\nEnter queried words in one line: ";
        getline(cin, line);
    }

    return 0;

}
