// Beren Aydoğan - HW3
// Implemented using the HashTable implementation in lecture slides


#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef STRUCTS
#define STRUCTS

struct DocumentItem {
    string documentName;
    int count;

    DocumentItem(string name, int count)
        : documentName(name), count(count) {}
};

struct WordItem {
    string word;
    vector<DocumentItem *> details;

    WordItem(string word)
        : word(word) {}
};

#endif

template <class HashedObj>
class HashTable;

template <class HashedObj>
int hash1(const WordItem * key, const HashTable<HashedObj> * myTable);

template <class HashedObj>
int hash2(const WordItem * key, const HashTable<HashedObj> * myTable);

template <class HashedObj>
int doubleHash(const WordItem * key, int attempts, const HashTable<HashedObj> * myTable);


template <class HashedObj>
class HashTable {

    public:
        HashTable(const HashedObj & notFound, int size = 101); // Parameterized contructor
        HashTable(const HashTable & rhs); // Copy constructor

        const HashedObj & find(const HashedObj & x) const;
        void makeEmpty();
        void insert(const HashedObj & x);
        void remove(const HashedObj & x);

        double loadFactor();
        int getCurrentSize();

        const HashTable & operator=(const HashTable & rhs);

        enum EntryType {ACTIVE, EMPTY, DELETED};

    private:
        struct HashEntry
        {
            HashedObj element;
            EntryType info;

            HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY)
                    : element(e), info(i) { }
        };
        
        int currentSize = 0;
        vector<HashEntry> array;
        const HashedObj ITEM_NOT_FOUND;

        bool isActive(int currentPos) const;
        int findPos(const HashedObj & x) const;
        void rehash();

        // Internal method to test if a positive number is prime.
        // Not an efficient algorithm.
        bool isPrime( int n )
        {
            if (n == 2 || n == 3)
                return true;

            if (n == 1 || n % 2 == 0)
                return false;

            for (int i = 3; i * i <= n; i += 2)
                if ( n % i == 0 )
                    return false;

            return true;
        }

        // Internal method to return a prime number at least as large as n.
        // Assumes n > 0.
        int nextPrime(int n)
        {
            if (n % 2 == 0) // If n is even, make it odd
                n++;

            for (; !isPrime(n); n += 2) // Check odd numbers bigger than n
                ;

              return n;
        }

        int previousPrime(int n) {
            for (int i = n - 1; i > 1; i--) { // Start checking from n-1 downwards
                if (isPrime(i)) return i;
            }
            return -1; // Return -1 if no prime number is found (which should never happen if n > 2)
        }

    
    friend int hash1<>(const WordItem * key, const HashTable<HashedObj> * myTable);
    friend int hash2<>(const WordItem * key, const HashTable<HashedObj> * myTable);
    friend int doubleHash<>(const WordItem * key, int attempts, const HashTable<HashedObj> * myTable);
};

#include "HashTable.cpp"

// Primary hash function
template <class HashedObj>
int hash1(const WordItem * key, const HashTable<HashedObj> * myTable) {
    const int prime = 37; // A small prime number
    int hashVal = 0;
    for (char c : key->word) {
        hashVal = (hashVal * prime + c) % myTable->array.size();
    }
    return hashVal;
}

// Secondary hash function to use for double hashing when a collision occurs
template <class HashedObj>
int hash2(const WordItem * key, const HashTable<HashedObj> * myTable) {
    const int prime = 17; // Another small prime number, make sure it's relatively prime to table size
    int hashVal = 0;
    for (char c : key->word) {
        hashVal = (hashVal * prime + c) % (myTable->array.size() - 1);
    }
    return (myTable->array.size() - 1 - hashVal) + 1;
}

#endif // HASHTABLE_H
