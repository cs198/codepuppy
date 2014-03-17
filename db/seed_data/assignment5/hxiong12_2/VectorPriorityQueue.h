/*This file exports a set of methods that implement the VctorPriorityQueue data structure.
 * The VectorPriorityQueue is a "prioritized" queue in which the elements are placed in order
 * by an integer "priority" and a corresponding value.
 * VectorPriorityQueue uses a vector as its internal data structure.
 *
 * For documentation, see VectorPriorityQueue.cpp.
 * Used pseudocode from assignment spec.
 * */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

class VectorPriorityQueue {
public:

/*Initializes the Priority Queue and the internal data structures.*/
    VectorPriorityQueue();

/*Clears the Priority Queue and frees any related memory*/
    ~VectorPriorityQueue();

/*Changes the priority of an existing element in the list so that it is more urgent.
 * Inputs: a string value, and an integer newPriority.
 * If newPriority is less than the original priority, program throws an exception.*/
    void changePriority(string value, int newPriority);

/*Removes the elements in the Priority Queue without freeing internal memory.*/
    void clear();

/*Returns the value of the most urgent member in the queue, and removes it from the queue.*/
    string dequeue();

/*Puts a new entry into the Priority Queue.
 * Inputs: a string value, and an integer priority.*/
    void enqueue(string value, int priority);

/*Returns if the Priority Queue is empty.*/
    bool isEmpty() const;

/*Returns the value of the most urgent member of the list.*/
    string peek() const;

/*Returns the priority of the most urgent member of the list.*/
    int peekPriority() const;

/*Returns the size of the Priority Queue.*/
    int size() const;

/*Converts the members of the Priority Queue into a suitable output string form.*/
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector<PQEntry> v; //private vector to store the members of the priority queue

};

#endif
