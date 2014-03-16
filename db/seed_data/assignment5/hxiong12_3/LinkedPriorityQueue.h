/*This file exports a set of methods that implement the LinkedPriorityQueue data structure.
 * The LinkedPriorityQueue is a "prioritized" queue in which the elements are placed in order
 * by an integer "priority" and a corresponding value.
 * LinkedPriorityQueue uses a linked list with internal ListNode data structures.
 *
 * LinkedPriorityQueue runs in O(N) time for enqueue, size, and changePriority, and in O(1) for
 * dequeue, peek, and other methods.
 *
 * For documentation, see LinkedPriorityQueue.cpp.
 * Used pseudocode from assignment spec.
 * */

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>
#include <iostream>
#include <string>
#include <strlib.h>
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
    ListNode* front; //pointer to the first element of the Linked List

    /*Helper method for changePriority that adds a changed ListNode to its new position in the list.*/
    void searchInsert(ListNode*& moved);
};

#endif
