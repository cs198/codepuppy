/*This file exports a set of methods that implement the HeapPriorityQueue data structure.
 * The HeapPriorityQueue is a "prioritized" queue in which the elements are placed in order
 * by an integer "priority" and a corresponding value.
 * HeapPriorityQueue uses a binary heap as its internal data structure.
 *
 *HeapPriorityQueue runs in O(log(N)) time for enqueue and changePriority, and O(1) time for the other
 * operations.
 *
 * For documentation, see HeapPriorityQueue.cpp.
 * Used pseudocode from assignment spec; checkResize method from Marty's lecture
 * */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "strlib.h"
#include "PQEntry.h"
//#include "array"
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
    PQEntry* h;    // array of PQEntries to store elements
    int m_size;    // # of elements added
    int capacity;  // # of slots the array has

    /*Private method to check if the array needs to be expanded, and does so if necessary. Called by enqueue method.*/
    void checkResize();

    /*Private helper method for changePriority function; inserts a changed element at a new index.*/
    int search(string value, int newPriority);

    /*Private helper method for dequeue method; handles "bubbling down" of dequeued values.*/
    void dequeueHelper(int index);

    /*Private helper method: changes the values of two elements in the list.*/
    void swap(int i, int j);
};

#endif
