/*
 * linked list implementation
 * enqueuing is slower, but by consequent peeking and dequeuing are much much faster
 * changing value is still slow though
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

//0(N^2) because we find the value then re-enqueue it
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;

    //if the value we want to change is at the front
    if (current->value == value){

        front = front->next;
        delete current; //deletes the entry
        enqueue(value , newPriority); //reenqueues it

    } else{

        while (current->next != NULL){ //iterates over the whole vector
            if (current->next->value == value){ //if we found the value
                break;
            }
            current = current->next; //increment the current pointer
        }
        if (current->next != NULL){ //if we're not at the end of a list
            //deletes it then re-enqueues it with the new priority
            ListNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
            enqueue(value , newPriority);
        }

    }
}

//0(N)
void LinkedPriorityQueue::clear() {
    //iterates over the whole vector
    while (front != NULL) {
            ListNode* trash = front;
            front = front->next;
            delete trash;
    }
}

//0(1)
string LinkedPriorityQueue::dequeue() {

    if (front == NULL){
        throw "list is empty";
    }

    //increments the front pointer ans returns the original front
    ListNode* current = front;
    front = front->next;
    return current->value;

}

//0(N) iterates over the whole list
void LinkedPriorityQueue::enqueue(string value, int priority) {

    //creates a new listnode with the information and a pointer to null
    ListNode* entry = new ListNode(value , priority , NULL);
    ListNode* current = front;
    //if the value has to be in the front (various tests)
    if (front == NULL || entry->priority < front->priority || (entry->priority == front->priority && entry->value < front->value)){
        front = entry;
        entry->next = current;
    } else{
        //if the value is somewhere in the list
        while (true){ //iterates over the list
            if (current->next == NULL){ //if we've reached the end of the list
                current->next = entry;
                break;
            } else if (entry->priority < current->next->priority || (entry->priority == current->next->priority && entry->value < current->next->value)){
                //various tests...
                //adds the listnode in the appropriate position
                ListNode* temp = current->next;
                current->next = entry;
                entry->next = temp;
                break;
            }
            current = current->next; //increment the current pointer
        }

    }
}

//0(1)
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

//0(1)
string LinkedPriorityQueue::peek() const {
    if (front == NULL) throw "list is empty";
    return front->value;
}

//0(1)
int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) throw "list is empty";
    return front->priority;
}

//0(N)
int LinkedPriorityQueue::size() const {

    int counter = 0;
    ListNode* current = front;

    //iterates over the whole list, incrementing a constant
    while(current != NULL){
        counter ++;
        current = current->next;
    }

    return counter;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    if (queue.front != NULL){
        out << "\"" << queue.front->value << "\":" << queue.front->priority;
        ListNode* current = queue.front->next;
        while(current != NULL) {
            out << " , \"" << current->value << "\":" << current->priority;
            current = current->next;
        }
    }
    out << "}";
    return out;
}
