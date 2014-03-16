//See VectorPriorityQueue.cpp for documentation member functions.
//This is the cpp file that runs heap Priority queue.


#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    m_array = new PQEntry[8]();
    numOfElements = 0;
    capOfElements = 7;
}
/**
 * @brief HeapPriorityQueue::~HeapPriorityQueue
 * O(1)
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] m_array;
}
/**
 * @brief HeapPriorityQueue::changePriority
 * @param value
 * @param newPriority
 * O(NlogN)
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    if(isEmpty()){
        throw "No names in queue.";
    }
    for(int i = 1; i <= numOfElements; i++){
        if(m_array[i].value == value){
            m_array[i].priority = newPriority;
            bubbleUp(i);
            return;
        }
    }
}
/**
 * @brief HeapPriorityQueue::clear
 * O(1)
 */
void HeapPriorityQueue::clear() {
    delete m_array;
    m_array = new PQEntry[8]();
    numOfElements = 0;
    capOfElements = 7;
}
/**
 * @brief HeapPriorityQueue::dequeue
 * @return
 * O(LogN)
 */
string HeapPriorityQueue::dequeue() {
    if(isEmpty())throw "Queue is empty";
    string str = m_array[1].value;
    m_array[1] = m_array[numOfElements];
    m_array[numOfElements] = PQEntry();
    numOfElements--;
    bubbleDown(1);
    return str;
}
/**
 * @brief HeapPriorityQueue::bubbleDown
 * @param front
 * O(LogN)
 */
void HeapPriorityQueue::bubbleDown(int front){
    if(front*2 <= numOfElements && m_array[front] > m_array[front*2]){
        PQEntry temp = m_array[front*2];
        m_array[front*2] = m_array[front];
        m_array[front] = temp;
        bubbleDown(front*2);
        bubbleDown(front);
    }else if((front*2 + 1) <= numOfElements && m_array[front] > m_array[front*2 + 1]){
        PQEntry temp = m_array[front*2 + 1];
        m_array[front*2 + 1] = m_array[front];
        m_array[front] = temp;
        bubbleDown(front*2 + 1);
        bubbleDown(front);
    }
}
/**
 * @brief HeapPriorityQueue::enqueue
 * @param value
 * @param priority
 * O(LogN)
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry nextEntry = PQEntry(value, priority);
    numOfElements++;
    if(numOfElements < capOfElements){
        m_array[numOfElements] = nextEntry;
    }else{
        PQEntry* temp = m_array;
        m_array = new PQEntry[(capOfElements + 1)*2]();
        capOfElements = ((capOfElements + 1)*2 - 1);
        m_array[numOfElements] = nextEntry;
        for(int i = 1; i < numOfElements; i++){
            m_array[i] = temp[i];
        }
    }
    bubbleUp(numOfElements);

}
/**
 * @brief HeapPriorityQueue::bubbleUp
 * @param current
 * O(LogN)
 */
void HeapPriorityQueue::bubbleUp(int current){
    if(current/2 > 0){
        if(m_array[current] < m_array[current/2]){
            PQEntry temp = m_array[current/2];
            m_array[current/2] = m_array[current];
            m_array[current] = temp;
            bubbleUp(current/2);
        }
    }
}
/**
 * @brief HeapPriorityQueue::isEmpty
 * @return
 * O(1)
 */
bool HeapPriorityQueue::isEmpty() const {
    if(size() == 0){
        return true;
    }else return false;
}
/**
 * @brief HeapPriorityQueue::peek
 * @return
 * O(1)
 */
string HeapPriorityQueue::peek() const {
    if(isEmpty()){
        throw "No names in queue.";
    }
    return m_array[1].value;
}
/**
 * @brief HeapPriorityQueue::peekPriority
 * @return
 * O(1)
 */
int HeapPriorityQueue::peekPriority() const {
    if(isEmpty()){
        throw "No names in queue.";
    }
    return m_array[1].priority;

}
/**
 * @brief HeapPriorityQueue::size
 * @return
 * O(1)
 */
int HeapPriorityQueue::size() const {
    return numOfElements;
}
/**
 * @brief operator <<
 * @param out
 * @param queue
 * @return
 * O(N)
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    cout << "{";
    for(int i = 1; i <= queue.numOfElements; i++){
        cout << queue.m_array[i];
        if(i < queue.numOfElements){
            cout << ", ";
        }
    }
    cout << "}";
    return out;
}
