//This is the header of the Vector implementation of a priority queue for
//CS106B Assignment 5.

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

//For the vector class priority queue, the private variable declared is the
//priority queue itselt, and the private functions include one to find
//the most urgent index in the priority queue, and another to return the
//priority queue so it may be printed.
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
    Vector<PQEntry> priorityqueue;
    int mostUrgentIndex() const;
    Vector<PQEntry> getPriorityQueue() const;
};

#endif
