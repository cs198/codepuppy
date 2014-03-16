// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "LinkedPriorityQueue.h"

/*
 * Creates a new LinkedPriorityQueue, initializing
 * the front as NULL.
 * O(1)
 */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}


/*
 * Deletes the queue.
 * O(1)
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}


/*
 * Checks to see if the queue is empty. If the value is the
 * first value, it checks and then takes the entry out of the
 * front and sends it to get enqueued; if not, it looks through
 * the list until it finds it, ties the strings together, and
 * again sends it to get enqueued.
 * O(N)
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (front == NULL) throw "exception";
    ListNode* current = front;
    if (current->value == value) {
        ListNode* trash = current;
        front = front->next;
        delete trash;
        LinkedPriorityQueue::enqueue(value,newPriority);
    }
    else {
        while ((front->next != NULL) && (current->next->value != value)) {
            current = current->next;

        }
        if (current->next->value == value) {
            ListNode* trash = current->next;
            current->next = current->next->next;
            delete trash;
            LinkedPriorityQueue::enqueue(value,newPriority);
        }
    }
}


/*
 * Deletes the list and all its trash.
 * O(1)
 */
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}


/*
 * Removes the first value from the list.
 * O(1)
 */
string LinkedPriorityQueue::dequeue() {
    ListNode* current = front;
    string s = current->value;
    ListNode* trash = current;
    front = front->next;
    delete trash;
    return s;
}


/*
 * Enqueues a value into the list. If the list is empty,
 * it just adds it and creates a new list; if the value
 * should go in front, it makes the front be the newNode
 * and adds it to the rest of the list; if there is only
 * one other value, then it adds it to the end of that value;
 * finally, if it is none of these cases, it looks through
 * until it finds the appropriate place to insert the entry.
 * O(N)
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newNode = new ListNode(value,priority,NULL);
    if (front == NULL) {
        front = newNode;
        front->next = NULL;
    } else if (priority == 0 || priority < front->priority) {
        newNode->next = front;
        front = newNode;
    }
    else if (front->priority < priority && front->next == NULL){
        front->next = newNode;
        newNode->next = NULL;
    } else {
        ListNode* current = front->next;
        ListNode* previous = front;
        while (current->priority <= priority && current->next != NULL) {
            previous = current;
            current = current->next;
        }
        newNode->next = current;
        previous->next = newNode;
        if (previous == front)
            front->next = newNode;
    }

}


/*
 * Checks to see if the queue is empty.
 * O(1)
 */
bool LinkedPriorityQueue::isEmpty() const {
    if (front == NULL) return true;
    return false;
}


/*
 * Outputs the string of the first
 * value in the queue.
 * O(1)
 */
string LinkedPriorityQueue::peek() const {
    return front->value;
}


/*
 * Outputs the priority of the first
 * value in the queue.
 * O(1)
 */
int LinkedPriorityQueue::peekPriority() const {
    return front->priority;
}

/*
 * Outputs the size of the first
 * value in the queue.
 * O(1)
 */
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
}

/*
 * Outputs the priority and
 * value of each element
 * in the queue.
 * O(N)
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    cout << "{";
    while (current != NULL) {
        cout << current->value << ":" << current->priority;
        if (current->next != NULL)
            cout << ", ";
        current = current->next;
    }
    cout << "}" << endl;
}
