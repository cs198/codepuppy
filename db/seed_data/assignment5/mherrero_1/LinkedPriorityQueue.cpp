/*
 * Filename: LinkedPriorityQueue.cpp - Matt Herrero
 * -------------------------------------------------------------------------------
 * This is the .cpp file for the ListNode implementation of the priorityQueue. This
 * class provides the functionality of a priorityQueue with the behavior described
 * in the assignment. This implementation keeps track of each entry using the ListNode
 * struct provided. Each node stores a PQEntry with value and priority, and a pointer
 * to the next node in the queue. The queue is kept in sorted order of most urgent to
 * least urgent. This means that dequeuing and peeking is very simple, but enqueuing is
 * much more involved and slower.
 */
#include "LinkedPriorityQueue.h"

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front=NULL;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's destructor has a run time of O(N).
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N^2).
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(front->value == value) {
        if(front->priority > newPriority) {
            ListNode* trash = front;
            front = front->next;
            delete trash;
            enqueue(value, newPriority);
            return;
        } else {
            throw "Error: new priority cannot be greater than old priority";
        }
    } else {
        ListNode* current = front;
        while(current->next!=NULL) {
            if(current->next->value == value) {
                if(current->next->priority > newPriority) {
                    ListNode* trash = current->next;
                    current->next = current->next->next;
                    delete trash;
                    enqueue(value, newPriority);
                    return;
                } else {
                    throw "Error: new priority cannot be greater than old priority";
                }
            }
            current = current->next;
        }
    }
    throw "Error: the given value is not found in this priority queue";
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
void LinkedPriorityQueue::clear() {
    ListNode* nextPointer = front->next;
    delete front;
    while(nextPointer!=NULL) {
        ListNode* temp = nextPointer->next;
        delete nextPointer;
        nextPointer = temp;
    }
    front = NULL;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
string LinkedPriorityQueue::dequeue() {
    if(front==NULL) throw "Error: priority queue is empty";
    string value = front->value;
    ListNode* trash = front;
    front = front->next;
    delete trash;
    return value;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newNode = new ListNode(value, priority);
    if(front==NULL){
        newNode->next = NULL;
        front = newNode;
    } else if (front->next == NULL){
        if(front->priority > priority || (front->priority == priority && front->value > value)) {
            newNode->next = front;
            front = newNode;
        } else {
            newNode->next = NULL;
            front->next = newNode;
        }
    } else {
        enqueueThreeOrMore(newNode, value, priority);
    }
}

/*
 * This method locates the appropriate place and enqueues a new ListNode based on the value and priority
 * passed to the method. This method works if the queue has a size of three or greater and the entry
 * does not belong in either of the first two places. It iterates through the list until the current
 * pointer is pointing at the ListNode that should be immediately in front of the new node. Then, it
 * inserts the new node.
 *
 * This implementation's method has a runtime of O(N).
 */
void LinkedPriorityQueue::enqueueThreeOrMore(ListNode* newNode, string value, int priority) {
        ListNode* current = front;
        while(current->next != NULL) {
            if(current->next->priority > priority || (current->next->priority == priority
                                                      && current->next->value > value)) {
                newNode->next = current->next;
                current->next = newNode;
                return;
            }
            current = current->next;
        }
        newNode->next = NULL;
        current->next = newNode;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
bool LinkedPriorityQueue::isEmpty() const {
    return front==NULL;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
string LinkedPriorityQueue::peek() const {
    if(front==NULL) throw "Error: priority queue is empty";
    return front->value;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
int LinkedPriorityQueue::peekPriority() const {
    if(front==NULL) throw "Error: priority queue is empty";
    return front->priority;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* current = front;
    while(current!=NULL) {
        size++;
        current = current->next;
    }
    return size;   // remove this
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()) {
        ListNode* current = queue.front;
        out << "\"" << current->value << "\":" << current->priority;
        current= current->next;
        while(current!=NULL) {
            out <<", " << "\"" << current->value << "\":" << current->priority;
            current = current->next;
        }
    }
    out << "}";
    return out;
}
