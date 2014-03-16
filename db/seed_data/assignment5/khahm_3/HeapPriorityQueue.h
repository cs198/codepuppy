/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Heap Priority Queue cpp file
 * Stores its elements in an binary sorted array of PQEntries
 * All methods are efficient
 * Therefore, this is the most efficient data structure in this assignment.
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
    // pointer to the array of elements of PQEntries
    PQEntry* elements;
    // size of array
    int listSize;
    // capacity of array
    int listCapacity;
    // returns the PQEntry at the desired index
    PQEntry getElementAt(int index) const;
    // returns true if entry at index1 is more urgent than entry at index2
    bool isPriority(int index1, int index2) const;
    // swaps the entries at index1 and index2
    void swapEntries(int index1, int index2);
    // bubbles up the entry at the index until its its parent is more urgent than itself
    void bubbleUp(int index);
    // bubbles down the entry at the index until its children are less urgent than itself
    void bubbleDown(int index);
};

#endif
