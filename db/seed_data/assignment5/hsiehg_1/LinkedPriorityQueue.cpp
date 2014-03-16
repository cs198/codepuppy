// LinkedPriorityQueue.h
// This file contains the implementation of the LinkedPriorityQueue structure.

#include "LinkedPriorityQueue.h"

/*
 * Constructor for LinkedPriorityQueue that sets front to point to NULL.
 */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

/*
 * Deconstructor for LinkedPriorityQueue.
 * Calls the clear() function to delete all the nodes in the Linked List Priority Queue.
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();

}

/*
 * Changes the priority of a specific value in the queue.
 * Temp points to the node that should be reinserted based on the new priority. If temp is NULL, the function will throw an error since it means that
 * the new priority is greater than the old priority.
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(front == NULL) {
        throw "Error: priority queue is empty";
    } else {
        ListNode* temp = changedNode(value, newPriority);
        if(temp != NULL) {
            enqueue(temp->value, temp->priority);
            delete temp;
        } else {
            throw "Error: new priority cannot be greater than old priority";
        }
    }

}

/*
 * Returns a pointer to the node that needs to be reinserted after its priority is changed.
 * Temp is only changed if the new priority is less than the old priority (otherwise it remains NULL).
 * The function locates the element that needs to be changed by checking the front, middle and end of the queue.
 *
 */
ListNode* LinkedPriorityQueue::changedNode(string value, int newPriority) {
    ListNode* temp = NULL;
    if(front->value == value && front->priority >= newPriority) {
        changeFront(temp, newPriority);
    } else {
        ListNode* cur = front;
        bool isMiddleCase = changeMiddle(temp, cur, value, newPriority);
        if(!isMiddleCase) {
            changeEnd(temp, cur, value, newPriority);
        }
    }
    return temp;
}

/*
 * Sets temp to point to the last element of the queue with an altered priority if the new priority is less than the old priority.
 * The pre-conditions for this function is that the element was not located in the front or middle part of the queue.
 */
void LinkedPriorityQueue::changeEnd(ListNode*& temp, ListNode* cur, string value, int newPriority) {
    if(cur->next->next == NULL && cur->next->value == value && cur->next->priority > newPriority) {
        cur->next->priority = newPriority;
        temp = cur->next;
        cur->next = NULL;
    }
}

/*
 * Returns true if a matching element was found, otherwise returns false.
 * Sets temp to point one of the middle elements of the queue with an altered priority if the new priority is less than the old priority.
 * The pre-conditions for this function is that the element was not located in the front of the queue.
 */
bool LinkedPriorityQueue::changeMiddle(ListNode*& temp, ListNode*& cur, string value, int newPriority) {
    bool isFound = false;
    while (cur->next != NULL && cur->next->next != NULL) {
        if(cur->next->value == value) {
            if (cur->next->priority >= newPriority) {
                cur->next->priority = newPriority;
                temp = cur->next;
                cur->next = cur->next->next;
                isFound = true;
            }
            break;
        }
        cur = cur->next;
    }
    return isFound;
}

/*
 * Sets temp to point to the first element of the queue with an altered priority if the new priority is less than the old priority.
 */
void LinkedPriorityQueue::changeFront(ListNode*& temp, int newPriority) {
    front->priority = newPriority;
    temp = front;
    front = front->next;
}

/*
 * Removes all elements from the priority queue.
 */
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

/*
 * Removes the element with the most urgent priority from the queue.
 * This function throws a string exception if the queue is empty.
 */
string LinkedPriorityQueue::dequeue() {
   if (front == NULL){
        throw "Error: priority queue is empty";
   } else {
       ListNode* trash = front;
       string val = trash->value;
       delete trash;
       front = front->next;
       return val;
   }
}

/*
 * Adds the given string value into the priority queue with the given priority.
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    if(front == NULL) {
        front = new ListNode(value, priority, NULL);
    } else if (ListNode(value, priority) <= *front) {
        front = new ListNode(value, priority, front);
    } else {
        addToQueue(value, priority);
    }
}

/*
 * Enqueues an element to the middle or end of the queue if an element that is less than or equal to the new element exists.
 */
void LinkedPriorityQueue::addToQueue(string value, int priority) {
    ListNode* cur = front;
    while (cur->next != NULL) {
        if (ListNode(value, priority) <= *cur->next) {
            cur->next = new ListNode(value, priority, cur->next);
            break;
        }
        cur = cur->next;
    }
    if(cur->next == NULL) {
        cur->next = new ListNode(value, priority);
    }
}

/*
 * Returns true if the priority queue has no elements.
 */
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

/*
 * Returns the string element with the most urgent priority.
 */
string LinkedPriorityQueue::peek() const {
    if (front == NULL) {
        throw "Error: priority queue is empty";
    }
    return front->value;
}

/*
 * Returns the integer priority that is most urgent.
 */
int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) {
        throw "Error: priority queue is empty";
    }
    return front->priority;
}

/*
 * Returns the size of the priority queue.
 */
int LinkedPriorityQueue::size() const {
    if (front == NULL) {
        return 0;
    } else {
        int count = 0;
        ListNode* cur = front;
        while (cur != NULL) {
            count++;
            cur = cur->next;
        }
        return count;
    }
}

/*
 * Overrides the << operator so that the priority queue can be printed to the console.
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    if(queue.front != NULL) {
        ListNode* cur = queue.front;
        out << *cur;
        cur = cur->next;
        while(cur != NULL) {
            out << ", " << *cur;
            cur = cur->next;
        }
    }
    out << "}";
    return out;
}
