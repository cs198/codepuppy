//This is the Vector PQEntry implementation of a priority queue
//for 106B Assignment 5.

#include "vector.h"
#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

/* The changePriority function finds the value requested in the queue and
 * modifies its priority to be more urgent (a smaller integer). If the value is
 * present in the queue and has a more urgent priority than the new priority, or
 * if it is not present, the function throws a string exception. If the value
 * occurs multiple times in the queue, the first occurrence's priority is
 * updated.
 * O(N)
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int foundvalueindex;
    bool notfound = true;

    for(int i=0; i<priorityqueue.size(); i++){
        if(priorityqueue[i].value == value){
            foundvalueindex = i;
            notfound = false;
            break;
        }
    }

    if(notfound){
        throw("Value Not Present");
    }

    if(priorityqueue[foundvalueindex].priority<=newPriority){
        throw("Not more Urgent Priority");
    }else{
        priorityqueue[foundvalueindex].priority = newPriority;
    }
}

/* The clear function removes all PQEntries from the priority queue.
 * O(1)
 */
void VectorPriorityQueue::clear() {
    priorityqueue.clear();
}

/* The dequeue function finds the entry with the most urgent priority in the
 * priority queue. It removes this entry and returns its string value. If the
 * priority queue is empty, the function throws an exception.
 * O(1)
 */
string VectorPriorityQueue::dequeue() {
    if(priorityqueue.isEmpty()){
        throw("Empty Queue");
   }else{
      int largestpriorityindex = mostUrgentIndex();
      string dequeuevalue = priorityqueue[largestpriorityindex].value;
      priorityqueue.remove(largestpriorityindex);
      return dequeuevalue;
   }
}

/* The mostUrgentIndex function finds the index of the entry with the
 * greatest priority. It is helpful for the dequeue, peek, and peekPriority
 * functions.
 * O(N)
 */
int VectorPriorityQueue::mostUrgentIndex() const{
    int mostUrgent = 0;
    for(int i=0; i<priorityqueue.size(); i++){
        PQEntry entry = priorityqueue[i];
        if(entry.priority < priorityqueue[mostUrgent].priority){
            mostUrgent = i;
        }
    }
    return mostUrgent;
}

/* The enqueue functions adds a PQEntry to the priority queue.
 * O(1)
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    priorityqueue.add(PQEntry(value, priority));
}

/* The isEmpty function determines if a priority queue contains
 * no entries. It returns true if it has no entries and false if
 * it does.
 * O(1)
 */
bool VectorPriorityQueue::isEmpty() const {
    return priorityqueue.isEmpty();
}

/* The peek function finds the entry with the most urgent priority and
 * it returns the value of this entry. If the priority queue is empty,
 * the function throws an exception.
 * O(1)
 */
string VectorPriorityQueue::peek() const {
    if(priorityqueue.isEmpty()){
        throw("Empty Queue");
    }else{
        int largestpriorityindex = mostUrgentIndex();
        return priorityqueue[largestpriorityindex].value;
    }
}

/* The peekPriority function finds the entry with the most urgent priority
 * and it returns the priority of this entry. If the priority queue is empty,
 * the function throws an exception.
 * O(1)
 */
int VectorPriorityQueue::peekPriority() const {
    if(priorityqueue.isEmpty()){
        throw("Empty Queue");
    }else{
        int largestpriorityindex = mostUrgentIndex();
        return priorityqueue[largestpriorityindex].priority;
    }
}

/* The size function returns the number of entries in the priority queue.
 * O(1)
 */
int VectorPriorityQueue::size() const {
    return priorityqueue.size();
}

/* The getPriorityQueue function returns the current Vector of PQEntries
 * constituting the priority queue.
 * O(1)
 */
Vector<PQEntry> VectorPriorityQueue::getPriorityQueue() const{
    return priorityqueue;
}

/* The operator prints out the present state of the priority queue, with
 * each value and corresponding priority.
 * O(N)
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    Vector<PQEntry> priorityqueue = queue.getPriorityQueue();
    out << "{";
    for(int i=0; i<priorityqueue.size(); i++){
        PQEntry entry = priorityqueue[i];
        out << entry;
        if(i!=priorityqueue.size()-1){
            out<<", ";
        }
    }
    out<< "}";
    return out;
}
