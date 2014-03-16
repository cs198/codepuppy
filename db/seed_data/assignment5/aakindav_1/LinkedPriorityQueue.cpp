// LinkedPriorityQueue.cpp implements the LinkedPriorityQueue
// class behavior declared in LinkedPriorityQueue.h
// See LinkedPriorityQueue.h for documentation of each member

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (front != NULL) {
        ListNode* current = front;
        ListNode* prev = NULL;
        bool flag = false;
        while (current != NULL) {
            if (current->value == value) {
                if (current->priority > newPriority) {
                    current->priority = newPriority;
                    if (prev != NULL) prev->next = current->next;
                    enqueue(current->value, current->priority);
                    delete current;
                    flag = true;
                }
                else throw "Must upgrade priority!";
            }
            prev = current;
            current = current->next;
        }
        if (!flag) throw "Value is not contained in Priority Queue!";
    }
    else throw "Empty Priority Queue!";
}

void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

string LinkedPriorityQueue::dequeue() {
    if (front != NULL) {
        ListNode* current = front;
        front = front->next;
        string val = current->value;
        delete current;
        return val;
    } else throw "Empty Priority Queue!";
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* node = new ListNode (value, priority);
    if (front == NULL) {
        front = node;
    }
    else {
        ListNode* current = front;
        ListNode* prev = NULL;
        bool flag = true;
        while (current != NULL && node->priority >= current->priority) {
            if (node->priority == current->priority) {
                if (node->value > current->value) {
                    flag = false;
                    break;
                }
            }
            prev = current;
            current = current->next;
        }
        if (flag) {
            node->next = current;
            if (prev == NULL) front = node;
            else prev->next = node;
        } else {
            node->next = current->next;
            current->next = node;
        }
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

string LinkedPriorityQueue::peek() const {
    if (front!=NULL) return front->value;
    else throw "Empty Priority Queue";
}

int LinkedPriorityQueue::peekPriority() const {
    if (front!=NULL) return front->priority;
    else throw "Empty Priority Queue";
}

int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
}

ListNode* LinkedPriorityQueue::getFront() const{
    return front;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* current = queue.getFront();
    while (current != NULL) {
        out << *current;
        if (current->next != NULL) out << ", ";
        current = current->next;
    }
    out << "}";
    return out;
}
