/* Name: Kevin Vo
 * TA: Jimmy Lee
 * This is the h file for the priorty queue that uses
 * a linkedlist as a data structure. All the member
 * functions are similar to ones foudn in the vector queue.
*/

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>
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
    bool valid(string value, int newPriority, ListNode *current);
    ListNode* front;

};

#endif
