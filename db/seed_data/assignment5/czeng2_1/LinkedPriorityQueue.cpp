/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This file implements the LinkedPriorityQueue class described in
 * LinkedPriorityQueue.h.
 *
 * (1 late day)
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;   // empty list
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    checkEmpty();
    bool found = false;
    if(front->value == value){
        found = true;

        // Changes priority of first element, no reodering needed
        if(front->priority > newPriority){
            front->priority = newPriority;
            return;
        }
    }
    else{
        ListNode* curr = front; // pointer to traverse over list
        ListNode* prev = NULL;  // pointer to indicate where element should be inserted
        while(curr->next != NULL){
            if(curr->priority < newPriority || (curr->priority == newPriority && curr->value < value)){
                prev = curr;
            }
            if(curr->next->value == value){
                found = true;

                // Changes priority of value and removes from list
                if(curr->next->priority > newPriority){
                    curr->next->priority = newPriority;
                    ListNode* temp = curr->next;
                    curr->next = curr->next->next;

                    // Inserts at front of list
                    if(prev == NULL){
                        temp->next = front;
                        front = temp;
                    }
                    // Inserts later in list
                    else{
                        temp->next = prev->next;
                        prev->next = temp;
                    }
                    return;
                }
            }
            curr = curr->next;
        }
    }
    if(found == true){
        throw "Error: new priority cannot be greater than old priority";
    }
    else{
        throw "Error: The given value is not found in this priority queue";
    }
}

void LinkedPriorityQueue::clear() {
    while(front != NULL){
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

string LinkedPriorityQueue::dequeue() {
    checkEmpty();
    ListNode* temp = front;
    string val = temp->value;

    front = front->next;
    delete temp;
    return val;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* elem = new ListNode(value, priority);

    // Inserts at front of list
    if(front == NULL || *elem < *front){
        elem->next = front;
        front = elem;
    }
    // Inserts later in list
    else{
        ListNode* curr = front;
        while(curr->next != NULL && *(curr->next) < *elem){
            curr = curr->next;
        }
        elem->next = curr->next;
        curr->next = elem;
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

string LinkedPriorityQueue::peek() const {
    checkEmpty();
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    checkEmpty();
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* curr = front;
    while(curr != NULL){
        size++;
        curr = curr->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* curr = queue.front;
    out << "{";

    while(curr != NULL){
        out << *curr;
        if(curr->next != NULL){
            out << ", ";
        }
        curr = curr->next;
    }

    out << "}";
    return out;
}

void LinkedPriorityQueue::checkEmpty() const {
    if(isEmpty()){
        throw "Error: priority queue is empty";
    }
}
