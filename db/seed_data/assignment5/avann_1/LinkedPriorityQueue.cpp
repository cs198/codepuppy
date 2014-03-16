//This is the Linked List implementatin of a priority queue for
//CS106B Assigment 5. It operates more quickly than the Vector Implementation.
//I received help from the Lair for changePriority for this assignment.

#include "LinkedPriorityQueue.h"
#include "strlib.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

/* See VectorPriorityQueue.h
 * O(N)
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(front->value == value){
        if(front->priority <= newPriority){
            throw("Not more urgent priority");
        }else{
            ListNode* newfront = front->next;
            front->priority = newPriority;
            ListNode* store = front;
            front = newfront;
            enqueue(store->value, store->priority);
        }
    }else{
        bool notfound = true;
        ListNode* current = front;
        ListNode* temp;

        while(current->next != NULL){
            temp = current->next;
            if(temp->value != value){
                current = current->next;
            }else{
                notfound = false;
                break;
            }
        }

        if(notfound) throw("Value not present");
        if(temp->priority <= newPriority) throw("Not more urgent priority");

        temp->priority = newPriority;
        if(current->next->next == NULL){
            current->next = NULL;
            enqueue(temp->value, temp->priority);
            delete temp;
        }else{
            ListNode* aftervalue = current->next->next;
            enqueue(temp->value, temp->priority);
            current->next = aftervalue;
            delete temp;
        }
    }
}

/* See VectorPriorityQueue.h
 * O(N)
 */
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
string LinkedPriorityQueue::dequeue() {
    if(front == NULL){
        throw("Empty!");
    }else if(front->next == NULL){
        string oldfront = front->value;
        front = NULL;
        return oldfront;
    }else{
        ListNode* newfront = front->next;
        string oldfront = front->value;
        delete front;
        front = newfront;
        return oldfront;
    }
}

/* See VectorPriorityQueue.h
 * O(N)
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newnode = new ListNode(value, priority);
    if(front==NULL){
        front = newnode;
    }else if(front->priority > priority){
        ListNode* temp = front;
        front = newnode;
        front->next = temp;
    }else{
        ListNode* current = front;
        while(current->next != NULL && priority > current->priority && current->next->priority <= priority){
            current = current->next;
        }
        if(current->next == NULL){
            current->next = newnode;
        }else{
            ListNode* temp = current->next;
            current->next = newnode;
            current->next->next = temp;
        }
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

/* See VectorPriorityQueue.h
 * O(1)
 */
string LinkedPriorityQueue::peek() const {
    if(front == NULL){
        throw("Empty!");
    }else{
        return front->value;
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
int LinkedPriorityQueue::peekPriority() const {
    if(front == NULL){
        throw("Empty!");
    }else{
        return front->priority;
    }
}

/* See VectorPriorityQueue.h
 * O(N)
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

/* See VectorPriorityQueue.h
 * O(1)
 */
ListNode* LinkedPriorityQueue::getFront() const{
    return front;
}

/* See VectorPriorityQueue.h
 * O(N)
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.getFront();
    out << "{";
    while(current!=NULL){
        out << "\"" << current->value << "\":" << current->priority ;
        if(current->next!=NULL){
            out << ", ";
        }
        current = current->next;
    }
    out << "}";
    return out;
}
