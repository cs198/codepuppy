/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This program implements a priority queue of strings, using a vector for
 * internal storage.
 *
 * (1 late day)
 */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

/*
 * A piority queue is a collection that enqueues elements with priority ratings,
 * so that the elements are dequeued in order of priority. A VectorPriorityQueue
 * stores elements in an unsorted vector. This results in fast enqueuing but
 * slow dequeuing/peeking.
 */
class VectorPriorityQueue {
public:
    /*
     * Constructor that creates an empty priority queue.
     *
     * O(1)
     */
    VectorPriorityQueue();

    /*
     * Destructor that frees memory allocated by the priority queue.
     *
     * O(1)
     */
    ~VectorPriorityQueue();

    /*
     * Changes the priority of the specified value to the new priority. The new
     * priority must be more urgent than the value's current priority. If the
     * value occurs more than once in the priority queue, then only the first
     * valid instance is changed. Throws an exception if the value is found in
     * the priority queue but already has an urgency greater than or equivalent
     * to the new priority, or if the value is not found in the priority queue.
     *
     * O(N)
     */
    void changePriority(string value, int newPriority);

    /*
     * Removes all elements from the priority queue.
     *
     * O(1)
     */
    void clear();

    /*
     * Removes and returns the value of the element with the most urgent
     * priority. Throws an exception if the priority queue is empty.
     *
     * O(N)
     */
    string dequeue();

    /*
     * Adds an element with the passed value and priority.
     *
     * O(1)
     */
    void enqueue(string value, int priority);

    /*
     * Returns whether the priority queue is empty.
     *
     * O(1)
     */
    bool isEmpty() const;

    /*
     * Returns the value of the element with the most urgent priority.
     * Throws an exception if the priority queue is empty.
     *
     * O(N)
     */
    string peek() const;

    /*
     * Returns the priority of the element with the most urgent priority.
     * Throws an exception if the priority queue is empty.
     *
     * O(N)
     */
    int peekPriority() const;

    /*
     * Returns the number of elements in the priority queue.
     *
     * O(1)
     */
    int size() const;

    /*
     * Prints out all elements of the priority queue, in the form
     * "value":priority, separated by commas and enclosed in {} braces.
     *
     * O(N)
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector<PQEntry> PQVector;   // vector that stores elements as PQEntry objects

    /*
     * Returns index of the element with least urgent priority.
     *
     * O(N)
     */
    int findLowest() const;

    /*
     * Throws an exception if the priority queue is empty. Called in dequeue,
     * peek, and peekPriority methods.
     *
     * O(1)
     */
    void checkEmpty() const;
};

#endif
