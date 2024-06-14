// Implemented using the AvlTree implementation in lecture slides

#ifndef AVLSEARCHTREE_H
#define AVLSEARCHTREE_H

#include <iostream>

using namespace std;


template <class Key, class Value> 
class AvlSearchTree; // Declaration to be able to define it as a friend class of AvlNode

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

template <class Key, class Value>
class AvlNode {
private:
    Key wordItemStr;
    Value wordItem;
    AvlNode *leftChild;
    AvlNode *rightChild;
    int height; 

    // Constructor
    AvlNode(const Key & theWordItemStr, AvlNode *left, AvlNode *right, int h = 0) 
        : wordItemStr(theWordItemStr), leftChild(left), rightChild(right), height(h) {
        wordItem = new WordItem(wordItemStr);
    }

    friend class AvlSearchTree<Key, Value>;

public:
    Value getWordItem() {
        return this->wordItem;
    }
};

template <class Key, class Value>
class AvlSearchTree {
    public:
        explicit AvlSearchTree(const Key & notFound);
        AvlSearchTree(const AvlSearchTree & rhs);
        ~AvlSearchTree();

        const Key & findMin() const;
        const Key & findMax() const;
        AvlNode<Key, Value> * find(const Key & searchedItem) const;
        bool isEmpty() const;

        void makeEmpty();
        void insert(const Key & itemToBeInserted);
        void remove(const Key & itemToBeDeleted);

        const AvlSearchTree & operator=(const AvlSearchTree & rhs);


    private:
        AvlNode<Key, Value> *root;
        const Key ITEM_NOT_FOUND; // A special value object to indicate that the item is not found.

        const Key & elementAt(AvlNode<Key, Value> *t) const;

        void insert(const Key & itemToBeInserted, AvlNode<Key, Value> * & givenRoot) const;
        void remove(const Key & itemToBeDeleted, AvlNode<Key, Value> * & givenRoot) const;
        AvlNode<Key, Value> *findMin(AvlNode<Key, Value> *givenRoot) const;
        AvlNode<Key, Value> *findMax(AvlNode<Key, Value> *givenRoot) const;
        AvlNode<Key, Value> *find(const Key & searchedItem, AvlNode<Key, Value> *givenRoot) const;

        void makeEmpty(AvlNode<Key, Value> * & givenRoot) const;
        AvlNode<Key, Value> *clone(AvlNode<Key, Value> *t) const;

        // AVL Helper Methods
        int height(AvlNode<Key, Value> *t) const;
        int max(int lhs, int rhs) const;

        // AVL ROtations
        void LLRotation(AvlNode<Key, Value> * & unbalanced) const;
        void RRRotation(AvlNode<Key, Value> * & unbalanced) const;
        void LRRotation(AvlNode<Key, Value> * & unbalanced) const;
        void RLRotation(AvlNode<Key, Value> * & unbalanced) const;
};

#include "AvlSearchTree.cpp"

#endif