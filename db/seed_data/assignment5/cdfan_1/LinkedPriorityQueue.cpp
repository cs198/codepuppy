//This program implements priority queue using linkedList

#include "LinkedPriorityQueue.h"
#include "strlib.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode *current = front;
    ListNode *prev = front;
    bool found = false;
    //Locate the correct node for priority change, and change its priority to new if
    //the existing priority is larger or equals to the new priority.
    while(current != NULL) {
        if(current->value == value && newPriority < current->priority) {
            current->priority = newPriority;
            found = true;
            break;
        } else if(current->value == value) {
            throw "Current priority is already as urgent as or more urgent than the new priority.";
        }
        prev = current;
        current = current->next;
    }

    //If a node's priority is changed, relote itself to appriopriate position in order
    if(found) {
        ListNode *current2 = front;
        //if front is the correct location, move the current pointed node to front
        if(front->priority > newPriority || (front->priority == newPriority
           && toLowerCase(front->value) > toLowerCase(value))) {
            prev->next = prev->next->next;
            current->next = front;
            front = current;
        } else {
            //otherwise, continue loop through the list to find the right position.
            //Once it does, change the current-pointed node to that position.
            while(current2->next != NULL) {
                if(current2->next->priority > newPriority ||
                    (current2->next->priority == newPriority
                     && toLowerCase(current2->next->value) > toLowerCase(current->value))
                        ){
                    prev->next = prev->next->next;
                    current->next = current2->next;
                    current2->next =current;
                    break;
                }
                current2 = current2->next;
            }
        }
    }
    if(!found) throw "The priority queue does not contain such value.";
}

//put the front pointer to front->next, and delete the trash. It loop through the list to delete all
void LinkedPriorityQueue::clear() {
    while(front != NULL) {
        ListNode *trash = front;
        front = front->next;
        delete trash;
    }
}

//always return the value pointed by front, and removes it from the list
string LinkedPriorityQueue::dequeue() {
    if(front == NULL) throw "The priority queue is empty.";
    ListNode *poped = front;
    front = front->next;
    string result = poped->value;
    delete poped;
    return result;

}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode *current = front;
    //If front is null, or front priority is larger than new priority,
    //or if they are equal and the value is alphabetically ahead, add the new node to the front.
    if(front == NULL || front->priority > priority
        ||(front->priority == priority && toLowerCase(front->value)>toLowerCase(value))) {
        ListNode *newNode = new ListNode(value, priority, front);
        front = newNode;
    } else {
        //otherwise, loop through the list and find the new node's place. Add the node before any
        //priority that is larger than the new one, or equals to the new priority but alphbetically
        //after the new value.
        while(current->next != NULL) {
            if(current->next->priority > priority
                ||(current->next->priority == priority
                   && toLowerCase(current->next->value) > toLowerCase(value))) {
                ListNode *newNode = new ListNode(value, priority, current->next);
                current->next = newNode;
                break;
            }
            current = current->next;
        }
        //If after looping through the list, there isn't the right place for new node, add the node to the
        //end of the list. This also means that the new node priority is larger than all nodes in the list.
        if(current->next == NULL) {
            ListNode *newNode = new ListNode(value, priority);
            current->next = newNode;
        }
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

//Returns the value pointed by front
string LinkedPriorityQueue::peek() const {
    if(front == NULL) throw "The priority queue is empty.";
    return front->value;
}

//returns the priority pointed by front
int LinkedPriorityQueue::peekPriority() const {
    if(front == NULL) throw "The priority queue is empty.";
    return front->priority;
}

//Loop through the count the number of the nodes. Returns it as the size
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode *current = front;
    while(current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

//Returns the front
ListNode* LinkedPriorityQueue:: getFront() const {
    return front;
}

//Print out the priority queue. It will have a ascending order.
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
     out << "{";
     ListNode *current = queue.getFront();
     if(current != NULL){
         out<< "\"" << current->value <<"\":"<< current->priority;
         current = current->next;
         while(current != NULL) {
             out << ", " << "\"" << current->value <<"\":"<< current->priority;
             current = current->next;
         }
     }
     out << "}";
    return out;
}
