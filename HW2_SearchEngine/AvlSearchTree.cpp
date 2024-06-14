// Implemented using the AvlTree implementation in lecture slides
#include <iostream>
#include "AvlSearchTree.h"

using namespace std;

// Parametrized onstructor to construct an empty tree
template <class Key, class Value>
AvlSearchTree<Key, Value>::AvlSearchTree(const Key & notFound) 
    : ITEM_NOT_FOUND(notFound), root(nullptr){} // Initialize ITEM_NOT_FOUND to be used and root to null

// Private method to get element field in node 
// Return the element field of t or ITEM_NOT_FOUND if t is nullptr
template <class Key, class Value>
const Key & AvlSearchTree<Key, Value>::elementAt(AvlNode<Key, Value> *node) const {
    if (node != nullptr) {
        return node->wordItemStr;
    }
    else {
        return ITEM_NOT_FOUND;
    }
}

// Recursive private method to find an item in a tree or subtree
// If found, returns the node containing the matched item 
// If not found, returns nullptr
template <class Key, class Value>
AvlNode<Key, Value> * AvlSearchTree<Key, Value>::find(const Key & searchedItem, AvlNode<Key, Value> *givenRoot) const {
    // If not found, returns nullptr
    if (givenRoot == nullptr) {
        return nullptr;
    }
    // If searchedItem is larger than givenRoot->wordItemStr, recursively call find for givenRoot->rightChild
    else if (searchedItem > givenRoot->wordItemStr) {
        return find(searchedItem, givenRoot->rightChild);
    }
    // If searchedItem is smaller than givenRoot->wordItemStr, recursively call find for givenRoot->leftChild
    else if (searchedItem < givenRoot->wordItemStr) {
        return find(searchedItem, givenRoot->leftChild);
    }
    // If found, return the node containing the matched item 
    else {
        return givenRoot;
    }
}

// Public method to find item x in the tree
// Returns the matching node
template <class Key, class Value>
AvlNode<Key, Value> * AvlSearchTree<Key, Value>::find(const Key & searchedItem) const {
    return find(searchedItem, root);
}

// Recursive private method to find the minimum element in a tree.
// Returns the node containing the minimum item.
template <class Key, class Value>
AvlNode<Key, Value> * AvlSearchTree<Key, Value>::findMin(AvlNode<Key, Value> *givenRoot) const {
    if (givenRoot == nullptr) {
        return nullptr;
    } 
    if (givenRoot->leftChild == nullptr) {
        return givenRoot;
    }
    return findMin(givenRoot->leftChild);
}

// Public method to find the smallest item in the tree.
// Returns the smallest item or ITEM_NOT_FOUND if empty.
template <class Key, class Value>
const Key &  AvlSearchTree<Key, Value>::findMin() const {
    return elementAt(findMin(root));
}

// Recursive private method to find the maximum element in a tree or subtree
// Returns the node containing the maximum item
template <class Key, class Value>
AvlNode<Key, Value> * AvlSearchTree<Key, Value>::findMax(AvlNode<Key, Value> *givenRoot) const {
    if (givenRoot == nullptr) {
        return nullptr;
    } 
    if (givenRoot->rightChild == nullptr) {
        return givenRoot;
    }
    return findMax(givenRoot->rightChild);

}

// Public method to find the largest item in the tree
// Returns the largest item or ITEM_NOT_FOUND if empty
template <class Key, class Value>
const Key & AvlSearchTree<Key, Value>::findMax() const {
    return elementAt(findMax(root));
}

// Recursive private method to insert into a tree or subtree
template <class Key, class Value>
void AvlSearchTree<Key, Value>::insert(const Key & itemToBeInserted, AvlNode<Key, Value> * & givenRoot) const {
    // If tree is empty
    if (givenRoot == nullptr) {
        givenRoot = new AvlNode<Key, Value>(itemToBeInserted, nullptr, nullptr);
    }

    // If tree is empty
    else if(itemToBeInserted < givenRoot->wordItemStr) {
        // Insert at the left subtree of the current node
        insert(itemToBeInserted, givenRoot->leftChild);

        // Check if the left tree is out of balance as left subtree grew in height
        if (height(givenRoot->leftChild) - height(givenRoot->rightChild) == 2) {
            // If the item was inserted to the left-left subtree
            if (itemToBeInserted < givenRoot->leftChild->wordItemStr) {
                LLRotation(givenRoot);
            }

            // If the item was inserted to the left-right subtree
            else {
                LRRotation(givenRoot);
            }
        }
    }
    else if(givenRoot->wordItemStr < itemToBeInserted){
        // Insert at the right subtree of the current node
        insert(itemToBeInserted, givenRoot->rightChild);  

        // Check if the right tree is out of balance as right subtree grew in height
        if (height(givenRoot->rightChild) - height(givenRoot->leftChild) == 2) {
            // If the item was inserted to the right-right subtree
            if (itemToBeInserted > givenRoot->rightChild->wordItemStr) {
                RRRotation(givenRoot);
            }

            // If the item was inserted to the right-left subtree
            else {
                RLRotation(givenRoot);
            }
        }
    }

    // Do nothing if the want to be inserted item is a duplicate

    // Update the height of the node
    givenRoot->height = max(height(givenRoot->leftChild), height(givenRoot->rightChild)) + 1;
}

// Insert the item into the tree if it isn't a duplicate
template <class Key, class Value>
void AvlSearchTree<Key, Value>::insert(const Key & itemToBeInserted) {
    insert(itemToBeInserted, root);
}

// Recursive private method to remove from a tree or subtree
template <class Key, class Value>
void AvlSearchTree<Key, Value>::remove(const Key & itemToBeDeleted, AvlNode<Key, Value> * & givenRoot) const {

    if(givenRoot == nullptr) {
        return; // Item not found; do nothing
    }
    
    if(itemToBeDeleted < givenRoot->wordItemStr) {
        remove(itemToBeDeleted, givenRoot->leftChild);
    }

    else if(givenRoot->wordItemStr < itemToBeDeleted) {
        remove(itemToBeDeleted, givenRoot->rightChild);
    }

    // If the node to be deleted has two children
    else if(givenRoot->leftChild != nullptr && givenRoot->rightChild != nullptr) { 
        givenRoot->wordItemStr = findMin(givenRoot->rightChild)->wordItemStr;
        remove(givenRoot->wordItemStr, givenRoot->rightChild);
    }

    // If the node to be deleted has one or no children
    else {
        AvlNode<Key, Value> *nodeToBeDeleted = givenRoot;
        if (givenRoot->leftChild != nullptr) {
            givenRoot = givenRoot->leftChild;
        } 
        else {
            givenRoot = givenRoot->rightChild;
        }
        delete nodeToBeDeleted;
    }

    // If the tree had one node 
    if(givenRoot == nullptr)  {
        return; 
    }

    // Update height
    givenRoot->height = max(height(givenRoot->leftChild), height(givenRoot->rightChild)) + 1;

    // Balance the tree
    if(height(givenRoot->leftChild) - height(givenRoot->rightChild) == 2) {
        if(height(givenRoot->leftChild->leftChild) >= height(givenRoot->leftChild->rightChild)) {
            LLRotation(givenRoot);
        }
        else {
            LRRotation(givenRoot);
        }
    }
    else if(height(givenRoot->rightChild) - height(givenRoot->leftChild) == 2) {
        // Right heavy
        if(height(givenRoot->rightChild->rightChild) >= height(givenRoot->rightChild->leftChild)) {
            RRRotation(givenRoot);
        }
        else {
            RLRotation(givenRoot);
        }
    }
}

// Remove the item from the tree if it is in the tree, otherwise do nothing
template <class Key, class Value>
void AvlSearchTree<Key, Value>::remove(const Key & itemToBeDeleted) {
    remove(itemToBeDeleted, root);
}

// Recursive private method to make tree with the given node empty
template <class Key, class Value>
void AvlSearchTree<Key, Value>::makeEmpty(AvlNode<Key, Value> * & givenRoot) const {
    if(givenRoot != nullptr) {
        makeEmpty(givenRoot->leftChild);
        makeEmpty(givenRoot->rightChild);
        delete givenRoot;
    }
    givenRoot = nullptr;
}

// Public method to make the tree empty
template <class Key, class Value>
void AvlSearchTree<Key, Value>::makeEmpty() {
    makeEmpty(root);
}

// Destructor for the tree
template <class Key, class Value>
AvlSearchTree<Key, Value>::~AvlSearchTree(){
    makeEmpty();
}

// Return true if tree is empty, false otherwise
template <class Key, class Value>
bool AvlSearchTree<Key, Value>::isEmpty() const{
    return (root == nullptr);
}

// Copy constructor
template <class Key, class Value>
AvlSearchTree<Key, Value>::AvlSearchTree(const AvlSearchTree<Key, Value> & rhs) :
        root(nullptr), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND)
{ 
    *this = rhs;
}

// Private method to clone subtree.
template <class Key, class Value>
AvlNode<Key, Value> * AvlSearchTree<Key, Value>::clone(AvlNode<Key, Value> * t) const {
    if (t == nullptr)
        return nullptr;
    else
        return new AvlNode<Key, Value>(t->wordItemStr, clone(t->leftChild), clone(t->rightChild));
}

// Overloading = operator
template <class Key, class Value>
const AvlSearchTree<Key, Value> & AvlSearchTree<Key, Value>::operator=(const AvlSearchTree & rhs) {
    if (this != &rhs) {
        makeEmpty(this);
        this = clone(rhs->root);
    }
    return *this;
}

// Return the height of node t, or -1, if nullptr.
template <class Key, class Value>
int AvlSearchTree<Key, Value>::height(AvlNode<Key, Value> *t) const {
    if (t == nullptr)
        return -1;

    return t->height;
}

// Return maximum of lhs and rhs according to their heights
template <class Key, class Value>
int AvlSearchTree<Key, Value>::max(int lhs, int rhs) const {
    if (lhs > rhs)
        return lhs;

    return rhs;
}

// Rotate binary tree node with left child.
// Left-Left (LL) Rotation
template <class Key, class Value>
void AvlSearchTree<Key, Value>::LLRotation(AvlNode<Key, Value> * & unbalanced) const {
    AvlNode<Key, Value> *unbalancedLeft = unbalanced->leftChild;
    unbalanced->leftChild = unbalancedLeft->rightChild;
    unbalancedLeft->rightChild = unbalanced;
    unbalanced->height = max(height(unbalanced->leftChild), height(unbalanced->rightChild)) + 1;
    unbalancedLeft->height = max(height(unbalancedLeft->leftChild), unbalanced->height) + 1;
    unbalanced = unbalancedLeft;
}

// Right-Right (RR) Rotation
template <class Key, class Value>
void AvlSearchTree<Key, Value>::RRRotation(AvlNode<Key, Value> * & unbalanced) const {
    AvlNode<Key, Value> *unbalancedRight = unbalanced->rightChild;
    unbalanced->rightChild = unbalancedRight->leftChild;
    unbalancedRight->leftChild = unbalanced;
    unbalanced->height = max(height(unbalanced->leftChild), height(unbalanced->rightChild)) + 1;
    unbalancedRight->height = max(height(unbalancedRight->rightChild), unbalanced->height) + 1;
    unbalanced = unbalancedRight;
}

// Left-Right (LR) Double Rotation
template <class Key, class Value>
void AvlSearchTree<Key, Value>::LRRotation( AvlNode<Key, Value> * & unbalanced) const {
    RRRotation(unbalanced->leftChild);
    LLRotation(unbalanced);
}

// Right-Left (RL) Double Rotation
template <class Key, class Value>
void AvlSearchTree<Key, Value>::RLRotation(AvlNode<Key, Value> * & unbalanced) const {
    LLRotation(unbalanced->rightChild);
    RRRotation(unbalanced);
}


