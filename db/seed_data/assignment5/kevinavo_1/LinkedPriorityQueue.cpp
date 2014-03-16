/*Name: Kevin Vo
 *TA: Jimmy lee
 * This cpp file implements a priority queue using a linkedlist as a
 * data structure.
*/

#include "LinkedPriorityQueue.h"

/* This is the construcutor for a LinkedPriorityQueue
 * object. It doesn't have any parameters so all it
 * does is sets the front pointer to null.
*/
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

/* This is the destructor. It iterates through
 * the linkedlist and destroys each node
 * one by one. It runs in O(N).
*/
LinkedPriorityQueue::~LinkedPriorityQueue() {
    ListNode* current = front;
    ListNode* temp;
    while (current != NULL){
        temp = current;
        current = current->next;
        delete temp;
    }
}

/* This function changes the priority of a given node in the linkedlist.
 * The algorithm loops handles two cases one for adding in the front and
 * one for insertion. It loops over the linked list and adds the new
 * ListNode in the rightful order and then deletes the old node. It throws
 * exceptions when the user tries to change a value that is not in the
 * queue, when the new priority is greater than the old and when they
 * try to reassign a value to the exact same priority. It rusn in O(N)
*/
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (front == NULL){
        throw "The queue is empty!";
    }
    ListNode* current = front;
    ListNode* newNode = new ListNode(value, newPriority);
    //ensures that insertion only occurs once
    bool trigger = true;
    if (newPriority >= current->priority){
        while (current->next != NULL && current->next->value != value){
            if ((newPriority < current->next->priority && trigger) || valid(value,newPriority, current)){
                newNode->next = current->next;
                current->next = newNode;
                current = current->next->next;
                trigger = false;
            }else{
                current = current->next;
            }
        }
        if (current->priority < newPriority){
            throw "New priority must be a number less than the old priority.";
        }
        if (current->next == NULL){
            throw "Value not found in the queue.";
        }
        if (value == current->value && current->priority == newPriority){
            throw "not a valid command";
        }
        ListNode* trash = current->next;
        current->next = current->next->next;
        delete trash;
    }else{
        newNode->next = current;
        front = newNode;
        while (current->next != NULL && current->next->value != value){
            current = current->next;
        }
        ListNode* trash = current->next;
        current->next = current->next->next;
        delete trash;
    }
}

/* This function clears the linked list. The difference
 * between this and the constructor is that the clear
 * function maintains the old linkedlist object.It runs in O(N).
*/
void LinkedPriorityQueue::clear() {
    while (front != NULL){
        ListNode* garbage = front;
        front = front->next;
        delete garbage;
    }
}

/* Since the queue is guranteed to be in a sorted order
 * the dequeue function simply pulls out the first node,
 * returns the value and destroys it. It runs in O(1).
*/
string LinkedPriorityQueue::dequeue() {
    if (front == NULL){
        throw "the queue is empty";
    }
    ListNode* current = front;
    ListNode* temp;
    string result = front->value;
    temp = current;
    current = current->next;
    front = current;
    delete temp;
    return result;
}

/* The general algorithm for enqueuing is to loop over the linkedlist
 * and add the new node in the correct order of priority and value.
 * It handles two cases one for insertion and one for adding to the
 * front. It runs in O(N).
*/
void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (front == NULL) {
            ListNode* newNode = new ListNode(value,priority);
            newNode->next = NULL;
            front = newNode;
    }else{
        ListNode* current = front;
        if ((priority == current->priority && value > current->value) || (priority > current->priority)){
            while(current->next != NULL && priority >= current->priority){
                if (priority == current->next->priority) break;
                current = current->next;
            }
            while (current->next != NULL && (value >= current->next->value)){
                current = current->next;
            }
            ListNode* newNode = new ListNode(value,priority);
            newNode->next = current->next;
            current->next = newNode;
        }else {
            ListNode* newNode = new ListNode(value, priority);
            newNode->next = current;
            front = newNode;
        }
    }
    cout << "Enqueued " << "\"" << value << "\"" << " with priority " << priority << endl;
}

/*This function checks to see if the linked list
is empty. It runs in O(1).*/
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

/* This function looks at the first node
 * and returns the value without deleting
 * the node. It runs in O(1).
*/
string LinkedPriorityQueue::peek() const {
    if (front == NULL){
        throw "The queue is empty!";
    }
    return front->value;
}

/*Returns the highest priority value. It runs in O(1).*/
int LinkedPriorityQueue::peekPriority() const {
    return front->priority;
}

/*This function loops over the entire linked
list and returns the priorty number that is the
lowest (highest priority number). It runs in O(N).
*/
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* current = front;
    while (current != NULL){
        current = current->next;
        count++;
    }
    return count;
}

bool LinkedPriorityQueue::valid(string value, int newPriority,ListNode* current){
    return (newPriority == current->next->priority && current->next->value > value);
}

/* This is an operator overload that prints out the entire linked list
 * using a while loop. It runs in O(N).
*/
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* temp = queue.front;
    int count = 0;
    out <<"{";
    while (temp != NULL){
        if (count == 0){
            out << *temp;
        }else{
            out << ", " << *temp;
        }
        temp = temp->next;
        count++;
    }
    out << "}";

    return out;
}
