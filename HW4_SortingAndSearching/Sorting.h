// Implemented using the lecture slides

#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Templated Sorting class that includes various sorting algorithms
template <class Comparable>
class Sorting {
public:
    // Public member functions for different sorting algorithms
    void insertionSort(vector<Comparable> &array);
    void heapSort(vector<Comparable> &array);
    void mergeSort(vector<Comparable> &array);
    void quickSort(vector<Comparable> &array);

private:
    // Private helper functions used by the sorting algorithms
    void percDown(vector<Comparable> &array, int i, int n);
    int leftChild(int i);
    const Comparable & median3(vector<Comparable> &array, int left, int right);
    void insertionSortForQuick(vector<Comparable> &array, int left, int right);
    void swap(Comparable &obj1, Comparable &obj2);
    void mergeSort(vector<Comparable> &array, int left, int right);
    void merge(vector<Comparable> &array, int left, int center, int right);
    void quickSort(vector<Comparable> &array, int left, int right);
};

// Templated Insertion Sort
// This function sorts the array using the insertion sort algorithm
template <class Comparable>
void Sorting<Comparable>::insertionSort(vector<Comparable> &array) {
    int movingDownIdx;
    // Iterate through each element starting from the second element
    for (int toBeSortedIdx = 1; toBeSortedIdx < array.size(); ++toBeSortedIdx) {
        Comparable toBeSorted = array[toBeSortedIdx]; // Element to be sorted
        // Move elements of the sorted segment to the right until the correct position is found
        for (movingDownIdx = toBeSortedIdx; movingDownIdx > 0 && toBeSorted < array[movingDownIdx-1]; movingDownIdx--)
            array[movingDownIdx] = array[movingDownIdx-1];
        array[movingDownIdx] = toBeSorted; // Insert the element at the correct position
    }
}

// Templated Heap Sort
// This function sorts the array using the heap sort algorithm
template <class Comparable>
void Sorting<Comparable>::heapSort(vector<Comparable> &array) {
    // Build the heap
    for (int i = array.size() / 2; i >= 0; i--)
        percDown(array, i, array.size());
    // Extract elements from the heap one by one
    for (int j = array.size() - 1; j > 0; j--) {
        swap(array[0], array[j]); // Move the current root to the end
        percDown(array, 0, j);    // Call percDown on the reduced heap
    }
}

// Templated Merge Sort
// This function sorts the array using the merge sort algorithm
template <class Comparable>
void Sorting<Comparable>::mergeSort(vector<Comparable> &array) {
    mergeSort(array, 0, array.size() - 1);
}

// Templated Quick Sort
// This function sorts the array using the quick sort algorithm
template <class Comparable>
void Sorting<Comparable>::quickSort(vector<Comparable> &array) {
    quickSort(array, 0, array.size() - 1);
}

// Perc Down function for Heap Sort
// This function is used to maintain the heap property
template <class Comparable>
void Sorting<Comparable>::percDown(vector<Comparable> &array, int i, int n) {
    int child;
    Comparable tmp = array[i];

    // Sift down the node at index i to the proper place such that all nodes below the start node are in heap order
    for (; leftChild(i) < n; i = child) {
        child = leftChild(i);
        if (child != n - 1 && array[child] < array[child + 1])
            child++;
        if (array[child] > tmp)
            array[i] = array[child];
        else
            break;
    }
    array[i] = tmp;
}

// Helper function to get the left child of a node in a heap
template <class Comparable>
int Sorting<Comparable>::leftChild(int i) {
    return 2 * i + 1;
}

// Function to find the median of three values
// This function is used to choose a good pivot for quick sort
template <class Comparable>
const Comparable & Sorting<Comparable>::median3(vector<Comparable> &array, int left, int right) {
    int center = (left + right) / 2;

    // Order the left, center, and right elements
    if (array[center] < array[left]) swap(array[left], array[center]);
    if (array[right] < array[left]) swap(array[left], array[right]);
    if (array[right] < array[center]) swap(array[center], array[right]);

    // Place the pivot at position right-1
    swap(array[center], array[right - 1]);
    return array[right - 1];
}

// Insertion Sort used for small segments in Quick Sort
// This function sorts a small segment of the array
template <class Comparable>
void Sorting<Comparable>::insertionSortForQuick(vector<Comparable> &array, int left, int right) {
    for (int p = left + 1; p <= right; p++) {
        Comparable tmp = array[p];
        int j;
        for (j = p; j > left && tmp < array[j - 1]; j--)
            array[j] = array[j - 1];
        array[j] = tmp;
    }
}

// Swap function to swap two elements
template <class Comparable>
void Sorting<Comparable>::swap(Comparable &obj1, Comparable &obj2) {
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

// Merge Sort function that sorts the array from left to right
template <class Comparable>
void Sorting<Comparable>::mergeSort(vector<Comparable> &array, int left, int right) {
    if (left < right) {
        int center = left + (right - left) / 2;
        // Recursively sort the left and right halves
        mergeSort(array, left, center);
        mergeSort(array, center + 1, right);
        // Merge the sorted halves
        merge(array, left, center, right);
    }
}

// Merge function used in Merge Sort
// This function merges two sorted halves of the array
template <class Comparable>
void Sorting<Comparable>::merge(vector<Comparable> &array, int left, int center, int right) {
    int start2 = center + 1;

    // If the direct merge is already sorted
    if (array[center] <= array[start2]) {
        return;
    }

    // Two pointers to maintain start of both arrays to merge
    while (left <= center && start2 <= right) {
        // If the element 1 is in the right place
        if (array[left] <= array[start2]) {
            left++;
        } else {
            Comparable value = array[start2];
            int index = start2;

            // Shift all the elements between element 1 and element 2 right by 1
            while (index != left) {
                array[index] = array[index - 1];
                index--;
            }
            array[left] = value;

            // Update all the pointers
            left++;
            center++;
            start2++;
        }
    }
}

// Quick Sort function that sorts the array from left to right
template <class Comparable>
void Sorting<Comparable>::quickSort(vector<Comparable> &array, int left, int right) {
    if (left + 10 <= right) {
        // Use median-of-three partitioning
        Comparable pivot = median3(array, left, right);

        // Begin partitioning
        int i = left, j = right - 1;
        for (; ;) {
            while (array[++i] < pivot) {}
            while (pivot < array[--j]) {}
            if (i < j) swap(array[i], array[j]);
            else break;
        }

        // Restore pivot
        swap(array[i], array[right - 1]);

        // Sort small segments with recursive quick sort
        quickSort(array, left, i - 1);
        quickSort(array, i + 1, right);
    } else {
        // Use insertion sort for small segments
        insertionSortForQuick(array, left, right);
    }
}

#endif 
