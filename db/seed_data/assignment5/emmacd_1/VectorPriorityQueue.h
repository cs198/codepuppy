/*
 * Header file for VectorPriorityQueue
 */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;


class VectorPriorityQueue {
public:
    VectorPriorityQueue();
    ~VectorPriorityQueue();
    /*
     * This method changes the priority of an existing value in the queue. The value must be in the queue and the
     * new priority must be higher than the old priority otherwise a string exception will be thrown. If there are
     * multiple occurences of the same value the first one will be changed.
     *
     * O(N)
     */
    void changePriority(string value, int newPriority);

    /*
     * This method removes all the elements from the priority queue
     *
     * O(1)
     */
    void clear();

    /*
     * This method removes the element with the most urgent priority and returns the value. If the queue does not
     * contain any elements a string exception is thrown
     *
     * O(N)
     */
    string dequeue();

    /*
     * This method adds the given value and priority to the queue.
     *
     * O(1)
     */
    void enqueue(string value, int priority);

    /*
     * This method returns true if there are no elements in the queue and false otherwise
     *
     * O(1)
     */
    bool isEmpty() const;

    /*
     * This method returns the value of the most urgent element. A string exception is thrown if the queue is
     *
     * O(N)
     */
    string peek() const;

    /*
     * This method returns the priority of the most urgent element. A string exception is thrown if the queue is empty
     *
     * O(N)
     */
    int peekPriority() const;

    /*
     * This method returns the number of elements in the queue
     *
     * O(1)
     */
    int size() const;

    /*
     * This method prints the content of the queue
     *
     * O(N)
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector<PQEntry> queue;

};

#endif
