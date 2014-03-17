// This file implements the linked list version of a priority queue. This is very fast for peeking/dequeueing, but slower for enqueuing, etc.

#include "LinkedPriorityQueue.h"
#include "linkedlist.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front=NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    ListNode* current=front;
    ListNode currentNode=*front;
    while(current->next!=NULL) {
        delete current;
        current=currentNode.next;
    }
    delete current;

}

//This method changes the priority of an entry by removing it from the list and re-enqueueing it with a new priority
//This runs in O(N) time
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current=front;
    if(current->value==value) {
        front=current->next;
        enqueue(value, newPriority);
    } else while(current->next!=NULL) {
        string nextValue=current->next->value;
        if(nextValue==value) {
            if(current->next->next==NULL) {
                current->next=NULL;
            } else {
                current->next=current->next->next;
            }
            enqueue(value, newPriority);
            break;
        }
        current=current->next;
    }

}

//This method clears the contents of the list by resetting the value of the first pointer, breaking ties with any existing list
//This runs in O(1) time
void LinkedPriorityQueue::clear() {
    front=NULL;
}

//This method returns the value of the most urgent entry and removes that entry from the queue
//This runs in O(1) time
string LinkedPriorityQueue::dequeue() {
    if(size()==0) {
        throw "This queue is empty";
    } else {
        ListNode* newFront=front->next;
        ListNode* dq=front;
        front=newFront;
        return dq->value;
    }
}

//This method adds an entry to the list in the correct location
//This runs in O(N) time
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newNode=new ListNode(value, priority);
    if(size()==0) {
        front=newNode;
    } else {
        ListNode* current=front;
        if(newNode->priority<current->priority) {
            newNode->next=current;
            front=newNode;
        } else {
            while(true) {
                if(current->next==NULL) {
                    current->next=newNode;
                    break;
                } else {
                    ListNode* nextNode=current->next;
                    if(newNode->priority==nextNode->priority) {
                        if(newNode->value<nextNode->value) {
                            current->next=newNode;
                            newNode->next=nextNode;
                            break;
                        } else {
                            current=current->next;
                        }
                    } else if(newNode->priority<nextNode->priority) {
                        current->next=newNode;
                        newNode->next=nextNode;
                        break;
                    } else {
                        current=current->next;
                    }
                }
            }
        }
    }

}

//This method returns a boolean indicating whether or not the list is empty
//This runs in O(1) time
bool LinkedPriorityQueue::isEmpty() const {
    if(size()==0) {
        return true;
    }
    return false;
}

//This method returns the value of the most urgent entry without removing it from the queue
//This runs in O(1) time
string LinkedPriorityQueue::peek() const {
    if(size()==0) {
        throw "This queue is empty";
    } else {
        ListNode node=*front;
        string value=node.value;
        return value;
    }
}

//This method returns the priority of the most urgent entry without removing it from the queue
//This runs in O(1) time
int LinkedPriorityQueue::peekPriority() const {
    if(size()==0) {
        throw "This queue is empty";
    } else {
        ListNode node=*front;
        int priority=node.priority;
        return priority;
    }
}

//This method returns the size of the queue
//This runs in O(N) time
int LinkedPriorityQueue::size() const {
    int counter=0;
    ListNode* currentNodePointer=front;
    if(currentNodePointer!=NULL) {
        while(currentNodePointer->next!=NULL) {
            counter++;
            currentNodePointer=currentNodePointer->next;
        }
        counter++;
        return counter;
    }
    return counter;
}

//This method overloads the "<<" operator to output the contents of the queue
//This runs in O(N) time
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    cout << "{";
    if(queue.size()!=0) {
        ListNode* p=queue.front;
        for(int i=0; i<queue.size(); i++) {
            ListNode node=*p;
            cout << "\"";
            cout << node.value;
            cout << "\"";
            cout << ":";
            cout << node.priority;
            cout << " ";
            p=node.next;
        }
    }
    cout << "}";
    return out;
}
