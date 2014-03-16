/*
 * Header file for HeapPriorityQueue
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

    // O(NlogN)
    void changePriority(string value, int newPriority);

    // O(1)
    void clear();

    // O(logN)
    string dequeue();

    // O(logN)
    void enqueue(string value, int priority);

    // O(1)
    bool isEmpty() const;

    // O(1)
    string peek() const;

    // O(1)
    int peekPriority() const;

    // O(1)
    int size() const;

    // O(N)
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry* queue;
    int s;
    int capacity;

};

#endif
