// VectorPriorityQueue.h
// This file contains the implementation of the VectorPriorityQueue structure.

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
    VectorPriorityQueue(); //Constructor
    ~VectorPriorityQueue(); //Destructor
    void changePriority(string value, int newPriority); //Changes the priority of an existing string in the queue
    void clear(); //Removes all elements in the queue
    string dequeue(); //Removes the element with the highest urgency from the queue and returns the string value of that element
    void enqueue(string value, int priority); //Adds a string to the queue with an associated priority
    bool isEmpty() const; //Returns true if the queue has no elements in it
    string peek() const; //Returns the string of the element with the most urgency
    int peekPriority() const; //Returns the priority of the element with the most urgency
    int size() const; //Returns the number of elements in the queue
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue); //Prints the queue

private:
    /*Private Instance Variables*/
    Vector <PQEntry> priorityVec; //Vector stores all the elements of the queue

    /*Helper Methods*/
    int getMaxEntry(PQEntry& maxEntry) const; //Sets maxEntry to the PQEntry element with the most urgency and returns the index of that element in the vector
};

#endif
