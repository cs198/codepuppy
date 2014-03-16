/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Priority Queue
 * This program implements a priority queue using a vector.
 */

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
    /*
     * Initializes the priority queue.
     */
    VectorPriorityQueue();

    /*
     * Deletes memory allocated for the priority queue.
     */
    ~VectorPriorityQueue();

    /*
     * Modifies the priority of a given existing variable in the queue.
     * Throws a string exception if the given value is present in the queue and already has
     * a more urgent or equally urgent priority, or if the given value is not already in
     * the queue.
     */
    void changePriority(string value, int newPriority);

    /*
     * Removes all elements from the priority queue.
     */
    void clear();

    /*
     * Removes the element with the most urgent priority from the queue and returns it.
     * Throws a string exception if queue has no elements.
     */
    string dequeue();

    /*
     * Adds the given string value into the queue with the given priority.
     * Duplicates are allowed and no invalid values can be passed.
     */
    void enqueue(string value, int priority);

    /*
     * Returns true if the queue contains no elements.
     */
    bool isEmpty() const;

    /*
     * Returns the string element with the most urgent priority without removing it or
     * altering the state of the queue.
     * Throw a string exception if the queue doesn't contain any elements.
     */
    string peek() const;

    /*
     * Returns the value of the most urgent priority without removing it or
     * altering the state of the queue.
     * Throw a string exception if the queue doesn't contain any elements.
     */
    int peekPriority() const;

    /*
     * Returns the number of elements in the priority queue.
     */
    int size() const;

    /*
     * Prints the priority queue to the console.
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:

    // private member variables
    Vector<PQEntry> priorityQueue;
};

#endif
