// VectorPriorityQueue has a fast enqueue operation,
// but slow dequeue and peek operations

#include "VectorPriorityQueue.h"
#include "strlib.h"

VectorPriorityQueue::VectorPriorityQueue() {

}

VectorPriorityQueue::~VectorPriorityQueue() {

}

// Modify the priority of a given existing value in the queue.
// if the given value is present in hte queue has already
// has a more urgent priority to the given new priority
// or if the given value si not already in the queue, it will
// throw an exception.
// O(N)
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for(int i = 0; i < vPQ.size(); i++){
       if(vPQ[i].value == value){
          if(vPQ[i].priority <= newPriority){
              throw "It already has a more urgent priority";
          }else{
          vPQ[i].priority = newPriority;
          return;
          }
       }
    }
    throw "The value is not in the queue";
}

//Remove all elements from the priority queue.
//O(1)
void VectorPriorityQueue::clear() {
    vPQ.clear();

}

//Remove the element with the most urgent priority the your priority queue
//It will throw an exception if the queue does not contain any elements.
//O(N)
string VectorPriorityQueue::dequeue() {
    if(vPQ.isEmpty()){
        throw "The queue is empty";
    }else{
    int min = 0; // the index of highest priority element in the vector
    for (int i = 1; i < vPQ.size(); i++){
        if(vPQ[i]< vPQ[min]){
            min = i; // find the highest priority Index
        }
    }
    string deElement;
    deElement = vPQ[min].value;
    vPQ.remove(min); // delete the element
    return deElement;
    }
}

// Add the given string value into the queue with the given priority
//O(1)
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry velement(value, priority);
    vPQ.add(velement);
}

//return true if the queue doesn't contain any elements.
// otherwise false.
//O(1)
bool VectorPriorityQueue::isEmpty() const {
    return vPQ.isEmpty();
}

//return the string element with the most urgent priority
//without removing it or altering the state of the queue
//it will throw ana exception if the queue is empty
//O(N)
string VectorPriorityQueue::peek() const {
    if(vPQ.isEmpty()){
        throw "The queue is empty";
    }else{
    int min = 0; // the index of highest priority element in the vector
    for (int i = 1; i < vPQ.size(); i++){
        if(vPQ[i] < vPQ[min]){
            min = i; // find the highest priority Index
        }
    }
    return vPQ[min].value;
    }
}

//Return priority that is most urgent from the queue,
// without removing it or alterin the queue.
//throw an exception if the queue is empty
//O(N)
int VectorPriorityQueue::peekPriority() const {
    if(vPQ.isEmpty()){
        throw "The queue is empty";
    }else{
    int min = 0; // the index of highest priority element in the vector
    for (int i = 1; i < vPQ.size(); i++){
        if(vPQ[i] < vPQ[min]){
            min = i; // find the highest priority Index
        }
    }
    return vPQ[min].priority;
    }
}

//return the number of elements in the queue
//O(1)
int VectorPriorityQueue::size() const {
    return vPQ.size();
}

//change all the elements to a string output
//O(1)
string VectorPriorityQueue::toString() const {
    string vPQstring;
    if(!vPQ.isEmpty()){
       vPQstring += "\"" + vPQ[0].value + "\":" + integerToString(vPQ[0].priority);
        for (int i = 1; i < vPQ.size(); i++){
            vPQstring += ", \"" + vPQ[i].value + "\":" + integerToString(vPQ[i].priority);
        }
    }
    return vPQstring;
}

// print the queue to the console
//O(1)
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    out << queue.toString();
    out << "}";
    return out;
}
