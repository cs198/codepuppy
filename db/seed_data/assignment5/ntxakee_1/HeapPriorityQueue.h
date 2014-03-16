/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Priority Queue
 * This program implements a priority queue using a binary heap.
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
    // private member variables
    PQEntry* priorityQueue;
    int m_size; // number of elements
    int capacity; // number of slots

    // private member functions

    /*
     * Resizes the heap once the size reaches the capacity.
     */
    void resize();

    /*
     * Performs the recursive act of "bubbling up" and swapping an element with its parent
     * index so long as the element has a more urgent priority than its parent index.
     */
    int bubbleUp(int index, PQEntry entry, PQEntry* priorityQueue);

    /*
     * Swaps the elements at the given index and parent index.
     */
    void bubbleUpHelper(int index, int parentIndex, PQEntry entry, PQEntry* priorityQueue);

    /*
     * Performs the recursive act of "bubbling down" and swapping an element with its more urgent
     * child index.
     */
    int bubbleDown(int index, PQEntry entry, PQEntry* priorityQueue);

    /*
     * Swaps the elements at the given index and child index.
     */
    void bubbleDownHelper(int index, int childIndex, PQEntry entry, PQEntry* priorityQueue);
};

#endif
