/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* LinkedPriorityQueue.h creates the LinkedPriorityQueue class
* and all its private, public functions as well as memeber
* variables.
* PriorityQueue backed by a LinkedList.
*/

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    ListNode* front; //points to the front of the list

    ListNode* getFront() const; //returns the front pointer
};

#endif
