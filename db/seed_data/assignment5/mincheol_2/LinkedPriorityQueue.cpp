//Min Cheol Kim
//Jimmy Lee SL
//Priority Queue

/* This is the linked list implementation of the priority queue. The elements are stored
 * in a linked list in a sorted order, so dequeuing process is O(1). However, the enqueing process
 * is O(N). Slower enqueing process than the vector implementation, but faster peek/dequeing
 * functions. */

//See VectorPriorityQueue.h for documentation member functions.

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

// O(N^2)
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    ListNode* trash;
    if (current != NULL) {
        if (current->value == value) {
            if (current->priority >= newPriority)
                current->priority = newPriority;
            else
                throw "Error: new priority cannot be greater than old priority";
            return;
        }
        while (current->next != NULL) {
            if (current->next->value == value) {
                if (current->next->priority >= newPriority) {
                    string tempValue = current->next->value;
                    trash = current->next;
                    current->next = trash->next;
                    delete trash;
                    enqueueHelper(tempValue, newPriority);
                }
                else
                    throw "Error: new priority cannot be greater than old priority";
                return;
            }
            current = current->next;
        }
    }
    throw "Error: The given value is not found in this priority queue";
}

// O(N)
void LinkedPriorityQueue::clear() {
    ListNode* current = front;
    ListNode* trash;
    front = NULL;
    while (current != NULL) {
        trash = current;
        current = current->next;
        delete trash;
    }
}

// O(N)
string LinkedPriorityQueue::dequeue() {
    if (front == NULL)
        throw "Error: priority queue is empty";
    else {
        string result = front->value;
        ListNode* trash = front;
        if (front->next == NULL)
            front = NULL;
        else
            front = front->next;
        delete trash;
        return result;
    }
}

// O(N)
void LinkedPriorityQueue::enqueue(string value, int priority) {
    enqueueHelper(value, priority);
}

void LinkedPriorityQueue::enqueueHelper(string value, int priority) {
    ListNode* node = new ListNode(value, priority, NULL);
    if (front == NULL) {
        front = node;
    }
    else if (!lowerPriority(front, value, priority)) {
            node->next = front;
            front = node;
    }
    else {
        ListNode* current = front;
        while (current->next != NULL && lowerPriority(current->next, value, priority))
            current = current->next;
        if (current->next == NULL)
            current->next = node;
        else {
            node->next = current->next;
            current->next = node;
        }
    }
}

// O(1)
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

// O(1)
string LinkedPriorityQueue::peek() const {
    if (front == NULL)
        throw "Error: priority queue is empty";
    else
        return front->value;
}

// O(1)
int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL)
        throw "Error: priority queue is empty";
    else
        return front->priority;  
}

// O(1)
int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* current = front;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* current = queue.front;
    while (current != NULL) {
        if (current->next != NULL)
            out << *current << " ";
        else
            out << *current;
       current = current->next;
    }
    out << "}";
    return out;
}

bool LinkedPriorityQueue::lowerPriority(ListNode *a, string value, int priority) {
    if (a->priority < priority)
        return true;
    else if (a->priority >priority)
        return false;
    else {
        if (a->value < value)
            return true;
        else
            return false;
    }
}
