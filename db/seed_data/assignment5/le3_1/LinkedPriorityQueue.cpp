// LinkedPriorityQueue has a slow enqueue operation,
// but fast dequeue and peek operations
// For function descriptions, See VectorPriorityQueue.cpp

#include "LinkedPriorityQueue.h"
#include "ListNode.h"
#include "strlib.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL; // empty list

}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();

}

//O(N)
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(value == front->value){
        if (front->priority <= newPriority){
            throw "It already has a more urgent priority";
        }else{
            front->priority = newPriority;
        }
    }else{
        ListNode* current = front;
        while(current->next != NULL){
            if(current->next->value == value){
                if(current->next->priority <= newPriority){
                    throw "It already has a more urgent priority";
                }else{
                    current->next = current->next->next;
                    this->enqueue(value, newPriority);
                    return;
                }
            }
        current = current->next;
        }
    }
    throw "The value is not in the queue";
}

// O(N)
void LinkedPriorityQueue::clear() {
    while(front != NULL){
        ListNode* trash = front;
        front = front->next;
        delete trash;
    }

}

//O(1)
string LinkedPriorityQueue::dequeue() {
    if(front == NULL){
        throw "The queue is empty";
    }else{
    ListNode* temp;
    temp = front;
    front = front->next;
    return temp->value;
    }
}

//O(N)
void LinkedPriorityQueue::enqueue(string value, int priority) {
    // enqueue at front
    ListNode* newNode = new ListNode(value, priority);
    if(front == NULL){
        front = newNode;
    }else{
        if(priority < front->priority){
            newNode->next = front;
            front = newNode;
        }else{
        // enqueue later in the list
            ListNode* current = front;
            while(current->next!= NULL){
                if((newNode->priority < current->next->priority) || ((newNode->priority == current->next->priority)&&(newNode->value < current->next->value))){
                    newNode->next = current->next;
                    current->next = newNode;
                    return;
                }
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

//O(1)
bool LinkedPriorityQueue::isEmpty() const {
    if(front == NULL){
        return true;
    }else{
        return false;
    }
}

//O(1)
string LinkedPriorityQueue::peek() const {
    if(front == NULL){
        throw "The queue is empty";
    }else{
    return front->value;
    }
}

//O(1)
int LinkedPriorityQueue::peekPriority() const {
    if(front == NULL){
        throw "The queue is empty";
    }else{
    return front->priority;
    }
}

//O(N)
int LinkedPriorityQueue::size() const {
    // TODO: implement
    int count = 0;
    ListNode* current = front;
    while(current != NULL){
        current = current->next;
        count++;
    }
    return count;
}

//O(N)
string LinkedPriorityQueue::toString() const{
    string lPQstring;
    if(front != NULL){
        lPQstring += "\"" + front->value + "\":" + integerToString(front->priority);
        ListNode* current = front->next;
        while(current!= NULL){
            lPQstring += ", \"" + current->value + "\":" + integerToString(current->priority);
            current = current->next;
        }
    }
    return lPQstring;
}

//O(1)
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    out << queue.toString();
    out << "}";
    return out;
}
