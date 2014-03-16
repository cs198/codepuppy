/*
 * Filename: LinkedPriorityQueue.h - Matt Herrero
 * -------------------------------------------------------------------------------
 * This is the .h file for the ListNode implementation of the priorityQueue. This
 * class provides the functionality of a priorityQueue with the behavior described
 * in the assignment. This implementation keeps track of each entry using the ListNode
 * struct provided. Each node stores a PQEntry with value and priority, and a pointer
 * to the next node in the queue. The queue is kept in sorted order of most urgent to
 * least urgent. This means that dequeuing and peeking is very simple, but enqueuing is
 * much more involved and slower.
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
    // This pointer points to the front node of our queue
    ListNode* front;
    /*
     * This method locates the appropriate place and enqueues a new ListNode based on the value and priority
     * passed to the method. This method works if the queue has a size of three or greater and the entry
     * does not belong in either of the first two places. It iterates through the list until the current
     * pointer is pointing at the ListNode that should be immediately in front of the new node. Then, it
     * inserts the new node.
     *
     * This implementation's method has a runtime of O(N).
     */
    void enqueueThreeOrMore(ListNode* newNode, string value, int priority);
};

#endif
