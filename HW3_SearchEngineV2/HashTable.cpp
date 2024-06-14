// Beren Aydoğan - HW3
// Implemented using the HashTable implementation in lecture slides

#include "HashTable.h"

// Parametrized constructor
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size)
        : ITEM_NOT_FOUND(notFound), array(nextPrime(size)) {
    makeEmpty();
}

// Copy constructor
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashTable & rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array) {}

        
// Make the hash table logically empty
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty() {
    for(int i = 0; i < array.size(); i++) {
        array[i].element = ITEM_NOT_FOUND;
        array[i].info = EMPTY;
    }
}

// Perform double hashing to find the position of an element (that already exists or to be inserted)
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) const {
    int collisionNum = 0;
    int currentPos = hash1<WordItem *>(x, this);  // Initial hash position

    while (array[currentPos].info != EMPTY && array[currentPos].element->word != x->word) {
        collisionNum++;
        currentPos = (hash1<WordItem *>(x, this) + collisionNum*hash2<WordItem *>(x, this)) % array.size();
    }
    return currentPos;
}

// Return true if the position is active, false otherwise
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
    return array[currentPos].info == ACTIVE;
}


// Insert item x into the hash table. If the item is
// already present, then do nothing.
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x) {
    // Insert x as active
    int currentPos = findPos(x);
    if (isActive(currentPos))
        return;
    array[currentPos] = HashEntry(x, ACTIVE);
    ++currentSize;

    // Enlarge the hash table if necessary
    if (loadFactor() >= 0.7) {
        rehash();
    }
}

// Remove item x from the hash table.
// x has to be in the table
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x) {
    int currentPos = findPos(x);
    if (isActive(currentPos))
        array[currentPos].info = DELETED;
    --currentSize;
}

// Find item x in the hash table.
// Return the matching item or ITEM_NOT_FOUND if not found
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x) const {
    int currentPos = findPos(x);
    if (isActive(currentPos))
        return array[currentPos].element; // If found, return the found element

    return ITEM_NOT_FOUND; // If not found, return ITEM_NOT_FOUND
}

// Overloading = operator
template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=(const HashTable & rhs) {

    if (this != &rhs) {
        makeEmpty();
        for( int i = 0; i < array.size(); i++)
            array[i] = rhs.array[i];
    }

    return *this;
}

// Expand the hash table when needed
template <class HashedObj>
void HashTable<HashedObj>::rehash() {

    cout << "rehashed..." << endl;

    vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize(nextPrime(2 * oldArray.size()));
    for (int j = 0; j < array.size(); j++ )
        array[j].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for (int i = 0; i < oldArray.size(); i++) {
        if (oldArray[i].info == ACTIVE) {
            insert(oldArray[i].element);
        }
    }

    // Print the requested message
    cout << "previous table size: " << oldArray.size() << ", new table size: " << array.size();
    cout << ", current unique word count " << currentSize << ", current load factor: " << loadFactor();
    cout << endl;
}

// Return the current load factor of the hash table
template <class HashedObj>
double HashTable<HashedObj>::loadFactor() {
    double hf = (double) currentSize / (double) array.size();
    return hf;
}

// Return the current size of the hash table
template <class HashedObj>
int HashTable<HashedObj>::getCurrentSize() {
    return currentSize;
}

