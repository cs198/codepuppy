/**
 * Name: Milan Doshi
 * Assignment: LinkedPriorityQueue.cpp
 * TA: Jimmy Lee
 *
 * This file uses a Linked List to input, store, and organize information. Actions are taken on the group of data through the use of pointers. The linked list version is not as
 * efficient as the heap implementation.
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    LinkedPriorityQueue::clear();
}

//Big-Oh: N
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* previous = front;
    if (previous->value == value) {
        if(newPriority > previous->priority) {
            throw "The priority must be higher than previously.";
        } else {
            previous->priority = newPriority;
        }
    } else {
        while(previous->next != NULL && previous->next->value != value) {
            previous = previous->next;
        }
        if(previous->next == NULL && previous->next->value != value) throw "No such value exists in the queue.";
        if(newPriority > previous->priority) throw "The priority must be higher than previously.";
        ListNode* temp = previous->next->next;
        delete previous->next;
        previous->next = temp;
        enqueue(value,newPriority);
    }
}

//Big-Oh: N
void LinkedPriorityQueue::clear() {
    while(front != NULL) {
        ListNode* current = front;
        front = front -> next;
        delete current;
    }
}

//Big-Oh: 1
string LinkedPriorityQueue::dequeue() {
    if(front == NULL) throw "The queue is empty.";
    string value =  front->value;
    ListNode* temp = front;
    front = front->next;
    delete temp;
    return value;
}

//Big-Oh: N
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* current = front;
    if(current == NULL) {
        ListNode* newListNode = new ListNode(value, priority);
        current = newListNode;
        front = current;
        cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
    } else if(priority < current->priority) {
            ListNode* temp = front;
            front = new ListNode(value,priority,temp);
            cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
    } else if(current->priority >= priority) {
        if(current->priority == priority) {
            if(value < current -> value) {
                ListNode* newListNode = new ListNode(value, priority, current);
                current = newListNode;
                front = current;
                cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
            }
            else {
                ListNode* newListNode = new ListNode(value, priority, current->next);
                front->next = newListNode;
                cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
            }
        }
    } else {
        while(current->next != NULL && current->next->priority <= priority) {
            if(current -> next -> priority == priority) {
                if(value > current -> next -> value) {
                   current = current->next;
                } else {
                    break;
                }
            } else {
                current = current -> next;
            }
        }
        ListNode* temp = current->next;
        current->next = new ListNode(value, priority, temp);
        cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
    }
}

//Big-Oh: 1
bool LinkedPriorityQueue::isEmpty() const {
    if(front == NULL) {
       return true;
    } else {
       return false;
    }
}

//Big-Oh: 1
string LinkedPriorityQueue::peek() const {
    if(front == NULL) throw "The queue is empty.";
    return front->value;
}

//Big-Oh: 1
int LinkedPriorityQueue::peekPriority() const {
    if(front == NULL) throw "The queue is empty.";
    return front->priority;
}

//Big-Oh: N
int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* current = front;
    while(current != NULL) {
        current = current->next;
        size++;
    }
    return size;
}

//Big-Oh: N
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    out << "{";
    while(current != NULL) {
        if(current -> next == NULL) {
            out << "\"";
            out << current->value;
            out << "\"";
            out << ":";
            out << current->priority;
        } else {
            out << "\"";
            out << current->value;
            out << "\"";
            out << ":";
            out << current->priority;
            out << ", ";
        }
        current = current -> next;
     }
     out << "}";
     return out;
}
