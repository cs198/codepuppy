// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "basicgraph.h"
#include "queue.h"
#include "pqueue.h"
#include "set.h"
#include "console.h"

// TODO: include any other headers you need; remove this comment
using namespace std;

bool DFShelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);

//DFS is guaranteed to find a path if one exisits
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    DFShelper(graph, start, end, path);
    return path;
}

bool DFShelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path){
    path.add(start);
    start->visited = true; // mark start as visited
    start->setColor(GREEN); // set it to green
    if(start->name == end->name){
        return true; // a path is found
    }else{
        Set<Vertex*> neighbourSet = graph.getNeighbors(start);
            for (Vertex* neighbour : neighbourSet){ // for each unvisited neighbor n of v1
                if(!neighbour->visited){
                if(DFShelper(graph, neighbour, end, path)){ // recursion
                    return true; // path found
                }
            }
        }
    }
    path[path.size()-1]->setColor(GRAY);
    path.remove(path.size()-1);
    return false; // no path found
}

//BFS finds a pth between two nodes by taking one step down all paths
// and then immediately backtracking.
//BFS always returns the shortest path.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> q;
    q.enqueue(start); // put start in the priority queue
    start->visited = true; // mark v1 as visited
    start->setColor(YELLOW); // set it yellow
    Vertex* lastVertex;
    while(!q.isEmpty()){ // while queue is not empty
        Vertex* currentVertex = q.dequeue(); //dequeue a vertex from queue
        currentVertex->visited = true; // set visisted
        currentVertex->setColor(GREEN); //set it to green
        if(currentVertex->name == end->name){ // if it's the end
            lastVertex = currentVertex->previous;
            path.add(currentVertex);
            break; // found!
        }else{
           Set<Vertex*> neighbourSet = graph.getNeighbors(currentVertex);
           for (Vertex* neighbour : neighbourSet){ // for each unvisited neighbor
               if(!neighbour->visited) {
                   neighbour->previous = currentVertex;
                   neighbour->visited = true; // make the neighbour as visited
                   q.enqueue(neighbour); // enqueueu it in the queue
                   neighbour->setColor(YELLOW);
               }
           }
        }
    }
   while(lastVertex->name != start->name){
   path.add(lastVertex);
    lastVertex = lastVertex->previous;
    }
   path.add(start); // add the start
    return path;
}

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    //initialize every node to have cost infinity
    Set<Vertex*> vertexSet = graph.getVertexSet();
    for(Vertex* currentVertex : vertexSet){
        currentVertex->cost = POSITIVE_INFINITY;
    }
    // set start's cost to 0
    start->cost = 0;
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start, 0);//enqueue the start to pq
    Set<Vertex*> s; // use a set to check the vertex existance in the pq
    s.add(start);
    start->setColor(YELLOW); //set it yellow
    Vertex* lastVertex;
    while(!pq.isEmpty()){
        Vertex* currentVertex = pq.dequeue(); // dequeue
        s.remove(currentVertex);
        currentVertex->setColor(GREEN); // set dequeue vertex to green
        currentVertex->visited = true; // mark it visited
        if(currentVertex == end){ // if it reaches the end, stop
            lastVertex = currentVertex->previous;
            path.add(currentVertex);
            break;
        }else{
            Set<Vertex*> neighbourSet = graph.getNeighbors(currentVertex);
            for (Vertex* neighbour : neighbourSet){
               if(!neighbour->visited) { // for each unvisited neighbor n
                   Edge* edge = graph.getEdge(neighbour,currentVertex);
                   double cost = currentVertex->cost + edge->cost; // cost = v cost + weight of edge(v,n)
                   if(cost < neighbour->cost){ // if cost < n cost
                       neighbour->cost = cost;
                       neighbour->previous = currentVertex;
                       if(s.contains(neighbour)){ // if neighbour already exists
                           pq.changePriority(neighbour,cost); // change the cost
                       }else{ // if it doesn't exist
                           pq.enqueue(neighbour,cost); // enqueue the new neighbor and cost
                           s.add(neighbour);
                           neighbour->setColor(YELLOW);
                       }
                   }
               }
            }
        }

    }
    while(lastVertex->name != start->name){
    path.add(lastVertex);
     lastVertex = lastVertex->previous;
     }
    path.add(start);
    return path;
}

//A* is modified version of Dijkstra's algorithm that uses a heuristic function
//to guide its order of path exploration
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    //initialize every node to have cost infinity
    Set<Vertex*> vertexSet = graph.getVertexSet();
    for(Vertex* currentVertex : vertexSet){
        currentVertex->cost = POSITIVE_INFINITY;
    }
    // set start's cost to 0
    start->cost = 0;
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start, heuristicFunction(start, end));//enqueue the start to pq
    Set<Vertex*> s; // use a set to check the vertex existance in the pq
    s.add(start);
    start->setColor(YELLOW); //set it yellow
    Vertex* lastVertex;
    while(!pq.isEmpty()){
        Vertex* currentVertex = pq.dequeue(); // dequeue
        s.remove(currentVertex);
        currentVertex->setColor(GREEN); // set dequeue vertex to green
        currentVertex->visited = true; // mark it visited
        if(currentVertex == end){
            lastVertex = currentVertex->previous;
            path.add(currentVertex);
            break;
        }else{
            Set<Vertex*> neighbourSet = graph.getNeighbors(currentVertex);
            for (Vertex* neighbour : neighbourSet){
               if(!neighbour->visited) {
                   Edge* edge = graph.getEdge(neighbour,currentVertex);
                   double cost = currentVertex->cost + edge->cost;
                   if(cost < neighbour->cost){
                       neighbour->cost = cost;
                       neighbour->previous = currentVertex;
                       if(s.contains(neighbour)){
                           pq.changePriority(neighbour,cost+heuristicFunction(neighbour, end)); // use heuristic function
                       }else{
                           pq.enqueue(neighbour,cost+heuristicFunction(neighbour, end)); // use heuristic function
                           s.add(neighbour);
                           neighbour->setColor(YELLOW);
                       }
                   }
               }
            }
        }

    }
    while(lastVertex->name != start->name){
    path.add(lastVertex);
     lastVertex = lastVertex->previous;
     }
    path.add(start);
    return path;
}

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Set<Vertex*> vertexSet = graph.getVertexSet();
    Map<Vertex*, Set<Vertex*>*> m;
    for(Vertex* vertex : vertexSet){ //put all nodes into their own cluster
        Set<Vertex*>* cluster = new Set<Vertex*>();
        cluster->add(vertex);
        m.put(vertex, cluster);
    }
    Set<Edge*> edgeSet = graph.getEdgeSet();
    PriorityQueue<Edge*> pq;
    for(Edge* edge : edgeSet){
        pq.enqueue(edge,edge->cost); // create a PQ of the edges order by their cost
    }
    while(!pq.isEmpty()){ //while pq is not empty
        Edge* e = pq.dequeue(); // remove the edge
        if(m.get(e->start) != m.get(e->finish)){ // if the vertices of the edge are not in the same cluster
           mst.add(e);
           Set<Vertex*>* fvCluster = m.get(e->finish);
           for(Vertex* v : *fvCluster){ //merge the two clusters
               m.get(e->start)->add(v);
           }
           for(Vertex* v : *fvCluster){
               m[v] = m.get(e->start);
           }
        }
    }
    return mst;
}




















