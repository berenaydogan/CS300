// Templated dynamic stack class implementation
// StackTemp.cpp

#include <iostream>

// Default constructor
template <class Type>
StackTemp<Type>::StackTemp() {
    stackTop = nullptr;
}

// Destructor
template <class Type>
StackTemp<Type>::~StackTemp() {
    emptyOut(); // Use emptyOut() function to destruct the stack
}

// Checks if the stack is empty
// Returns true if empty, else false
template <class Type>
bool StackTemp<Type>::isEmpty() const {
    return (stackTop == nullptr); // Returns true if stackTop == nullptri else false
}

// Empties the stack
template <class Type>
void StackTemp<Type>::emptyOut() {
    NodeTemp *temp;

    // While the stack is not empty, iterate through it and delete 
    while (!isEmpty()) {
        temp = stackTop->next;
        stackTop->next = nullptr;
        delete stackTop;
        stackTop = temp;
    }
}

template <class Type>
void StackTemp<Type>::pop(){

    // Inform the user if empty and return
    if (isEmpty()) {
        cout << "Stack is empty, pop operation is not valid." << endl;
        exit(1);
    }

    // Keep the next element of stackTop
    NodeTemp *temp;
    temp = stackTop->next;

    // Delete the top of the stack
    stackTop->next = nullptr;
    delete stackTop;

    // Assign the stackTop variable to new stack top
    stackTop = temp;
}

template <class Type>
void StackTemp<Type>::push(const Type & elem){
    stackTop = new NodeTemp(elem, stackTop);
}

template <class Type>
Type StackTemp<Type>::returnPop() {

    // Inform the user if empty and return
    if (isEmpty()) {
        cout << "Stack is empty, pop operation is not valid." << endl;
        exit(1);
    }

    // Keep the element in the node to be removed of the stack
    Type removedElem = stackTop->elem;

    // Pop the element
    pop();

    // Return the element in the removed node of the stack
    return removedElem;
}

// Member function to access private stackTop variable and returning it
template <class Type>
Type StackTemp<Type>::returnTop() {

    // Inform the user if empty and return
    if (isEmpty()) {
        cout << "Stack is empty, returnTop operation is not valid." << endl;
        exit(1);
    }

    // Return the element in the top node of the stack
    return stackTop->elem;
}


// Member function to reverse a stack using another stack for ease of print in main function
template <class Type>
const StackTemp<Type> & StackTemp<Type>::reverseStack() {

    // Declare a temporary stack for the reversal of the stack
    StackTemp<Type> reversedStack;

    // While stack is not empty, push its elements into the temporary stack
    while (!isEmpty()) {
        reversedStack.push(returnPop());
    }

    // Set the reference stack to the temporary stack to keep value
    *this = reversedStack;

    return *this;
}

// Overloading of the = operator for deep copy
template <class Type>
const StackTemp<Type> & StackTemp<Type>::operator=(const StackTemp<Type> & rhs) {
    if (this != &rhs) {

        emptyOut( ); // Empty the current values of the stack

        NodeTemp *tempRhs = rhs.stackTop; // Declare a temporary pointer to iterate through the rhs stack

        // Declare a temporary pointer to keep the top element of the new stack
        NodeTemp *tempLhs  = new NodeTemp(tempRhs->elem); 
        stackTop = tempLhs;

        // Move to next element on the rhs stack
        tempRhs = tempRhs->next;

        // While tempRhs is still pointing to a valid node, continue iteration for copying
        while (tempRhs != nullptr) {
            tempLhs->next = new NodeTemp(tempRhs->elem);
            tempLhs = tempLhs->next;
            tempRhs = tempRhs->next;
        }
    }

    // Return a reference to lhs
    return *this;
}