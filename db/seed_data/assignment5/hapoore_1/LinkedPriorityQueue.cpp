#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
   front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
    delete front;
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* curr = front;
    bool found = false;
    if(curr != NULL) {
        while(curr->next != NULL) {
            if (curr->value == value && curr->priority > newPriority) {
                curr->priority = newPriority;
                found = true;
                break;
            } else {
                curr = curr-> next;
            }
        }
        if (curr->value == value && curr->priority > newPriority) {
            curr->priority = newPriority;
            found = true;
        }
    }
    if(found) {
        rearrangeNodes(curr);
    }
    if (curr == NULL || !found) {
        throw "That element does not exist with a priority worse than the desired priority.";
    }
}


void LinkedPriorityQueue::rearrangeNodes(ListNode* curr) {
    ListNode* prev = front;
    while(prev->next != curr) {
        prev = prev->next;
    }
    if(front->priority > curr->priority || (front->priority == curr->priority && front->value <= curr->value)){
        prev->next = curr->next;
        curr->next = front;
        front = curr;
    } else {
        ListNode* insertion = front;
        while(insertion->next != NULL && (insertion->next->priority < curr->priority || (insertion->next->priority == curr->priority && insertion->next->value <= curr->value))) {
            insertion = insertion->next;
        }
        prev->next = curr->next;
        curr->next = insertion->next;
        insertion->next = curr;
    }
}

void LinkedPriorityQueue::clear() {
    if(front != NULL) {
        while(front->next != NULL) {
            ListNode* trash = front;
            if(front->next != NULL) {
                front = front->next;
                delete trash;
            }
        }
        ListNode* trash = front;
        front = NULL;
        delete trash;
    }
}

string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw "Empty queue.";
    }
    ListNode* trash;
    string urgent;
    trash = front;
    urgent = front->value;
    if (front->next != NULL) {
        front = front->next;
    } else {
        front = NULL;
    }
    delete trash;
    return urgent;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    if(front == NULL) {
        front = new ListNode(value, priority);
    } else {
        if(front->priority > priority || (front->priority == priority && front->value <= value)){
            front = new ListNode(value, priority, front);
        } else {
            ListNode* curr = front;
            while(curr->next != NULL && (curr->next->priority < priority || (curr->next->priority == priority && curr->next->value <= value))) {
                curr = curr->next;
            }
            curr->next = new ListNode(value, priority, curr->next);
        }
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return(front == NULL);
}

string LinkedPriorityQueue::peek() const {
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    if (front == NULL) {
        return 0;
    }
    int counter = 1;
    ListNode* temp = front;
    while(temp->next != NULL) {
        temp = temp->next;
        counter++;
    }
    return counter;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    if (queue.isEmpty()) {
        out << "{}";
    } else {
        out << "{";
        ListNode* temp;
        if (queue.front != NULL) {
            temp = queue.front;
            out << *temp;
            while(temp->next != NULL) {
                temp = temp->next;
                out << ", ";
                out << *temp;
            }
        }
        out << "}";
    }
    return out;
}
