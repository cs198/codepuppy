// Pros of linkedlist are the entries are sorted, therefore, when dequeueing, just return the front entry.
// As a result, the process of enqueueing an entry is more complited, as it has to marching along the list to find the right position to insert.s

#include "LinkedPriorityQueue.h"
#include "PQEntry.h"
#include "strlib.h"
#include "ListNode.h"
#include "PQEntry.h"

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
LinkedPriorityQueue::LinkedPriorityQueue() {
    front  = NULL;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (front == NULL) {
        throw string ("Empty queue!");
    } else if (front->value == value) {
        if (front->priority <= newPriority) {
            throw string ("Use higher priority!");
        } else {
            ListNode* trash = front;
            front = new ListNode(value, newPriority);
            front->next = trash->next;
            delete trash;
        }
    } else {
        ListNode* current = front;
        while (current->next != NULL) {
            if (current->next->value == value) {
                if (current->next->priority <= newPriority) {
                    throw string ("Use higher priority!!");
                } else {
                    ListNode* trash = current->next;
                    current->next = current->next->next;
                    delete trash;
                    enqueue(value, newPriority);
                    return;
                }
            }
            current = current->next;
        }
        if (current->next == NULL) {
            throw string ("Can not found such value!!");
        }

    }
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw "Empty queue!";
    } else {
        string result = front->value;
        ListNode* trash = front;
        front = front->next;
        delete trash;
        return result;
    }
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (front == NULL) {
        front =  new ListNode(value, priority);
    } else {
        ListNode* enqueueNode = new ListNode(value, priority, NULL);
        ListNode* current = front;
        //while (current < enqueueNode && current->next < enqueueNode && current->next != NULL) {
        while (current->next != NULL && (current->next->priority < enqueueNode->priority || (current->next->priority == enqueueNode->priority && current->next->value < enqueueNode->value))) {
            current = current->next;
        }
        //if (current == front && front > enqueueNode) {
        if ((enqueueNode->priority < front->priority) || (enqueueNode->priority == front->priority && enqueueNode->value < front->value) && (front == current)) {
            front = enqueueNode;
            front->next = current;
        } else {
            ListNode* temp = current->next;
            current->next = enqueueNode;
            current->next->next = temp;
        }
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
bool LinkedPriorityQueue::isEmpty() const {
    return (front == NULL);
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
string LinkedPriorityQueue::peek() const {
    if (front == NULL) {
        throw string ("Empty queue!");
    } else {
        return (front->value);
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) {
        throw string ("Empty queue!");
    } else {
        return (front->priority);
    }

}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
int LinkedPriorityQueue::size() const {
    ListNode* current = front;
    int count = 0;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
string LinkedPriorityQueue::toString() const{
    string result = "{";
    ListNode* current = front;
    while (current != NULL) {
        result += ("\"" + current->value + "\":" + integerToString(current->priority));
        if (current->next != NULL) {
            result += ", ";
        }
        current = current->next;
    }
    result += "}";
    return result;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    string output = queue.toString();
    out << output;
    return out;
}
