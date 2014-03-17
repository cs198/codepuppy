/*
 * CS106B Assignment #5: Priority Queue - Linked Priority Queue
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This class implements a priority queue where the data is stored in a linked list. The enqueuing time is slower
 * because it is sorted, but dequeuing is fast, leading to better overall performance over Vector priority queue.
 *
 * See VectorPriorityQueue.h for documentation of member functions
 */

#include "LinkedPriorityQueue.h"

/*
 * This method constructs a LinkedPriorityQueue object
 */
LinkedPriorityQueue::LinkedPriorityQueue() {

}
/*
 * This object destructs a LinkedPriorityQueue object
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}


void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    while (current->next != NULL) {
        if (current->next->value == value) {
            if (current->next->priority > newPriority) {
                ListNode* change = current->next;
                current->next = change->next;
                enqueue(value, newPriority);
                delete change;
                return;
            } else {
                throw "";
            }

        }
        current = current->next;
    }
    throw "";
}


void LinkedPriorityQueue::clear() {
    ListNode* current = front;
    while (current != NULL) {
        ListNode* trash = current;
        current = current->next;
        delete trash;
    }
}


string LinkedPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "";
    }
    string name = front->value;
    ListNode* trash = front;
    front = front->next;
    delete trash;
    return name;
}


void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* current = front;
    if (isEmpty()) {
        front = new ListNode(value, priority, NULL);
    } else {
        while (current->next != NULL && priority > current->next->priority) {
            current = current->next;
        }
        if (current->next == NULL) {
            if (current->priority > priority) {
                current->next = new ListNode(value, priority, NULL);
            } else if (current->priority < priority) {
                current->next = new ListNode(value, priority, NULL);
            } else {
                if (current->value < value) {
                   current->next = new ListNode(value, priority, NULL);
                } else {
                   front = new ListNode(value, priority, current); //check
                }
            }

        } else {
            if (current->priority < priority && current->next->priority > priority) {
                ListNode* temp = current->next;
                current->next = new ListNode(value, priority, temp);
            } else if (current->next->priority == priority) {
                if (current->next->value > value) {
                    ListNode* temp = current->next;
                    current->next = new ListNode(value, priority, temp);
                } else {
                    current = current->next;
                    while (current->next != NULL && current->next->priority == priority
                           && current->next->value < value) {
                        current = current->next;
                    }
                    ListNode* temp = current->next;
                    current->next = new ListNode(value, priority, temp);
                }
            } else if (current->priority >= priority && current == front){
                front = new ListNode(value, priority, current);
            }
        }
    }
}


bool LinkedPriorityQueue::isEmpty() const {
    return (size() == 0);
}


string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "";
    }
    string name = front->value;
    return name;
}


int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "";
    }
    int urgent = front->priority;
    return urgent;
}


int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}


ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    if (queue.isEmpty()) {
        out << "{}";
    } else {
        out << "{" << *current;
        current = current->next;
        while (current != NULL) {
            out << ", " << *current;
            current = current->next;
        }
        out << "}";
    }

    return out;
}
