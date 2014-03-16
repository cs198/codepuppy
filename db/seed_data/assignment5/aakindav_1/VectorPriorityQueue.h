/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* VectorPriorityQueue.h creates the VectorPriorityQueue class
* and all its private, public functions as well as memeber
* variables.
* PriorityQueue backed by a vector.
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
    /* Constructs a new priority queue
     */
    VectorPriorityQueue();

    /* Destructor that frees internally allocated memory
     */
    ~VectorPriorityQueue();

    /* Changes the priority of a passed in PQEntry
     * if the priority is of lesser urgenency an
     * error is thrown and also an error is thrown if
     * the value is not in the queue.
     */
    void changePriority(string value, int newPriority);

    /* Deletes all PQEntries in the queue
     */
    void clear();

    /* Dequeues the PQEntry at the most urgent priority and
     * return the value at that priority
     * Throws error is queue is empty
     */
    string dequeue();

    /* Enqueues a PQEntry when passed in a value and priority
     */
    void enqueue(string value, int priority);

    /* Returns true or false whether or not the queue is empty or not
     */
    bool isEmpty() const;

    /* Returns the value at the most urgent priority and
     * throws error if queue is empty
     */
    string peek() const;

    /* Retruns the integer of the most urgent priority
     * and throws a error if the queue is empty
     */
    int peekPriority() const;

    /* Returns the size of the queue.
     */
    int size() const;

    /* Returns the string representation of the queue
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector<PQEntry> vec; // the backhand vectore used to store the PQ

    PQEntry getEntry(int index) const; // returns the PQEntry at a given index
};

#endif
