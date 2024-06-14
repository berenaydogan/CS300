// Beren Aydoğan 31938 HW4

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "Sorting.h"
#include "strutils.cpp"

using namespace std;

// Define a structure for Contact
struct Contact {
    string name;
    string surname;
    string phone;
    string city;

    // Operator overloads for comparison based on full name
    bool operator<(const Contact& rhs) const {
        return getFullName() < rhs.getFullName();
    }

    bool operator>(const Contact& rhs) const {
        return getFullName() > rhs.getFullName();
    }

    bool operator<=(const Contact& rhs) const {
        return getFullName() <= rhs.getFullName();
    }

    // Function to get full name by combining name and surname
    string getFullName() const {
        return name + " " + surname;
    }
};

// Function to read the PhoneBook file and load data into a vector
void createPhoneBook(const string& filename, vector<Contact>& phoneBook) {
    ifstream file(filename);

    // Print an error message if file opening fails 
    if (!file.is_open()) {
        cerr << "Error opening file " << endl;
        exit(1);
    }

    string line;

    // Read each line from the file to create a Contact and add to phoneBook
    while (getline(file, line)) {
        istringstream iss(line);
        Contact contact;
        iss >> contact.name >> contact.surname >> contact.phone >> contact.city;
        contact.name = UpperString(contact.name);
        contact.surname = UpperString(contact.surname);
        phoneBook.push_back(contact); // Push the entry to the phone book
    }
}

bool findMatch(Contact contact, string query) {
    StripWhite(query);
    return (contact.getFullName().substr(0, query.length()) == query || contact.getFullName() == query);
}

// Sequential Search function
bool sequentialSearch(const vector<Contact>& phoneBook, vector<Contact>& matches, const string& query) {
    bool isFound = false;

    // Iterate through each contact in the phoneBook
    for (Contact contact : phoneBook) {
        // Check if the query is a substring of the contact's name or matches the full name
        if (findMatch(contact, query)) {
            matches.push_back(contact); // Add matching contact to the matches vector
            isFound = true;
        }
    }

    return isFound;
}

// Binary Search function
bool binarySearch(const vector<Contact>& phoneBook, vector<Contact>& matches, const string& query) {
    int left = 0;
    int right = phoneBook.size() - 1;
    int mid;
    bool isFound = false;

    // Standard binary search to find an initial match
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (findMatch(phoneBook[mid], query)) {
            isFound = true;
            break;
        }
        if (phoneBook[mid].getFullName() < query) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    // If an initial match is found, expand outwards to find all matches
    if (isFound) {
        // Find all matches to the left of mid
        int i = mid;
        while (i >= left && findMatch(phoneBook[i], query)) {
            matches.push_back(phoneBook[i]);
            i--;
        }

        // Find all matches to the right of mid
        i = mid + 1;
        while (i <= right && findMatch(phoneBook[i], query)) {
            matches.push_back(phoneBook[i]);
            i++;
        }
    }

    return isFound;
}

// Function to measure performance of sorting and searching algorithms
void measurePerformance(vector<Contact>& phoneBook, const string& query) {
    Sorting<Contact> sorter;

    // Copy the original phoneBook for each sorting algorithm
    vector<Contact> phoneBook1 = phoneBook;
    vector<Contact> phoneBook2 = phoneBook;
    vector<Contact> phoneBook3 = phoneBook;
    vector<Contact> phoneBook4 = phoneBook;

    cout << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;

    // Measure sorting times for various sorting algorithms
    auto start = chrono::high_resolution_clock::now();
    sorter.insertionSort(phoneBook1);
    auto end = chrono::high_resolution_clock::now();
    auto insertionSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    sorter.quickSort(phoneBook2);
    end = chrono::high_resolution_clock::now();
    auto quickSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    sorter.mergeSort(phoneBook3);
    end = chrono::high_resolution_clock::now();
    auto mergeSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    sorter.heapSort(phoneBook4);
    end = chrono::high_resolution_clock::now();
    auto heapSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // Output sorting times
    cout << "Quick Sort Time: " << quickSortTime << " Nanoseconds" << endl;
    cout << "Insertion Sort Time: " << insertionSortTime << " Nanoseconds" << endl;
    cout << "Merge Sort Time: " << mergeSortTime << " Nanoseconds" << endl;
    cout << "Heap Sort Time: " << heapSortTime << " Nanoseconds" << endl;
    cout << endl;

    cout << "Searching for " << query << endl;
    cout << "======================================" << endl;

    string upperQuery = UpperString(query);

    vector<Contact> binaryMatches;
    vector<Contact> sequentialMatches;
    double N = 100; // Number of iterations to average search times

    // Measure search times for binary search
    cout << "Search results for Binary Search:" << endl;
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        binarySearch(phoneBook2, binaryMatches, upperQuery);  // Assume Quick Sort was used
    }
    end = chrono::high_resolution_clock::now();

    binaryMatches.clear();

    auto binarySearchTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    if (binarySearch(phoneBook2, binaryMatches, upperQuery)) {
        sorter.quickSort(binaryMatches);
        for (const auto& contact : binaryMatches) {
            cout << contact.name << " " << contact.surname << " " << contact.phone << " " << contact.city << endl;
        }
    } else {
        cout << upperQuery << " does NOT exist in the dataset" << endl;
    }

    // Output binary search time
    cout << "\nBinary Search Time: " << binarySearchTime << " Nanoseconds" << endl;
    cout << endl;

    // Measure search times for sequential search
    cout << "Search results for Sequential Search:" << endl;
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        sequentialSearch(phoneBook2, sequentialMatches, upperQuery);  // Assume Quick Sort was used
    }
    end = chrono::high_resolution_clock::now();

    sequentialMatches.clear();

    auto sequentialSearchTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    if (sequentialSearch(phoneBook2, sequentialMatches, upperQuery)) {
        sorter.quickSort(sequentialMatches);
        for (const auto& contact : sequentialMatches) {
            cout << contact.name << " " << contact.surname << " " << contact.phone << " " << contact.city << endl;
        }
    } else {
        cout << upperQuery << " does NOT exist in the dataset" << endl;
    }

    // Output sequential search time
    cout << "\nSequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl;
    cout << endl;

    // Calculate and display speedups
    cout << "SpeedUp between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search / Binary Search) SpeedUp = " << (double)sequentialSearchTime / (double)binarySearchTime << endl;
    cout << endl;

    cout << "SpeedUp between Sorting Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Insertion Sort / Quick Sort) SpeedUp = " << (double)insertionSortTime / (double)quickSortTime << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << (double)mergeSortTime / (double)quickSortTime << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << (double)heapSortTime / (double)quickSortTime << endl;
}

int main() {
    string filename;
    string query;
    vector<Contact> phoneBook;

    // Prompt user to enter the contact file name
    cout << "Please enter the contact file name: " << endl;
    cin >> filename;
    createPhoneBook(filename, phoneBook);

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    // Prompt user to enter the query string
    cout << "Please enter the word to be queried: " << endl;
    getline(cin, query);

    cout << endl;

    // Measure performance of sorting and searching
    measurePerformance(phoneBook, query);

    return 0;
}
