/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Linked Priority Queue header file
 * Stores its elements in an sorted (by priority) linked list of PQEntries
 * Enqueuing and size are inefficient, and but other methods are efficient.
 * Therefore, this is more efficient than vector priority queue,
 * but is still not the most efficient form.
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
    // pointer to front of the linked list
    ListNode* front;
    // method to access the pointer to the front of the linked list
    ListNode* getFront() const;
};

#endif
