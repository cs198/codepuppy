// This is the CPP file for the Trailblazer assignment.

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;
/**
 * @brief dFSHelper
 * @param graph
 * @param start
 * @param end
 * @param path
 * @return
 * This is the recursive helper for depthFirstSearch.
 */
bool dFSHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path){
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    int i = path.size() - 1;
    if(start == end){
        return true;
    }
    for(Vertex* next: graph.getNeighbors(start)){
        if(!next->visited){
            next->visited = true;
            next->setColor(YELLOW);
            if(dFSHelper(graph, next, end, path)){
                return true;
            }
        }
    }
    path.remove(i);
    return false;
}
/**
 * @brief depthFirstSearch
 * @param graph
 * @param start
 * @param end
 * @return
 * depthFirstSearch uses recursion to find the path from the start to the end.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    start->setColor(YELLOW);
    dFSHelper(graph, start, end, path);
    return path;

}

/**
 * @brief breadthFirstSearch
 * @param graph
 * @param start
 * @param end
 * @return
 * breadthFirstSearch uses a queue to find a path from the
 * start to the end.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Queue<Vertex*> dFSQueue;
    Vector<Vertex*> path;
    dFSQueue.enqueue(start);
    start->visited = true;
    while(!dFSQueue.isEmpty()){
        Vertex* current = dFSQueue.dequeue();
        current->setColor(GREEN);
        if(current == end){
            path.add(current);
            break;
        }
        for(Vertex* next: graph.getNeighbors(current)){
            if(!next->visited){
                next->visited = true;
                next->previous = current;
                dFSQueue.enqueue(next);
                next->setColor(YELLOW);
            }
        }
    }
    Vertex* current = path.get(0);
    while(current->previous != NULL){
        path.add(current->previous);
        current = current->previous;
    }
    return path;
}

/**
 * @brief dijkstrasAlgorithm
 * @param graph
 * @param start
 * @param end
 * @return
 * dijkstrasAlgorithm is similar to breadthFirstSearch except that it
 * uses a priority queue to get from the start to the end.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    for(Vertex* current: graph.getVertexSet()){
        current->cost = POSITIVE_INFINITY;
    }
    PriorityQueue<Vertex*> queue;
    queue.enqueue(start, 0);
    Vector<Vertex*> path;
    while(!queue.isEmpty()){
        Vertex* top = queue.dequeue();
        top->visited = true;
        top->setColor(GREEN);
        if(top == end){
            path.add(top);
            break;
        }
        for(Vertex* next: graph.getNeighbors(top)){
            if(!next->visited){
                next->cost = top->cost + graph.getEdge(top, next)->cost;
                next->previous = top;
                next->setColor(YELLOW);
                bool check = false;
                PriorityQueue<Vertex*> queueTemp;
                while(!queue.isEmpty()){
                    Vertex* trial = queue.dequeue();
                    queueTemp.enqueue(trial, trial->cost);
                    if(trial == next){
                        check = true;
                    }
                }
                queue = queueTemp;\
                if(check){
                    queue.changePriority(next, next->cost);
                }else{
                    queue.enqueue(next, next->cost);
                }
            }
        }
    }
    Vertex* current = path.get(0);
    while(current->previous != NULL){
        path.add(current->previous);
        current = current->previous;
    }
    return path;
}
/**
 * @brief aStar
 * @param graph
 * @param start
 * @param end
 * @return
 * aStar is almost the exact same as dijkstas except that it utilizes
 * a heuristicFunction to be more efficient.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    for(Vertex* current: graph.getVertexSet()){
        current->cost = POSITIVE_INFINITY;
    }
    PriorityQueue<Vertex*> queue;
    queue.enqueue(start, 0 + heuristicFunction(start, end));
    Vector<Vertex*> path;
    while(!queue.isEmpty()){
        Vertex* top = queue.dequeue();
        top->visited = true;
        top->setColor(GREEN);
        if(top == end){
            path.add(top);
            break;
        }
        for(Vertex* next: graph.getNeighbors(top)){
            if(!next->visited){
                next->cost = top->cost + graph.getEdge(top, next)->cost;
                next->previous = top;
                next->setColor(YELLOW);
                bool check = false;
                PriorityQueue<Vertex*> queueTemp;
                while(!queue.isEmpty()){
                    Vertex* trial = queue.dequeue();
                    queueTemp.enqueue(trial, trial->cost + heuristicFunction(trial, end));
                    if(trial == next){
                        check = true;
                    }
                }
                queue = queueTemp;\
                if(check){
                        queue.changePriority(next, next->cost + heuristicFunction(next, end));
               }else{
                    queue.enqueue(next, next->cost + heuristicFunction(next, end));
                }
            }
        }
    }
    Vertex* current = path.get(0);
    while(current->previous != NULL){
        path.add(current->previous);
        current = current->previous;
    }
    return path;
}

Set<Edge*> kruskal(BasicGraph& graph) {
    Map<Vertex*, Set<Vertex*>*> mapCluster;

    PriorityQueue<Edge*> queue;
    for(Edge* current: graph.getEdgeSet()){
        queue.enqueue(current, current->cost);
    }

    for(Vertex* current: graph.getVertexSet()){
        Set<Vertex*>* currentSet = new Set<Vertex*>();
        currentSet->add(current);
        mapCluster.put(current, currentSet);
    }

    Set<Edge*> mst;
    while(!queue.isEmpty()){
        Edge* current = queue.dequeue();
        if(mapCluster.get(current->start) != mapCluster.get(current->finish)){
            Set<Vertex*>* currentStart = mapCluster.get(current->start);
            Set<Vertex*>* currentFinish = mapCluster.get(current->finish);
            currentStart->addAll(*currentFinish);
            mst.add(current);
            for(Vertex* test: mapCluster){
                if(mapCluster.get(test) == currentFinish){
                    mapCluster[test] = currentStart;
                }
            }
        }

    }
    return mst;
}
