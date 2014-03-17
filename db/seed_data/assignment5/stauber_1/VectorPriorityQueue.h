//File: VectorPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the class of VectorPriorityQueue
//  that implements a vector priority queue using an unsorted vector
//  Each element contains a PQEntry with a string value
//  and int priority. The vector priority queue is efficient in
//  enqueue of O(1) but peek and dequeue are O(N).
#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

class VectorPriorityQueue {
public:
    //Parameterless constructor for class. O(1)
    VectorPriorityQueue();

    //Destructor for class. Though no memory deletion needs to be
    //handled here for VectorPriorityQueue, it is done here for other
    //Priority Queue implementations. O(1)
    ~VectorPriorityQueue();

    //Change the priority of first entry containing 'value'to a lower
    //priority. Throws error if queue is empty, doesn't contain 'value'
    //or value is already at a <= priority. O(N)
    void changePriority(string value, int newPriority);

    //Clears the queue. O(1).
    void clear();
    //Dequeue's lowest priority element and returns its value. O(N)
    string dequeue();

    //Enqueue a new entry. O(1)
    void enqueue(string value, int priority);

    //Returns true if queue is empty, false otherwise. O(1)
    bool isEmpty() const;

    //Returns the value of lowest priority entry. O(N)
    string peek() const;

    //Returns priority of lowest priority entry. O(N)
    int peekPriority() const;

    //Returns size of queue. O(1)
    int size() const;

    //Prints out entries in queue in order that queue data is stored. O(N)
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    //Unsorted vector containing queue data.
    Vector<PQEntry> pqueue;

    //Returns the index of the entry with lowest priority.
    int getFirst() const;
};

#endif
