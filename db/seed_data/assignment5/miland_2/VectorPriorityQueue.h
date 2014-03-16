/**
 * Name: Milan Doshi
 * Assignment: HeapPriority.h
 * TA: Jimmy Lee
 *
 * This file is the header file for VectorPriorityQueue.cpp
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
    /**
     * method:VectorPriorityQueue()
     * return:
     * This method is the constructor which initializes anything which needs to be.
     */
    VectorPriorityQueue();
    /**
     * method:~VectorPriorityQueue()
     * return:
     * This method deletes any memory that needs to be deleted, if necessary.
     */
    ~VectorPriorityQueue();
    /**
     * method:changePriority(string value, int newPriority)
     * return: void
     * This method changes the priority of a value, given both the value to match to and the new value. However, it only does so if the new priorty is less than the old priority.
     */
    void changePriority(string value, int newPriority);
    /**
     * method:clear()
     * return: void
     * This method, in this case, clears all the data in the Vector. For other classes, it clears the data in whatever data structure is used to store data.
     */
    void clear();
    /**
     * method: dequeue()
     * return: void
     * This method prints the value of the element which holds the highest priority and removes it from the data structure.
     */
    string dequeue();
    /**
     * method: enqueue()
     * return: void
     * This method adds a new PQEntry to the datastructure of the given value and priority.
     */
    void enqueue(string value, int priority);
    /**
     * method: isEmpty()
     * return: bool
     * This method checks to see if the data structure is empty.
     */
    bool isEmpty() const;
    /**
     * method: peek()
     * return: string
     * This method returns the value of the element which holds the highest priority and removes it from the data structure.
     */
    string peek() const;
    /**
     * method: peekPriority()
     * return: string
     * This method returns the priority of the element which holds the highest priority and removes it from the data structure.
     */
    int peekPriority() const;
    /**
     * method: size()
     * return: int
     * This method returns the size of the data structure.
     */
    int size() const;
    /**
     * method: <<(ostream& out, const VectorPriorityQueue& queue);
     * return: ostream& operator
     * This method prints out the data structure as it is supposed to be printed.
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    Vector<PQEntry> entries;
    PQEntry getPriorityEntry() const;
};

#endif
