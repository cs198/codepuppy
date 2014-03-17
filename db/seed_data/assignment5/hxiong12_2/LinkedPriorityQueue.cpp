/*
 * This file implements the LinkedPriorityQueue.h interface.
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
    delete front;
}

//basically the same as enqueue, only it does not print out the "enqueued" message and it inputs a ListNode
void LinkedPriorityQueue::searchInsert(ListNode*& moved) {
    ListNode* current = front;
    if (*moved < *front) { //if changed node needs to be entered at front of list
        moved->next = front;
        front = moved;
        return;
    }
    while (current->next != NULL) { //if changed node needs to be inserted in the middle of the list
        if (*current->next > *moved) {
            moved->next = current->next;
            current->next = moved;
            return;
        }
        current = current->next; //moves forward
    }
    current->next = moved; //if changed node remains at the end of the list, since it hasn't been inserted yet
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (front == NULL) { //if list is empty
        throw "Empty queue, cannot change elements.";
    } else {
        ListNode* current = front;
        if (current->value == value) { //if element to be changed is at the front of the queue
            if (current->priority < newPriority) throw "Error: new priority cannot be greater than old priority";
            current->priority = newPriority;
            return;
        }
        while (current->next != NULL) { //if element to be changed is later in the list
            if (current->next->value == value) {
                if (current->next->priority < newPriority) throw "Error: new priority cannot be greater than old priority";
                current->next->priority = newPriority;
                ListNode* moved = current->next;
                current->next = current->next->next; //removes the "moved" node from the chain
                searchInsert(moved); //goes back to front of list to search for where to insert the changed node
                return;
            }
            current = current->next;
        }
    }
}

void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* insert = new ListNode(value, priority, NULL);
    //if list is empty
    if (front == NULL) {
        front = insert;
        cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
        return;
    }
    ListNode* current = front;
    //if new element needs to be inserted at front of list
    if (*insert < *front) {
        insert->next = front;
        front = insert;
        cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
        return;
    }
    //if there are multiple elements
    while (current->next != NULL) {
        if (*insert < *current->next) {
            insert->next = current->next;
            current->next = insert;
            cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
            return;
        } else {
            current = current->next;
        }
    }
    //we reach end of list and still have not inserted the element, i.e. new element is the last item on the list
    current->next = insert;
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw "Error: priority queue is empty";
    } else {
        ListNode* trash = front;
        front = front->next;
        string s = trash->value;
        delete trash;
        return s;
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    if (front == NULL) return true;
    return false;
}

string LinkedPriorityQueue::peek() const {
    if (front == NULL) throw "Error: priority queue is empty";
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) throw "Error: priority queue is empty";
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    if (front == NULL) return size; //empty list
    ListNode* current = front;
    while (current != NULL) { //non-empty list
        size++; //add 1
        current = current->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    out << "{";
    while (current != NULL) {
        out << *current;
        if (current->next != NULL) out << ", ";
        current = current->next;
    }
    out << "}";
    return out;
}
