/* Name: Kevin Vo
 * TA: Jimmy lee
 * This is the h file for the vector based queue structure.
 * The functions are described in detail below.
 */

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

/* This is the h file for the vector class. Each function is
 * described in further detail in the cpp file. The public portion
 * contains the following:
 * 1) constructor: in charge of creating the object
 * 2) destructor: cleans up memory for structures that dynamically allocate memeory
 * 3) change priority: changes the priority of an existing element in a queue
 * 4) enqueue: adds a new element into the queue
 * 4) isEmpty: checks to see if queue is empty
 * 5) peek: returns the value
 * 6) peekpriorty: returns the priority
 * 7) size: returns the size of the queue
 * 8) Also the << operator has been overloaded to print out the class object.
*/
class VectorPriorityQueue {
public:
    VectorPriorityQueue();
    ~VectorPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector <PQEntry> vector;
};

#endif
