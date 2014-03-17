// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "LinkedPriorityQueue.h"

//constructor.
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = new ListNode;
    front = NULL;
}

//destructor. Deletes all linkedNodes.
LinkedPriorityQueue::~LinkedPriorityQueue() {
    while(true) {
        if (isEmpty()) break;
        else {
            ListNode* temp = front->next;
            delete(front);
            front = temp;
        }
    }
}

/* Changes priority of a given value IF newPriority is more urgent than the value's old priority.
 * If the given value is present in the queue and already has a more
 * urgent priority to the given new priority, or if the given value is not already in the
 * queue, the function throws a string exception.*/
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (isEmpty()) {
        throw "Error: priority queue is empty"; //string exception
    }
    else {
        ListNode* current = front;
        bool valueFound = false;
        bool priorityChanged = false;
        while (current->next != NULL) {
            if (current->next->value == value) {
                valueFound = true;
            }
            if (current->next != NULL && current->next->value == value) {
                if (current->next->priority > newPriority) {
                    ListNode* temp = current->next->next;
                    delete(current->next->next);
                    current->next = temp;
                    enqueue(value, newPriority);
                    priorityChanged = true;
                    break;
                }
                if (current->next->priority < newPriority) {
                    current = current->next;
                }
            }
            current = current->next;
        }
        if (current->value == value && priorityChanged == false) { //checks for last link in node (edge case)
            valueFound = true;
            if (current->priority > newPriority) {
                current->priority = newPriority;
            }
        }
        if (valueFound == false) {
            throw "Error: The given value is not found in this priority queue";
        }
        if (valueFound == true && priorityChanged == false) {
            throw "Error: new priority cannot be greater than old priority";
        }
    }
}

//clears priority queue.
void LinkedPriorityQueue::clear() {
    if (isEmpty()) {
        throw "Queue is already empty.";
    }
    while (front->next != NULL) {
        ListNode* temp = front;
        front = front->next;
        delete(temp);
    }
    front = NULL;
}

//Removes and returns most urgent element from the queue. Throws a string exception if the queue
//does not contain any elements.
string LinkedPriorityQueue::dequeue() {
    if (isEmpty()) throw "Error: priority queue is empty";
    else {
        string dequeuedElement = front->value;
        ListNode* temp = front->next;
        delete(front);
        front = temp;
        return dequeuedElement;
    }
}

//adds an entry to the priority queue.
void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (isEmpty()) {
        front = new ListNode(value, priority, NULL);
    }
    else {
        ListNode* current = front;
        while(current->next != NULL && (current->next->priority < priority || (current->next->priority == priority && current->next->value <value)) ) {
            current = current->next;
        }
        if (current == front && current->priority >= priority) {// || current->priority == priority) {
            ListNode* newNode = new ListNode(value, priority, front);
            front= newNode;
        }
        else {
            if (current -> next == NULL) {
                ListNode* newNode = new ListNode(value, priority, NULL);
                current->next = newNode;
            }
            else if (current->next->priority > priority || (current->next->priority == priority && current->next->value >= value)) {
                ListNode* newNode = new ListNode(value, priority, current->next);
                current->next = newNode;
            }
        }
    }
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

//returns true if queue is empty, returns false if not.
bool LinkedPriorityQueue::isEmpty() const {
    if (front == NULL) {
        return true;
    }
    else return false;
}

//returns the string element with the highest priority. Throws string exception is queue is empty.
string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "Error: priority queue is empty";
    }
    else {
        ListNode* current = front;
        string highestPriorityString = current->value;
        int highestPriority = current->priority;
        while(current->next != NULL) {
            if (current->next->priority < highestPriority) {
                highestPriorityString = current->next->value;
                highestPriority = current->next->priority;
            }
            current = current->next;
        }
        if (current->priority < highestPriority) { //checks last node in linkedlist (edge case)
            highestPriorityString = current->next->value;
        }
        return highestPriorityString;
    }
}

//returns the most urgent integer priority. Throws string exception is queue is empty.
int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Error: priority queue is empty";
    }
    else {
        ListNode* current = front;
        int highestPriority = current->priority;
        while(current->next != NULL) {
            if (current->next->priority < highestPriority) {
                highestPriority = current->next->priority;
            }
            current = current->next;
        }
        if (current->priority < highestPriority) { //checks last node in linkedlist (edge case)
            highestPriority = current->next->priority;
        }
        return highestPriority;
    }
}


//returns the number of elements in the array.
int LinkedPriorityQueue::size() const {
    int size = 0;
    if (!isEmpty()) {
        ListNode* current = front;
        while(current->next != NULL) {
            size++;
            current = current->next;
        }
        size++;
    }
    return size;
}

//prints priority queue to console.
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    if (queue.front != NULL) {
        ListNode* current = queue.front;
        if (current->next == NULL) {
            out << *current;
        }
        else {
            while(current->next != NULL) {
                out << *current << " ,";
                current = current->next;
            }
            out << *current;
        }
    }
    out << "}";
    return out;
}
