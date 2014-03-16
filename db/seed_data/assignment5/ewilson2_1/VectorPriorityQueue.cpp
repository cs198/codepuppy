// This file implements the vector version of a priority queue.

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {

}

VectorPriorityQueue::~VectorPriorityQueue() {
    clear();
}

//This method changes the priority of a given entry
//This runs in O(N) time
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if(vec.size()>0) {
        for(int i=0; i<vec.size(); i++) {
            PQEntry current=vec[i];
            if(current.value==value) {
                current.priority=newPriority;
                vec[i]=current;
            }
        }
    }

}

//This method clears the queue
//This runs in O(N) time
void VectorPriorityQueue::clear() {
    while(vec.size()>0) {
        vec.remove(0);
    }

}

//This method returns the value of the most-urgent entry and removes it from the queue
//This runs in O(N) time
string VectorPriorityQueue::dequeue() {
    if(!isEmpty()) {
        int returnIndex=firstPriorityIndex();
        PQEntry returnEntry=vec[returnIndex];
        string returnValue=returnEntry.value;
        vec.remove(returnIndex);
        return returnValue;
    } else {
        return "This queue is empty";
    }

}

//This method returns the index of the most-urgent entry
//This runs in O(N) time
int VectorPriorityQueue::firstPriorityIndex() const {
    if(vec.size()>0) {
        PQEntry returnEntry=vec[0];
        string returnValue=returnEntry.value;
        int returnPriority=returnEntry.priority;
        int returnIndex=0;
        for(int i=1; i<vec.size(); i++) {
            PQEntry currentEntry=vec[i];
            int currentPriority=currentEntry.priority;
            string currentValue=currentEntry.value;
            if(currentPriority==returnPriority) {
                if(currentValue<returnValue) {
                    returnValue=currentValue;
                    returnPriority=currentPriority;
                    returnIndex=i;
                }
            }
            if(currentPriority<returnPriority) {
                returnValue=currentValue;
                returnPriority=currentPriority;
                returnIndex=i;
            }
        }
        return returnIndex;
    }
}

//This method adds a new entry to the queue
//This runs in O(1) time
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry=PQEntry(value, priority);
    vec.add(newEntry);
}

//This method returns a boolean indicating whether or not the queue is empty
//This runs in 0(1) time
bool VectorPriorityQueue::isEmpty() const {
    if(vec.size()==0) {
        return true;
    }
    return false;
}

//This method returns the value of the most-urgent entry without dequeueing it
//This runs in o(N) time
string VectorPriorityQueue::peek() const {
    if(vec.size()>0) {
        int returnIndex=firstPriorityIndex();
        PQEntry returnEntry=vec[returnIndex];
        string returnValue=returnEntry.value;
        return returnValue;
    }
    return "";
}

//This method returns the priority of the most-urgent entry without dequeueing it
//This runs in O(N) time
int VectorPriorityQueue::peekPriority() const {
    if(!isEmpty()) {
        int returnIndex=firstPriorityIndex();
        PQEntry returnEntry=vec[returnIndex];
        int returnPriority=returnEntry.priority;
        return returnPriority;
    } else {
        return NULL;
    }
}

//This method returns the size of the queue
//This runs in O(N) time
int VectorPriorityQueue::size() const {
    return vec.size();
}

//This method overloads the "<<" operator to print out the contents of a queue
//This runs in O(N) time
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    cout << "{";
    for(int i=0; i<queue.vec.size(); i++) {
        PQEntry current=queue.vec[i];
        string val=current.value;
        int prior=current.priority;
        cout << "\"" << val << "\"";
        cout << ":";
        cout << prior;
        cout << " ";
    }
    cout << "}" << endl;
    return out;
}
