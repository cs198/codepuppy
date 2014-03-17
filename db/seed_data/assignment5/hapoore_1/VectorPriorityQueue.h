#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

/*
 * This is the Vector implementation of the priority queue. It uses a vector to store the
 * queue internally. The elements are stored in unsorted order, so enqueueing is efficient,
 * but dequeueing and peeking are inefficient because the entire vector must be traversed.
 */

class VectorPriorityQueue {
public:
    VectorPriorityQueue();
    ~VectorPriorityQueue();

    /* Changes the priority of the first entry with the given value found in the queue.
     * Throws an exception if the value is not found in the queue or already exists
     * with a priority better than the desired new priority.
     *
     * O(N)
     */
    void changePriority(string value, int newPriority);

    /* Clears the current priority queue and returns it to the state at which it existed
     * right when it was created.
     *
     * O(1)
     */
    void clear();

    /* Returns the value associated with the most urgent priority and removes it from the queue.
     * Throws an exception if the queue is empty.
     *
     * O(N)
     */
    string dequeue();

    /* Adds a value to the queue with the desired priority.
     *
     * O(1)
     */
    void enqueue(string value, int priority);

    /* Returns true if there are currently no elements in the queue. Otherwise it returns false.
     *
     * O(1)
     */
    bool isEmpty() const;

    /* Returns the value associated with the most urgent priority without removing it from the queue.
     * Throws an exception if the queue is empty.
     *
     * O(N)
     */
    string peek() const;

    /* Returns the integer value associated of the most urgent priority without removing its value from the queue.
     * Throws an exception if the queue is empty.
     *
     * O(N)
     */
    int peekPriority() const;

    /* Returns an integer representing the number of elements currently stored in the queue.
     *
     * O(1)
     */
    int size() const;

    /* Overloads the << operator to allow for easy printing of the queue to the console.
     *
     * O(N)
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    /* This private variable stores the queue internally. Elements are not stored in sorted order.
     */
    Vector<PQEntry> unsorted;

    /* This private helper function finds the element with the most urgent priority. It returns the
     * index where the element is stored and stores the entry itself in a reference parameter.
     *
     * O(N)
     */
    int findMostUrgent(PQEntry& mostUrgent, int urgentIndex = 0) const;

};

#endif
