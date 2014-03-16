/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Vector Priority Queue header file
 * Stores its elements in an unsorted vector
 * Dequeuing and peeking are inefficient, and therefore
 * this data structure is inefficient for storing large amounts of data.
 */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

class VectorPriorityQueue {
public:
    // constructor
    VectorPriorityQueue();
    // destructor
    ~VectorPriorityQueue();
    // changes the priority of a given value to the new priority
    void changePriority(string value, int newPriority);
    // clears all elements in the queue
    void clear();
    // returns the value of the most urgent element and deletes it
    string dequeue();
    // adds a new PQ entry
    void enqueue(string value, int priority);
    // returns true if queue is empty, false otherwise
    bool isEmpty() const;
    // returns the value of the most urgent element
    string peek() const;
    // returns the priority of the most urgent element
    int peekPriority() const;
    // returns the size of the queue
    int size() const;
    // overloads the printing "<<" operator
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    // vector of PQEntries to store data
    Vector<PQEntry> v;
    // returns the index of the lowest priority
    int getLowestPriorityIndex() const;
    // returns the entry at the desired index
    PQEntry getEntryAt(int index) const;
};

#endif
