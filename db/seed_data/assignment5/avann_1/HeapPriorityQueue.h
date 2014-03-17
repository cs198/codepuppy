//This is the header for the Heap implementation of a
//priority queue for CS106B Assignment 5.

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
//The three private variables for the HeapPriorityQueue class include a
//variable for the capacity, the size, and a pointer to the priority queue
//itself. There is a private function that returns the priority queue so it
//may be printed.
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
   int queuecapacity;
   PQEntry* priorityqueue;
   int queuesize;
   PQEntry* getPriorityQueue() const;
};

#endif
