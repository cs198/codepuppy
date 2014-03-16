// HeapPriorityQueue has fast enqueue operation, dequeue and peek operations
// For function descriptions, See VectorPriorityQueue.cpp

#include "HeapPriorityQueue.h"
#include "strlib.h"
#include "cmath"

HeapPriorityQueue::HeapPriorityQueue() {
    elements = new PQEntry[10](); // zero out
    m_size = 1;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;

}

//O(N)
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for(int i = 1; i < m_size; i++){
            if(elements[i].value == value){
                if(elements[i].priority <= newPriority){
                    throw "It already has a more urgent priority";
                }else{
                    elements[i].priority = newPriority;
                    bubbleUp(i);
                    return;
                }
            }
        }
    throw "The value is not in the queue";
}

//O(1)
void HeapPriorityQueue::clear() {
    m_size = 1;

}

//O(long(N))
string HeapPriorityQueue::dequeue() {
    if(m_size == 1){
        throw "The queue is empty";
    }else{
        string deValue = elements[1].value;
        elements[1] = elements[m_size-1];
        m_size--;
        bubbleDown(1);
        return deValue;
    }
}

//O(log(N))
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry temp;
    temp.priority = priority;
    temp.value = value;
    if(m_size == 1){
        elements[0].priority = 0;
        elements[0].value = "0";
        m_size++;
        elements[1] = temp; // add an element to an empty queue to index 1
    }else{
        checkResize();
        m_size++;
        elements[m_size-1] = temp;
        bubbleUp(m_size-1);
    }
}

//O(1)
bool HeapPriorityQueue::isEmpty() const {
    if(m_size == 0){
        return true;
    }else{
        return false;
    }
}

//O(1)
string HeapPriorityQueue::peek() const {
    if(m_size == 1){
        throw "The queue is empty";
    }else{
        string peekValue = elements[1].value; // index 1 is the highest poriority
        return peekValue;
    }
}

//O(1)
int HeapPriorityQueue::peekPriority() const {
    if(m_size == 1){
        throw "The queue is empty";
    }else{
        int peekPriority = elements[1].priority; // index 1 is the highest poriority
        return peekPriority;
    }
}

//O(1)
int HeapPriorityQueue::size() const {
    return m_size - 1;
}

//O(N)
void HeapPriorityQueue::checkResize(){
    if(m_size == capacity){
        PQEntry* oldElements = elements;
        elements = new PQEntry[2*capacity]();
        for (int i = 0; i < m_size; i++){
            elements[i] = oldElements[i];
        }
        delete[] oldElements;
        capacity *=2;
    }
}

//Bubble up function to swap element upward with its parent until
//it is in order
//O(log(N))
void HeapPriorityQueue::bubbleUp(int index){
    int fatherIndex = floor(index/2);
    if(fatherIndex < 1){
        return;
    }else{
       if(elements[index] > elements[fatherIndex]) {
           return;
       }else{
           PQEntry temp;
           temp = elements[fatherIndex];
           elements[fatherIndex] = elements[index]; // swap up with fatherindex
           elements[index] = temp;
           bubbleUp(fatherIndex); // recursion
       }
    }
}

//Then swap it downward with its most-urgent child
//until in order
//O(log(N))
void HeapPriorityQueue::bubbleDown(int index){
    int childIndexLeft = 2*index;
    int childIndexRight = 2*index+1;
    if(childIndexRight > m_size -1){
        return;
    }else{
        if(elements[index] <= elements[childIndexLeft] && elements[index] <= elements[childIndexRight]){
            return;
        }else{
            if(elements[childIndexLeft] <= elements[childIndexRight]){ // if the left one is more urgent
               PQEntry temp;
               temp = elements[childIndexLeft];
               elements[childIndexLeft] = elements[index]; // swap down with left childindex
               elements[index] = temp;
               bubbleDown(childIndexLeft); // recursion
            }else{ // if the right one is more urgent
                PQEntry temp;
                temp = elements[childIndexRight];
                elements[childIndexRight] = elements[index]; // swap down with right childindex
                elements[index] = temp;
                bubbleDown(childIndexRight);  // recursion
            }
        }
    }
}

//O(N)
string HeapPriorityQueue::toString() const{
   string hPQstring;
   if(m_size != 1){
       hPQstring += "\"" + elements[1].value + "\":" + integerToString(elements[1].priority);
       for(int i = 2; i < m_size; i++){
       hPQstring += ", \"" + elements[i].value + "\":" + integerToString(elements[i].priority);
       }
   }
   return hPQstring;
}

//O(1)
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    out << queue.toString();
    out << "}";
    return out;
}
