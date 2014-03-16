/* This is a .h file with member functions to implement priority queue using vector */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

// TODO: comment
class VectorPriorityQueue {
public:
    VectorPriorityQueue();
    ~VectorPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    PQEntry getMinPriority() const; //helper function to find element with most urgent priority
    Vector<PQEntry> v; //vector to implement priority queue
};

#endif
