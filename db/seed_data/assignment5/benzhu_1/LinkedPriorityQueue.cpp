//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    bool flag = true;
    ListNode* current = front;
    // find the node with the same value
    if (front == NULL) {
        throw "exception";
    } else {
        while (current->next != NULL) {
            if (current->next->value == value && current->next->priority > newPriority) {
                flag = false; // found the node with the same value and lower priority
                break;
            } else {
                current = current->next;
            }
        }
    }
    //remove it
    ListNode* trash = current->next;
    current->next =  current->next->next;
    delete trash;
    //enqueue
    enqueue(value, newPriority);

    if(flag) {
        throw "exception";
    }
}

void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw "exception";
    } else {
        ListNode* temp = front;
        front = front->next;
        return temp->value;
    }
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (front == NULL) { //empty list
        ListNode* newNode = new ListNode(value, priority);
        front = newNode;
    } else if ((front->priority > priority)||
               (front->priority == priority && value < front->value)) { //needs to insert before the first node
        ListNode* newNode = new ListNode(value, priority);
        newNode->next = front;
        front = newNode;
    } else {
        ListNode* current = front;
        while (current->next != NULL) {
            if ((current->priority < priority||(current->priority == priority && value > current->value))&&
                    (current->next->priority > priority||(current->next->priority == priority && value < current->value))){
                break;
            } else {
                current = current->next;
            }
        }

        ListNode* newNode = new ListNode(value, priority);
        newNode->next = current->next;
        current->next = newNode;
    }
}


bool LinkedPriorityQueue::isEmpty() const {
    return front==NULL;
}

string LinkedPriorityQueue::peek() const {
    if (front == NULL) {
        throw "exception";
    } else {
        return front->value;
    }
}

int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) {
        throw "exception";
    } else {
        return front->priority;
    }
}

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
    out<<"{";
    if (queue.front != NULL) {
        ListNode* current = queue.front;
        while (current->next != NULL) {
            out<<*current<<", ";
            current = current->next;
        }
        out<<*current;
    }
    out<<"}";
    return out;
}
