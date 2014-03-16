/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This program implements a priority queue of strings, using a binary heap for
 * internal storage.
 *
 * (1 late day)
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * A piority queue is a collection that enqueues elements with priority ratings,
 * so that the elements are dequeued in order of priority. A HeapPriorityQueue
 * stores elements in a binary heap array, such that every element is more urgent
 * than each of its two child elements. This results in efficient enqueuing and
 * dequeueing, as well as fast peeking.
 *
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    /* O(1) */
    HeapPriorityQueue();

    /* O(N) */
    ~HeapPriorityQueue();

    /* O(N) */
    void changePriority(string value, int newPriority);

    /* O(1) */
    void clear();

    /* O(log N) */
    string dequeue();

    /* O(log N), O(N) if resizing needed */
    void enqueue(string value, int priority);

    /* O(1) */
    bool isEmpty() const;

    /* O(1) */
    string peek() const;

    /* O(1) */
    int peekPriority() const;

    /* O(1) */
    int size() const;

    /* O(N) */
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry *PQHeap;    // pointer to array that stores elements as PQEntry objects
    int heapSize;       // number of elements in priority queue
    int capacity;       // capacity of array

    /*
     * Swaps the element at the given index with its parent, if it has a more
     * urgent priority. Recursively repeated until reaching a location where the
     * parent has a more urgent priority.
     *
     * O(1) per call, O(log N) total
     */
    void bubbleUp(int index);

    /*
     * Swaps the element at the given index with its most urgent child, if it
     * has a less urgent priority. Recursively repeated until reaching a
     * location where both children have a less urgent priority.
     *
     * O(1) per call, O(log N) total
     */
    void bubbleDown(int index);

    /*
     * Throws an exception if the priority queue is empty. Called in dequeue,
     * peek, and peekPriority methods.
     *
     * O(1)
     */
    void checkEmpty() const;

    /*
     * Checks if the size of the priority queue has reached the capacity of
     * the array. If needed, transfers all elements of the priority queue to a
     * new array with doubled capacity.
     *
     * O(N)
     */
    void checkResize();
};

#endif
