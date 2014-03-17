/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Priority Queue
 * This program implements a priority queue using a Linked List.
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    m_front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    while (m_front != NULL) {
        ListNode* trash = m_front;
        m_front = m_front -> next;
        delete trash;
    }
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    int tracker = 0;
    ListNode* current;
    current = m_front;
    while (current != NULL) {
        if (current->value == value) {
            if (current->priority < newPriority) {
                string msg = "Current priority is more urgent.";
                throw msg;
            } else {
                current->priority = newPriority;
                tracker++;
                break;
            }
        } else {
            current = current->next;
        }
    }
    if (tracker == 0) {
        string msg = "Value can't be found.";
        throw msg;
    }
}

void LinkedPriorityQueue::clear() {
    m_front = NULL;
}

string LinkedPriorityQueue::dequeue() {
    string answer;
    if (m_front == NULL) {
        string msg = "Queue contains no elements.";
        throw msg;
    } else {
        answer = m_front->value;
        ListNode* oldFront;
        oldFront = m_front;
        m_front = m_front -> next;
        oldFront -> next = NULL;
    }
    return answer;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* entry = new ListNode(value, priority, NULL);
    ListNode* current;

    if (m_front == NULL) {
        m_front = entry;
    } else {
        current = m_front;
        while (current -> next != NULL) {
            if (current -> next -> priority > priority) {
                break;
            } else if (current -> next -> priority == priority) {
                if (value > current -> next -> value) {
                    current = current -> next;
                } else if (value < current -> next -> value) {
                    break;
                }
            } else {
                current = current -> next;
            }
        }
        entry -> next = current -> next;
        current -> next = entry;
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    if (m_front == NULL) return true;
    else return false;
}

string LinkedPriorityQueue::peek() const {
    string answer = m_front->value;
    return answer;
}

int LinkedPriorityQueue::peekPriority() const {
    int priority = m_front->priority;
    return priority;
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* current;
    current = m_front;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    int size = queue.size();
    if (!queue.isEmpty()) {
        out << *queue.m_front;
        ListNode* current;
        current = queue.m_front -> next;
        while (current != NULL) {
            out << ", " << *current;
            current = current -> next;
        }
    }
    out << "}";
    return out;
}
