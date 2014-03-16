/* File: VectorPriorityQueue.h
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * --------------------------------
 * The interface for the VectorPriorityQeue class.
 * Also refer to this interface for information on the
 * HeapPriorityQueue and LinkedPriorityQueue classes.
 */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
#include <climits>
#include "strlib.h"
using namespace std;


class VectorPriorityQueue {
public:
    //Constructor of a VectorPriorityQueue object
    //O(1)
    VectorPriorityQueue();

    //Deconstructor of the VectorPriorityQueue object. This prevents memory leaks.
    //O(1)
    ~VectorPriorityQueue();

    /*Modifies the priority of a value that already exists in the queue.
     * If the value given is not in the queue or the new priority is less urgent
     * than the current priority, a string exception is thrown. If the value
     * occurs multiple times in the queue, only the first value encountered is altered.
     */
    //O(N)
    void changePriority(string value, int newPriority);

    //Removes all elements from the priority queue.
    //O(1)
    void clear();

    /* Removes the element with the most urgent priority and returns it. If
     * the queue is empty, it throws a string exception.
     */
    //O(N)
    string dequeue();

    /* Adds the given string value to the priority queue with the priority passed as
     * an argument. Duplicates are allowed and there are no illegal arguments.
     */
    //O(1)
    void enqueue(string value, int priority);

    //Returns true if the queue has no elements and false if it has elements.
    //O(1)
    bool isEmpty() const;

    /* Returns the string that has the most urgent priority in the queue without removing
     * it from the priority queue. Throws a string exception if the queue is empty.
     */
    //O(N)
    string peek() const;


    /* Returns the priority of the string that has the most urgent priority in the queue without removing
     * anything from the priority queue. Throws a string exception if the queue is empty.
     */
    //O(N)
    int peekPriority() const;

    // Returns the number of elements in the priority queue.
    //O(1)
    int size() const;

    /*Operator overloading of the << operator that allows the client
     * to print the priority queue to the console.
     */
    //O(N)
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    //Vector that stores the PQEntries.
    Vector<PQEntry> data;

};

#endif
