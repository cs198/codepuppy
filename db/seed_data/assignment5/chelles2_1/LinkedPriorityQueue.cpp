/*This .cpp file implements a priority queue using a linked list. A priority queue is a sorted queue
where the elements are sorted by priority, breaking ties by comparing the string elements themselves.*/

#include "LinkedPriorityQueue.h"

/* Constructor. Sets the front of the linked list to null. */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

/* Destructor. Calls the helper function and passes the front of the linked list */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    // TODO: implement
    destructorHelper(front);
}

/* Destructor helper function. Recursively deletes each node until none is left */
void LinkedPriorityQueue::destructorHelper(ListNode* node) {
    if (node == NULL) return;
    destructorHelper(node -> next);
    delete node;
}

/* Changes the priority value of the given string to the given integer value. */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    bool found = false;
    //Follow the linked list until null
    while (current -> next != NULL) {

        //If the desired value is found
        if (current -> value == value) {
            //Set the priority to the new value and break
            current -> priority = newPriority;
            found = true;
            break;
        }

        //Keep searching
        current = current -> next;
    }

    //End case: if the desired value is at the end of the linked list, update priority value
    if (current -> value == value) current -> priority = newPriority;

    //If the desired value is not found, throw string exception
    if (!found) {
        throw ("The given value is not in the queue.");
    }
}

/* Removes all elements from the priority queue */
void LinkedPriorityQueue::clear() {
    ListNode* current = front;
    //Loops through the linked list to delete all elements
    while (current != NULL) {
        front = front -> next;
        delete current;
        current = front;
    }
    delete front;
}

/* Removes the element with the most urgent priority from priority queue, and returns its value */
string LinkedPriorityQueue::dequeue() {

    //String exception if empty queue
    if (isEmpty()) {
        throw ("The queue is empty.");
    }

    //Removes and returns the first entry from the linked list
    PQEntry pq = front -> value;
    front = front -> next;
    return pq.value;
}

/* Adds the given string value into priority queue with the given priority */
void LinkedPriorityQueue::enqueue(string value, int priority) {

    //Create a list node with the given value and priority
    ListNode* entry = new ListNode(value, priority);

    //If the queue is empty, add the node to the front
    if (isEmpty()) {
        front = entry;
    } else {
        ListNode* current = front;
        if (*current > *entry) {
            //If the entry has the most urgent priority, put it at the front of the list
            //and link it the rest of the list
            entry -> next = front;
            front = entry;
        } else {

            //Go through list until the priority & value are less than those of our desired entry
            while (current -> next != NULL && *(current -> next) < *entry) {
                current = current -> next;
            }

            //Then rearrange the pointers to put entry between the sorted priorities
            entry->next = current->next;
            current->next = entry;
        }
    }
}

/* Returns true if queue is empty, false otherwise */
bool LinkedPriorityQueue::isEmpty() const {
    return (front == NULL);
}

/* Returns the string element with the most urgent priority without
 * removing it or altering the state of the queue*/
string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        //String exception if empty queue
        throw ("The queue is empty.");
    }

    //Returns the first node's value from the linked list
    return front -> value;
}

/* Returns the integer value of the most urgent priority without
 * removing it or altering the state of the queue*/
int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        //String exception if empty queue
        throw ("The queue is empty.");
    }

    //Returns the first node's priority from the linked list
    return front -> priority;
}

/* Returns the size of the queue */
int LinkedPriorityQueue::size() const {
    int counter = 0;
    ListNode* current = front;

    //Loop through list and increment counter
    if (current != NULL) {
        while (current -> next != NULL) {
            counter++;
            current = current -> next;
        }
        counter++;
    }

    //Return the counter
    return counter;
}

/* << operator for printing priority queue to the console */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* current = queue.front;

    //Go through linked list and print the value and priority
    while (current != NULL) {
        out << current->value << ":" << current->priority;
        current = current->next;
        if (current != NULL) out << ", ";
    }
    out << "}";

    return out;
}
