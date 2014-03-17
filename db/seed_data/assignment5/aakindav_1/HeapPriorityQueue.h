/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* HeapPriorityQueue.h creates the HeapPriorityQueue class
* and all its private, public functions as well as memeber
* variables.
* PriorityQueue backed by a Heap back by an array.
*/

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
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry* m_array; // points to an array
    int array_size;  // returns the size of the heap
    int capacity;   // returns the capacity of

    void checkResize(); // checks to see if the capacity must be increased
                       // for a new element to be added to the array.
    PQEntry* getArray() const; // returns the array
};

#endif
