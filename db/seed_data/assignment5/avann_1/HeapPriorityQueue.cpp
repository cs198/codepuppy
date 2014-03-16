//This is the Heap implementation of a Priority Queue for CS106B
//Assignment 5. This is the most efficient implementation of a
//priority queue.

#include "HeapPriorityQueue.h"
#include <algorithm>
#include "strlib.h"

HeapPriorityQueue::HeapPriorityQueue() {
    queuecapacity = 10;
    queuesize = 0;
    priorityqueue = new PQEntry[queuecapacity];
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete []priorityqueue;
}

/* See VectorPriorityQueue.h
 * O(N^2)
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool notfound = true;
    int foundindex=0;

    for(int i=1; i<=queuesize; i++){
        if(priorityqueue[i].value == value){
            notfound = false;
            foundindex = i;
            break;
        }
    }

    if(notfound) throw("Value not present");
    if(priorityqueue[foundindex].priority <= newPriority){
        throw("Not more urgent priority");
    }else{
        priorityqueue[foundindex].priority = newPriority;
        while(priorityqueue[foundindex/2].priority > priorityqueue[foundindex].priority){
            std::swap(priorityqueue[foundindex/2], priorityqueue[foundindex]);
            foundindex /= 2;
        }
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
void HeapPriorityQueue::clear() {
    queuesize = 0;
}

/* See VectorPriorityQueue.h
 * O(N)
 */
string HeapPriorityQueue::dequeue() {
    if(queuesize==0){
        throw("Empty queue");
    }else{
        string mosturgent = priorityqueue[1].value;

        std::swap(priorityqueue[1], priorityqueue[queuesize]);
        queuesize--;
        int bubbledownindex = 1;

        while ( (bubbledownindex*2 <= queuesize || (bubbledownindex*2)+1 <= queuesize) &&
                (priorityqueue[bubbledownindex*2].priority < priorityqueue[bubbledownindex].priority ||
                 priorityqueue[(bubbledownindex*2)+1].priority < priorityqueue[bubbledownindex].priority) ){

            bool firstchild = false;
            int mosturgentchild;

            if(bubbledownindex*2 <= queuesize && priorityqueue[bubbledownindex*2].priority < priorityqueue[bubbledownindex].priority){
                mosturgentchild = bubbledownindex*2;
                firstchild = true;
            }
            if((bubbledownindex*2)+1 <= queuesize){
                if(firstchild){
                    if(priorityqueue[mosturgentchild].priority > priorityqueue[(bubbledownindex*2)+1].priority){
                        mosturgentchild = bubbledownindex*2 +1;
                    }
                }else{
                    mosturgentchild = bubbledownindex*2 +1;
                }
            }

            std::swap(priorityqueue[bubbledownindex], priorityqueue[mosturgentchild]);

            bubbledownindex = mosturgentchild;
        }

        return mosturgent;
    }
}

/* See VectorPriorityQueue.h
 * O(N^2)
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry newentry =  PQEntry(value, priority);
    if(queuesize==0){
        priorityqueue[1] = newentry;
        queuesize++;
    }else{
        if(queuesize+1 < queuecapacity){
            queuesize++;
            priorityqueue[queuesize] = newentry;
        }else{
            queuecapacity *= 2;
            PQEntry* newarray = new PQEntry[queuecapacity];
            for(int i=0; i<queuesize; i++){
                newarray[i]= priorityqueue[i];
            }
            delete []priorityqueue;
            priorityqueue = newarray;
            priorityqueue[queuesize+1]= newentry;
            queuesize++;
        }

        int childindex = queuesize;
        while(priorityqueue[childindex/2].priority > priorityqueue[childindex].priority){
            std::swap(priorityqueue[childindex/2], priorityqueue[childindex]);
            childindex /= 2;
        }
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
bool HeapPriorityQueue::isEmpty() const {
    return queuesize==0;
}

/* See VectorPriorityQueue.h
 * O(1)
 */
string HeapPriorityQueue::peek() const {
    if(queuesize==0){
        throw("Empty queue");
    }else{
        return priorityqueue[1].value;
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
int HeapPriorityQueue::peekPriority() const {
    if(queuesize==0){
        throw("Empty queue");
    }else{
        return priorityqueue[1].priority;
    }
}

/* See VectorPriorityQueue.h
 * O(1)
 */
int HeapPriorityQueue::size() const {
    return queuesize;
}

/* See VectorPriorityQueue.h
 * O(1)
 */
PQEntry* HeapPriorityQueue::getPriorityQueue() const{
    return priorityqueue;
}

/* See VectorPriorityQueue.h
 * O(N)
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    PQEntry* array = queue.getPriorityQueue();
    int arraysize = queue.size();
    out << "{";
    for(int i=1; i<=arraysize; i++){
        PQEntry entry = array[i];
        out << entry;
        if(i!=arraysize){
            out << ", ";
        }
    }
    out << "}";
    return out;
}
