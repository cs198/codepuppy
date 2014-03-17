//See VectorPriorityQueue.cpp for documentation member functions.
//This is the cpp file that runs linked Priority queue

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    first = NULL;
}
/**
 * @brief LinkedPriorityQueue::~LinkedPriorityQueue
 * O(N)
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    removeAll();
}
/**
 * @brief LinkedPriorityQueue::removeAll
 * removeAll deletes every node.
 * O(N)
 */
void LinkedPriorityQueue::removeAll(){
    while(first != NULL){
        ListNode* temp = first;
        first = first->next;
        delete temp;
    }
}
/**
 * @brief LinkedPriorityQueue::changePriority
 * @param value
 * @param newPriority
 * O(N)
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = first;
    if(first->value == value){
        first = first->next;
        enqueue(value, newPriority);
    }else{
        while(current->next != NULL){
            if(current->next->value == value){
                current->next = current->next->next;
                enqueue(value, newPriority);
                break;
            }
            current = current->next;
        }
    }
}
/**
 * @brief LinkedPriorityQueue::clear
 * O(N)
 */
void LinkedPriorityQueue::clear() {
    removeAll();
}
/**
 * @brief LinkedPriorityQueue::dequeue
 * @return
 * O(1)
 */
string LinkedPriorityQueue::dequeue() {
    if(isEmpty())throw "Empty Queue";
    ListNode* temp = first;
    first = first->next;
    string str = temp->value;
    delete temp;
    return str;

}

/**
 * @brief LinkedPriorityQueue::enqueue
 * @param value
 * @param priority
 * O(N)
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* nextEn = new ListNode(value, priority, NULL);
    if(first == NULL){
        first = nextEn;
    }else {
        if(first->next == NULL && first->priority < priority){
            first->next = nextEn;
        }else if(first->next == NULL || first->priority >= priority && first->value > value){
            ListNode* temp = first;
            first = nextEn;
            first->next = temp;
        }else{
            ListNode* current = first;
            while(current->next != NULL){
                if(current->next->priority > priority && current->value > value){
                    ListNode* temp = current->next;
                    current->next = nextEn;
                    current->next->next = temp;
                    break;
                }
                current = current->next;
            }
            if(current->next == NULL && current <= nextEn){
                current->next = nextEn;
            }else if(current->next == NULL && current > nextEn){
                ListNode* temp = current;
                current = nextEn;
                current->next = temp;
            }
        }

    }
}
/**
 * @brief LinkedPriorityQueue::isEmpty
 * @return
 * O(1)
 */
bool LinkedPriorityQueue::isEmpty() const {
    if(first == NULL){
        return true;
    }
    return false;
}
/**
 * @brief LinkedPriorityQueue::peek
 * @return
 * O(1)
 */
string LinkedPriorityQueue::peek() const {
    if(first != NULL){
        return first->value;
    }else throw "empty queue.";
}
/**
 * @brief LinkedPriorityQueue::peekPriority
 * @return
 * O(1)
 */
int LinkedPriorityQueue::peekPriority() const {
    if(first != NULL){
        return first->priority;
    }else throw "empty queue.";
}
/**
 * @brief LinkedPriorityQueue::size
 * @return
 * O(N)
 */
int LinkedPriorityQueue::size() const {
    if(isEmpty()){
        return 0;
    }
    int counter = 1;
    ListNode* current = first;
    while(current->next != NULL){
        counter++;
        current = current->next;
    }
    return counter;
}
/**
 * @brief operator <<
 * @param out
 * @param queue
 * @return
 * O(N)
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.first;
    cout << "{";
    while (current != NULL) {
        cout << *current;
        if(current->next != NULL){
            cout << ", ";
        }
        current = current->next;
    }
    cout << "}";
    return out;
}
