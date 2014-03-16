//HeapPriorityQueue.h
// This file contains the implementation of the HeapPriorityQueue structure.

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue(); //Constructor
    ~HeapPriorityQueue(); //Destructor
    void changePriority(string value, int newPriority); //Changes the priority of an element
    void clear(); //Clears all elements in the queue
    string dequeue(); //Removes the highest priority element in the queue
    void enqueue(string value, int priority); //Adds an element to the queue
    bool isEmpty() const; //Returns true if the array has no elements
    string peek() const; //Returns the string value of the most urgent element
    int peekPriority() const; //Returns the integer priority of the element with the most urgency
    int size() const; //Returns the number of elements in the queue
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue); //Prints the queue

private:
    /*Private Instance Variables*/
    PQEntry* m_array;
    int m_size;
    int capacity;

    /*Helper Methods*/
    void checkResize(); //Checks and resizes the array (capacity) accordingly
    void swapElements(int index1, int index2); //Swaps two elements within the array
    int getIndex(string value, int newPriority); //Gets the index that matches the specified value
    void bubbleUp (int startIndex); //Moves a value up in the array as long as its priority is more urgent than the element above
    PQEntry moveLastToTop(); //Returns the element at index 1 and then moves the last element to index 1
    void bubbleDown(); //Moves the element at index 1 down until it is not less urgent than any of its children
};

#endif
