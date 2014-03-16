/* File: LinkedPriorityQueue.cpp
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * ---------------------------------
 * The implemntation for the LinkedPriorityQueue class. This class
 * uses a LinkedList to store the elements of the priority queue. This means
 * that enqueueing can be complicated because the list is in sorted order but dequeueing
 * is easy because the most urgent element is at the front of the list.
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    ListNode* current = front;
    ListNode* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        delete temp;
    }
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    ListNode* previous = front;
    while (current != NULL) {
        if (current->value == value) {
            break;
        }
        if (current != front) {
            previous = previous->next;
        }
        current = current->next;
    }
    if (current == NULL) {
        throw "The given value is not in the queue.";
    }
    if (current->priority < newPriority) {
        throw "The current priority is more urgent.";
    }
    if (previous == front) {
        front = current->next;
    } else {
        previous->next = current->next;
    }
    delete current;
    enqueue(value,newPriority);

}

void LinkedPriorityQueue::clear() {
    ListNode* current = front;
    ListNode*next;
    while (current != NULL) {
        next = current->next;
        delete current;
        current = next;
    }
    delete current;
    front = NULL;
}

string LinkedPriorityQueue::dequeue() {
    if (front==NULL) {
        throw "Error: The queue is empty.";
    }
    string value = front->value;
    ListNode* temp = front->next;
    delete front;
    front = temp;
    return value;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* current = front;
    ListNode*previous = front;
    ListNode* newNode = new ListNode(value,priority);
    while (current != NULL) {
        if ((current->priority > priority) || (current->priority == priority && value < current->value)) {
            break;
        }
        if (current != front) {
            previous = previous->next;
        }
        current = current->next;
    }
    if (current == front) {
        newNode->next = front;
        front = newNode;
    } else {
        previous->next = newNode;
        newNode->next = current;
    }

}

bool LinkedPriorityQueue::isEmpty() const {
    return front==NULL;
}

string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "Error: The queue is empty";
    } else {
        return front->value;
    }
}

int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Error: The queue is empty";
    } else {
        return front->priority;
    }
}

int LinkedPriorityQueue::size() const {
    ListNode* current = front;
    int count = 0;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    out << "{";
    while (current != NULL) {
        out << *current;
        if (current->next != NULL) {
            out << ", ";
        }
        current = current->next;
    }
    out << "}";
    return out;
}
