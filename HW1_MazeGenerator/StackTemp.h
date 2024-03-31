// Templated dynamic stack class declaration
// StackTemp.h

#ifndef STACKTEMP_H
#define STACKTEMP_H

#include <iostream>

using namespace std;

template <class Type>
class StackTemp {
public:
    StackTemp(); // Default constructor
    ~StackTemp(); // Destructor

    bool isEmpty() const; // Returns true if empty, else false

    void emptyOut(); // Empties the stack

    void pop(); // Pops the topmost element of the stack
    void push(const Type & elem); // Pushes an element on top of the stack

    Type returnPop(); // Returns the element of the top node and pops it
    Type returnTop(); // Returns the element of the top node

    const StackTemp<Type> & reverseStack(); // Reverses the stack

    const StackTemp<Type> & operator=(const StackTemp<Type> & rhs); // Deep copies the stack
private:

    // NodeTemp struct for the nodes of the stack
    struct NodeTemp {
        Type elem; // The element the node keeps
        NodeTemp *next; // Pointer to the next element after the node

        // Parametrized constructor for NodeTemp structs
        NodeTemp(const Type & theElem, NodeTemp *theNext = nullptr) :
            elem(theElem), next(theNext) {}
    };
    
    // Pointer to the top of the stack
    NodeTemp *stackTop;
};

#include "StackTemp.cpp"

#endif