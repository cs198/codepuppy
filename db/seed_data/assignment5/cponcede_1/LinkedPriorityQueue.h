/* File: LinkedPriorityQueue.h
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * ---------------------------------
 * The interface for the LinkedPriorityQueue class. This class
 * uses a LinkedList to store the elements of the priority queue. This means
 * that enqueueing can be complicated because the list is in sorted order but dequeueing
 * is easy because the most urgent element is at the front of the list.
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
    //O(1)
    LinkedPriorityQueue();

    //O(N)
    ~LinkedPriorityQueue();

    //O(N)
    void changePriority(string value, int newPriority);

    //O(N)
    void clear();

    //O(1)
    string dequeue();

    //O(N)
    void enqueue(string value, int priority);

    //O(1)
    bool isEmpty() const;

    //O(1)
    string peek() const;

    //O(1)
    int peekPriority() const;

    //O(N)
    int size() const;

    //O(N)
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    //Pointer to the front of the priority queue.
    ListNode* front;
};

#endif
