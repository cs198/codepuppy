//File: trailblazer.cpp
//Author: Mark Stauber
//Description: This file contains the functions for the path search and maze creation
//algorithms for the trailblazer app.
#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;

//Helper functions for path search algorithms
bool dfsHelper(Stack<Vertex*>& path, Vertex* v1, Vertex* end);
void getPath(Vector<Vertex*>& path, Vertex* pathEnd);
bool dijAstarHelper(BasicGraph& graph, Vertex* start, Vertex* end,
                    double (*heuristic)(Vertex* from, Vertex* to), Vertex** pathEnd);

//Placeholder heuristic function for dijkstras algorithm. Returns 0.
double dijkstrasHeuristic(Vertex* from, Vertex* to);

//Implements a depth first search from start to end and returns an vector
//of the path ordered from start to end. If no path returns empty vector.
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Stack<Vertex*> pathStack;
    Vector<Vertex*> path;

    //Call recursive dfsHelper to search for path.
    if(dfsHelper(pathStack, start, end)){
        while(!pathStack.isEmpty()){
            path.push_back(pathStack.pop());
        }
    }
    return path;
}

//Implements a depth first search from start to end and returns an vector
//of the path ordered from start to end. If no path returns empty vector.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Queue<Vertex*> pathQueue;
    Vertex* pathEnd;
    bool found = false;

    //Put start in pathQueue
    pathQueue.enqueue(start);
    start->visited = true;
    start->setColor(YELLOW);

    //Continue bfs until queue is empty (or path is found)
    while(!pathQueue.isEmpty()){
        Vertex* v = pathQueue.dequeue();
        v->setColor(GREEN);
        if(v == end){
            found = true;
            pathEnd = v;
            break;
        }
        //For each edge in v
        for(Edge* i : v->edges){
            //Check if start or finish was visited
            if(!i->start->visited){
                Vertex* vNext = i->start;
                vNext->visited = true;
                vNext->setColor(YELLOW);
                vNext->previous = v;
                pathQueue.enqueue(vNext);
            }else if(!i->finish->visited){
                Vertex* vNext = i->finish;
                vNext->visited = true;
                vNext->setColor(YELLOW);
                vNext->previous = v;
                pathQueue.enqueue(vNext);
            }
        }
    }

    //If found, compile the found path into path.
    Vector<Vertex*> path;
    if(found) getPath(path, pathEnd);
    return path;
}

//Implements dijkstras algorithm to find path from start to end and returns an vector
//of the path ordered from start to end. If no path returns empty vector.
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    //Pass address of pathEnd to helper function and search for path.
    Vertex* pathEnd;
    bool found = dijAstarHelper(graph, start, end, dijkstrasHeuristic, &pathEnd);

    //If found, compile the found path into path.
    Vector<Vertex*> path;
    if(found) getPath(path, pathEnd);
    return path;
}

//Implements aStar algorithm to find path from start to end and returns an vector
//of the path ordered from start to end. If no path returns empty vector.
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    //Pass address of pathEnd to helper function and search for path.
    Vertex* pathEnd;
    bool found = dijAstarHelper(graph, start, end, heuristicFunction, &pathEnd);

    //If found, compile the found path into path.
    Vector<Vertex*> path;
    if(found) getPath(path, pathEnd);
    return path;
}

//Implements kruskal's algorithm to build a minimum spanning tree for graph
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    //Initialize all clusters with one vertex each.
    Set<Set<Vertex*>*> clusterSet;
    for(Vertex* i : graph.getVertexSet()){
        Set<Vertex*>* thisCluster = new Set<Vertex*>;
        thisCluster->add(i);
        clusterSet.add(thisCluster);
    }

    //Engueue each edge w/ priority = cost
    PriorityQueue<Edge*> edgePQ;
    for(Edge* i : graph.getEdgeSet()){
        edgePQ.enqueue(i, i->cost);
    }

    //As long as there are 2 or more clusters.
    while(clusterSet.size() > 1){
        Edge* e = edgePQ.dequeue();
        Vertex* v1 = e->start;
        Vertex* v2 = e->finish;
        Set<Vertex*>* v1Cluster;
        Set<Vertex*>* v2Cluster;
        bool foundv1 = false;
        bool foundv2 = false;
        //For each cluster
        for(Set<Vertex*>* i : clusterSet){
            //Check if v1 or v2 are in the cluster.
            if(foundv1 = i->contains(v1)){
                v1Cluster = i;
            }
            if((foundv2 = i->contains(v2))){
                v2Cluster = i;
            }
            //If we found v1 and v2 then we can stop looking through clusters.
            if((foundv1 && foundv2)) break;
        }
        //If v1's cluster is different than v2's cluster, combine the clusters and add edge to mst.
        if(v1Cluster != v2Cluster){
            Set<Vertex*>* newCluster = new Set<Vertex*>;
            *newCluster = *v1Cluster + *v2Cluster;
            clusterSet.remove(v1Cluster);
            clusterSet.remove(v2Cluster);
            clusterSet.add(newCluster);
            mst.add(e);
            delete v1Cluster;
            delete v2Cluster;
        }
    }

    delete clusterSet.first(); //Deletes the last instance of new Set for memory management.
    return mst;
}

//Recursive helper function for dfs. Returns true if path is found.
bool dfsHelper(Stack<Vertex*>& path, Vertex* v1, Vertex* end){
    path.push(v1);
    v1->visited = true;
    v1->setColor(GREEN);
    if(v1 == end) return true;

    for(Edge* i : v1->edges){
        if(!i->start->visited){
            if(dfsHelper(path, i->start, end)) return true;
        }else if(!i->finish->visited){
            if(dfsHelper(path, i->finish, end)) return true;
        }
    }
    v1->setColor(GRAY);
    path.pop();
    return false;
}

//Generic helper function for both dijkstra's and a* algorithms.
bool dijAstarHelper(BasicGraph& graph, Vertex* start, Vertex* end,
                    double (*heuristic)(Vertex* from, Vertex* to), Vertex** pathEnd){
    PriorityQueue<Vertex*> pathPQ;
    bool found = false;

    //Initialize all vertex to infinite cost.
    for(Vertex* i : graph.getVertexSet()){
        i->cost = POSITIVE_INFINITY;
    }

    //set start cost to 0 and enqueue it using heuristic function.
    start->cost = 0;
    start->setColor(YELLOW);
    pathPQ.enqueue(start, heuristic(start, end));

    //Until patPQ is empty or path is found.
    while(!pathPQ.isEmpty()){
        Vertex* v = pathPQ.dequeue();
        v->setColor(GREEN);
        v->visited = true;
        if(v == end){
            found = true;
            *pathEnd = v;
            break;
        }

        //For each edge coming from v.
        for(Edge* i : v->edges){
            //Check if edge connects to non-visited vNext and if cost < vNext cost.
            if(!i->start->visited){
                Vertex* vNext = i->start;
                double cost = v->cost + i->cost;
                if(cost < vNext->cost){
                    vNext->cost = cost;
                    vNext->previous = v;
                    if(vNext->getColor() == YELLOW){
                        pathPQ.changePriority(vNext, cost + heuristic(vNext, end));
                    }else{
                        vNext->setColor(YELLOW);
                        pathPQ.enqueue(vNext, cost + heuristic(vNext, end));
                    }
                }
            }else if(!i->finish->visited){
                Vertex* vNext = i->finish;
                double cost = v->cost + i->cost;
                if(cost < vNext->cost){
                    vNext->cost = cost;
                    vNext->previous = v;
                    if(vNext->getColor() == YELLOW){
                        pathPQ.changePriority(vNext, cost + heuristic(vNext, end));
                    }else{
                        vNext->setColor(YELLOW);
                        pathPQ.enqueue(vNext, cost + heuristic(vNext, end));
                    }
                }
            }
        }
    }
    return found;
}

//Compiles the path from pathEnd using vertex's previous member variable.
void getPath(Vector<Vertex*>& path, Vertex* pathEnd){
    Vertex* v = pathEnd;
    path.push_back(v);
    while(v->previous != NULL){
        v = v->previous;
        path.push_back(v);
    }

}

//Placeholder heuristic function for dijkstra's algorithm.
double dijkstrasHeuristic(Vertex* from, Vertex* to){
    return (double)0;
}

