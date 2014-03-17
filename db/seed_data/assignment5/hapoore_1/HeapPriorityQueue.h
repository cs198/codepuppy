/*
 * This is the binary heap implementation of the priority queue. It stores the data
 * internally in a softly sorted array. The most urgent element is guaranteed to be
 * stored at index 1, which makes peeking very efficient. Enqueueing and dequeueing are
 * relatively efficient due to the binary structure. The least efficient function is change
 * priority, which must traverse the entire array.
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
    // O(1)
    HeapPriorityQueue();

    // O(1)
    ~HeapPriorityQueue();

    // O(N)
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
    int length;
    int capacity;

    // O(logN)
    void bubbleUp(int currIndex);

    // O(logN)
    void bubbleDown(int index = 1);

    // O(N)
    void resize();
};

#endif
