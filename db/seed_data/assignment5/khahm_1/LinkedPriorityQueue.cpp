/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Linked Priority Queue cpp file
 * Stores its elements in an sorted (by priority) linked list of PQEntries
 * Enqueuing and size are inefficient, and but other methods are efficient.
 * Therefore, this is more efficient than vector priority queue,
 * but is still not the most efficient form.
 */

#include "LinkedPriorityQueue.h"
#include "ListNode.h"

// constructor
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}
// destructor
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

/*
 * Runs in O(N^2) time
 * parameter: string value of element to be changed, int new priority
 * changes the first occurance of the indicated value to the new priority
 * throws a string exception if the new priority is greater than or equal
 * to the old priority, or if the value doesn't exist in the queue.
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    // checks first element
    if (current->value == value) {
        if (current->priority <= newPriority) {
            throw value;
        }
        front = current->next;
        enqueueHelper(value, newPriority);
        return;
    }
    // checks rest of the elements
    while (current != NULL && current->next != NULL) {
        if (current->next->value == value) {
            if (current->next->priority <= newPriority) {
                throw value;
            }
            current->next = current->next->next;
            enqueueHelper(value, newPriority);
            return;
        }
        current = current->next;
    }
    throw value;
}

// Runs in O(N) time
// Removes all elements from the queue
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

/*
 * Runs in O(1) time
 * Removes the element with the most urgent priorty
 * returns: value of element with the most urgent priorty that was removed
 * throws a string exception if the queue is empty
 */
string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw "List is empty";
    }
    ListNode* trash = front;
    string result = trash->value;
    front = front->next;
    delete trash;
    return result;
}

/*
 * Runs in O(N) time
 * parameter: new string value, new priority
 * adds the new entry to the end of the queue
 * precondition: parameters are all valid
 * duplicates are also added
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    enqueueHelper(value, priority);
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

/*
 * helper function
 * Runs in O(N) time
 * parameter: new string value, new priority
 * adds the new entry to the end of the queue
 * precondition: parameters are all valid
 * duplicates are also added
 * Only purpose is to differentiate the cout statement from enqueue versus change priority
 */
void LinkedPriorityQueue::enqueueHelper(string& value, int priority) {
    ListNode* newNode = new ListNode(value, priority);
    // empty list
    if (front == NULL) {
        front = newNode;
    }
    // new entry is at the beginning
    else if (front->priority > priority || (front->priority == priority && front->value > value)) {
        ListNode* temp = front;
        front = newNode;
        newNode->next = temp;
    }
    // other locations
    else {
        ListNode* prev = front;
        while ((prev->next != NULL) && (prev->next->priority < priority)) {
            prev = prev->next;
        }
        while ((prev->next != NULL) && (prev->next->priority == priority) && (prev->next->value < value)) {
            prev = prev->next;
        }
        ListNode* temp = prev->next;
        prev->next = newNode;
        newNode->next = temp;
    }
}

// Runs in O(1) time
// returns: true if queue is empty, false if it is not empty
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

/*
 * Runs in O(1) time
 * returns: string value of the element with the highest urgency
 * throws a string exception if queue is empty
 */
string LinkedPriorityQueue::peek() const {
    if (front == NULL) {
        throw "List is empty";
    }
    return front->value;
}

/*
 * Runs in O(1) time
 * returns: int priority of the element with the highest urgency
 * throws a string exception if queue is empty
 */
int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) {
        throw "List is empty";
    }
    return front->priority;
}

// Runs in O(N) time
// returns: size of queue
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* temp = front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// helper function
// Runs in O(1) time
// this function is a getter function for the front pointer
ListNode* LinkedPriorityQueue::getFront() const {
    return front;
}

/*
 * Runs in O(N) time
 * printing operator overload
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* current = queue.getFront();
    if (current!= NULL) {
        while (current->next != NULL) {
            out << *current << ", ";
            current = current->next;
        }
        out << *current;
    }
    out << "}";
    return out;
}
