//File: LinkedPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the methods of LinkedPriorityQueue class
//  that implements a priority queue using a linked list.

#include "LinkedPriorityQueue.h"
#include "strlib.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(front == NULL) throw("Empty Queue");

    bool found = false; //Flag to see if value was found.
    ListNode* trash;

    //Start with first entry.
    ListNode* current = front;
    if(front->value == value){
        if(front->priority < newPriority) throw("Value already higher priority");
        trash = front;
        front = front->next;
        delete trash;
        return;
    }

    //If not first entry continue until value is found or end of list
    while(current->next != NULL){
        if(current->next->value == value){
            found = true;
            break;
        }
        current = current->next;
    }

    //If found and newPriority is lower than current priority, delete entry
    //and re-enqueue with newPriority.
    if(found){
        if(current->next->priority <= newPriority) throw("Value already higher priority");

        trash = current->next;
        current->next = current->next->next;
        delete trash;
        enqueue(value, newPriority);
    }else throw("Value not in queue");

}

void LinkedPriorityQueue::clear() {
    //Delete all entries until list is empty
    while (front != NULL) {
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }
}

string LinkedPriorityQueue::dequeue() {
    if(front == NULL) throw("Empty Queue");

    //Get value of first entry, then delete first entry
    string result = front->value;
    ListNode* trash = front;
    front = front->next;
    delete trash;

    return result;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
        ListNode* current = front;

        //If list is empty add first entry to list.
        if(front == NULL){
            front = new ListNode(value, priority);
            return;
        }

        //If new entry belongs in the first position.
        if(priority < front->priority ||
                (priority == front->priority && value < front->value)){
            ListNode* temp = front;
            front = new ListNode(value, priority, temp);
            return;
        }

        //Search through list until current is end of list or the next
        //entry is higher priority.
        while(current->next != NULL &&
              !(priority < current->next->priority ||
                (priority == current->next->priority && value < current->next->value))){
                current = current->next;
        }
        //Put new entry between current and current->next.
        ListNode* temp = current->next;
        current->next = new ListNode(value, priority, temp);
}

bool LinkedPriorityQueue::isEmpty() const {
 return front == NULL;
}

string LinkedPriorityQueue::peek() const {
    if(front == NULL) throw("Empty Queue");
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    if(front == NULL) throw("Empty Queue");
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    //Count entries in list
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out<<"{";
    if (!queue.isEmpty()) {
        //Print first entry
        cout<<*(queue.front);
        ListNode* current = queue.front->next;
        //Keep printing until at end of the list.
        while (current != NULL) {
            cout<< ", "<<*current;
            current = current->next;
        }
    }
    out<<"}";
    return out;
}
