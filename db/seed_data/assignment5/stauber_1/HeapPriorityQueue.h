//File: HeapPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the class of HeapPriorityQueue
//  that implements a heap priority queue using a binary heap
//  array. Each element contains a PQEntry with a string value
//  and int priority. The heap priority queue is efficient in
//  enqueue and dequeue with a time of O(log(N)).
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
    //O(1)
    HeapPriorityQueue();
    //O(1)
    ~HeapPriorityQueue();
    //O(log(N))
    void changePriority(string value, int newPriority);
    //O(1)
    void clear();
    //O(log(N))
    string dequeue();
    //O(log(N))
    void enqueue(string value, int priority);
    //O(1)
    bool isEmpty() const;
    //O(1)
    string peek() const;
    //O(1)
    int peekPriority() const;
    //O(1)
    int size() const;
    //O(N)
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    //Array that stores queue
    PQEntry* pqueue;
    //Size of queue
    int m_size;
    //Capacity of pqueue storage
    int capacity;
    //Function that resizes pqueue capacity if needed. O(N)
    void checkResize();
    //Bubbles up entry at index. O(log(N))
    void bubbleUp(int index);
    //Bubbles down first entry. O(log(N))
    void bubbleDown();
};

#endif
