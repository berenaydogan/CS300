// Beren Aydoğan - HW3

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "AvlSearchTree.h"
#include "strutils.cpp"
#include "HashTable.h"


using namespace std;

// Function to add a word to the map or update it if it already exists
void addOrUpdateWord(const string& word, const string& fileName, unordered_map<string, WordItem*>& words) {
    // Check if the word exists in the map
    WordItem *& wordItem = words[word];
    // If the word does not exist, create a new WordItem
    if (!wordItem) {
        wordItem = new WordItem(word);
    }
    // Check if the document already exists within the word's details, if it does increment the count
    bool found = false;
    for (DocumentItem*&  doc : wordItem->details) {
        if (doc->documentName == fileName) {
            doc->count++;
            found = true;
            break;
        }
    }
    // If the document does not exist, add a new document item to the word's details
    if (!found) {
        wordItem->details.push_back(new DocumentItem(fileName, 1));
    }
}

// Processes each word in a file, adding it to the map of words
void allWords(const string& fileName, unordered_map<string, WordItem*>& words) {

    // Open file
    ifstream inputFile(fileName.c_str());
    if (!inputFile.is_open()) {
        cerr << "Failed to open " << fileName << endl;
        exit(1);
    }

    // Read characters from file and construct words
    string currentWord;
    char ch;
    while (inputFile.get(ch)) {

        // If it is alphabetic, append character to current word after converting to lower case
        if (isalpha(ch)) {
            currentWord += tolower(ch); 
        } 
        
        // When a non-alphabetic character is reached, if the string isn't empty, add it to the map and clear the string
        else if (!currentWord.empty()) {
            addOrUpdateWord(currentWord, fileName, words);
            currentWord.clear();
        }
    }

    // Process the last word in the file
    if (!currentWord.empty()) {
        addOrUpdateWord(currentWord, fileName, words);
    }

    // Close the file
    inputFile.close();
}

// Inserts all words from the map into an AVL tree
void createAvlTree(unordered_map<string, WordItem *> &words, AvlSearchTree<string, WordItem *> &myTree) {
    // Iterate through each pair in the unordered_map
    for (unordered_map<string, WordItem *>::iterator it = words.begin(); it != words.end(); ++it) {
        // Insert each element into the AVL tree
        myTree.insert(it->first);
        AvlNode<string, WordItem *> * insertedNode = myTree.find(it->first);
        insertedNode->getWordItem()->word = it->second->word;
        insertedNode->getWordItem()->details = it->second->details;
    }
}

// Inserts all words from the map into a hash table
void createHashTable(std::unordered_map<std::string, WordItem *> &words, HashTable<WordItem *> &myTable) {
    for (auto it = words.begin(); it != words.end(); ++it) {
        // Insert each element into the hash table
        myTable.insert(it->second);
    }

    // Print the required message
    std::cout << "\n\nAfter preprocessing, the unique word count is " << myTable.getCurrentSize();
    std::cout << ". Current load ratio is " << myTable.loadFactor() << std::endl;
}

// Search the preprocessed file through the entries in the tree and
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

// Search all the files through the entries in the tree and
// return the number of instances found of word in files in a vector
vector<int> searchAllDocs(vector<string> fileNames, string word, AvlSearchTree<string, WordItem *> & myTree) {
    vector<int> wordCount;
    for (int i = 0; i < fileNames.size(); i++) {
        wordCount.push_back(searchDoc(fileNames[i], word, myTree));
    }
    return wordCount;
}

// Search the preprocessed file through the entries in the hash table and
// return the number of instances found of word in given file
int searchDoc(string fileName, string word, HashTable<WordItem *> & myTable) {

    // Search the document tree for the word
    WordItem * toFind = new WordItem(word);
    WordItem * found = myTable.find(toFind);

    // If the word is found
    if (found != nullptr) {
        vector<DocumentItem *> foundVector = found->details;
        for (DocumentItem* doc : foundVector) {
            if (doc->documentName == fileName) {
                return doc->count;
            }
        }
    }
    return 0;
}

// Search all the files through the entries in the hash table and
// return the number of instances found of word in files in a vector
vector<int> searchAllDocs(vector<string> fileNames, string word, HashTable<WordItem *> & myTable) {
    vector<int> wordCount;
    for (int i = 0; i < fileNames.size(); i++) {
        wordCount.push_back(searchDoc(fileNames[i], word, myTable));
    }
    return wordCount;
}

template <class DS>
void queryDocumentsPrint(DS & ds, vector<string> & queriedWords, vector<string> fileNames) {
    vector<vector<int> > wordCounts;

    // Iterate over each word in the queriedWords vector to find their occurrence in all documents
    for (string word : queriedWords) {
        // Collect word count for each word across all documents
        wordCounts.push_back(searchAllDocs(fileNames, word, ds));
    }
    
    // Flag to indicate if any document contains all the words in the query
    bool anyContains = false;

    // Flag to indicate if the specific document contains all the words in the query
    bool allContains = true;
    
    // Iterate over each document
    for(int i = 0; i < fileNames.size(); i++) {
        allContains = true; // Reset flag for each document
        
        // Check each queried word's count in the current document
        for(int j = 0; j < wordCounts.size(); j++) {
            // If any word count is zero, set allContains to false
            if (!(wordCounts[j][i] > 0)) {
                allContains = false;
            }
        }

        // If all queried words are found in the current document
        if (allContains) {
            anyContains = true; // Set anyContains to true as we found a document with all words
            cout << "in Document " << fileNames[i];
            for(int j = 0; j < wordCounts.size(); j++) {
                cout << ", " << queriedWords[j] << " found " << wordCounts[j][i] << " times";
            }
            cout << ".\n";
        }
    }

    // If no document contains all of the queried words
    if (!anyContains) {
        cout << "No document contains the given query\n";
    }
}

template <class DS>
void queryDocuments(DS & ds, vector<string> & queriedWords, vector<string> fileNames) {
    vector<vector<int> > wordCounts;

    // Iterate over each word in the queriedWords vector to find their occurrence in all documents
    for (string word : queriedWords) {
        // Collect word count for each word across all documents
        wordCounts.push_back(searchAllDocs(fileNames, word, ds));
    }
    
    // Flag to indicate if any document contains all the words in the query
    bool anyContains = false;

    // Flag to indicate if the specific document contains all the words in the query
    bool allContains = true;

    // String to collect prints
    string result = "";
    
    // Iterate over each document
    for(int i = 0; i < fileNames.size(); i++) {
        allContains = true; // Reset flag for each document
        
        // Check each queried word's count in the current document
        for(int j = 0; j < wordCounts.size(); j++) {
            // If any word count is zero, set allContains to false
            if (!(wordCounts[j][i] > 0)) {
                allContains = false;
            }
        }
        // If all queried words are found in the current document
        if (allContains) {
            anyContains = true; // Set anyContains to true as we found a document with all words
            result += "in Document " + fileNames[i];
            for(int j = 0; j < wordCounts.size(); j++) {
                result += ", " + queriedWords[j] + " found " + to_string(wordCounts[j][i]) + " times";
            }
            result += ".\n";
        }
    }
    // If no document contains any of the queried words
    if (!anyContains) {
        result += "No document contains the given query\n";
    }
}

int main() {

    // Construct a tree for inserting the words
    AvlSearchTree<string, WordItem *> myTree("");

    // Construct a hash table for inserting the words
    HashTable<WordItem*> myTable(nullptr, 53);

    // Unordered map to initially collect all the words
    unordered_map<string, WordItem*> words;

    // Vector for storing the names of the files
    vector<string> fileNames;

    int numOfInputFiles;
    string fileName, line, word;

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
        allWords(fileName, words);

    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Create data structures
    createAvlTree(words, myTree);
    createHashTable(words, myTable);

    // Take the words to be queried from the user
    cout << "Enter queried words in one line: ";
    getline(cin, line);

    // Strip the line the user entered from preceeding or succeeding punctuations and whitespace
    StripPunc(line);
    StripWhite(line);

    // Get the queried words one by one by separating them from separators
    istringstream iss(line);

    vector<string> queriedWords;
    vector<vector<int> > wordCounts;

    string currentWord;
    char ch;
    while (iss.get(ch)) {
        if (isalpha(ch)) {
            currentWord += tolower(ch);
        } else if (!currentWord.empty()) {
            queriedWords.push_back(currentWord);
            currentWord.clear();
        }
    }
    if (!currentWord.empty()) {
        queriedWords.push_back(currentWord);
    }

    // Run the query once to print results
    queryDocumentsPrint(myTree, queriedWords, fileNames);
    queryDocumentsPrint(myTable, queriedWords, fileNames);

    cout << endl;

    // Test timing and print results
    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        queryDocuments(myTree, queriedWords, fileNames);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "Time: " << BSTTime.count() / k << "\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        queryDocuments(myTable, queriedWords, fileNames);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "Time: " << HTTime.count() / k << "\n";

    double speedUp =  (double) BSTTime.count() / (double) HTTime.count();

    cout << "Speed Up: " << speedUp << endl;

    return 0;

}
