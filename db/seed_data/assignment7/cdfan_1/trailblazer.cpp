//This program implements multiple graph algorithm, including DFS, BFS, dijkstras, A*
//Kruskal
#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"

using namespace std;

//prototypes:
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> &path);
void backTracePath(Vertex* end, Vector<Vertex*> &path);
Vector<Vertex*> dijkstrasHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool h);
void exploreNeighbors(Vertex* & v, Vertex* end, PriorityQueue<Vertex*> & pq, bool h );

//It searches a path in a graph by explore one path until it finds the path, or
//recursively backtracking other possibilities.
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(graph, start, end, path);
    return path;
}

//The helper function for the recurssion in DFS. If the start equals the end, it returns
// return true, otherwise continue exploring recursively.
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> &path) {
    if(start->visited) return false;
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    if(start == end) return true;
    for(Edge* edge: start->edges) {
        if(depthFirstSearchHelper(graph, edge->finish, end, path)) return true;
    }
    Vertex *giveUp = path[path.size()-1];
    giveUp->setColor(GRAY);
    path.remove(path.size()-1);
    return false;
}

//BFS searches the path by exploring all unvisited neighbros one hop at a time, until
//it finds the end Vertex and trace back the path through the previous pointers.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    queue.enqueue(start);
    start->visited =true;
    start->setColor(GREEN);
    while(!queue.isEmpty()) {
        Vertex *v = queue.dequeue();
        v->setColor(GREEN);
        if(v == end) {
            backTracePath(end, path);
            return path;
        }
        for(Edge* edge: v->edges) {
            if(!edge->finish->visited) {
                queue.enqueue(edge->finish);
                edge->finish->previous = v;
                edge->finish->setColor(YELLOW);
                edge->finish->visited = true;
            }
        }
    }
    return path;
}

//Trace the path back through the previous pointers in Verteces. IT's used in
// DFS,dijkstras, and A*.
void backTracePath(Vertex* end, Vector<Vertex*> &path) {
    Vertex *current = end;
    while(current!= NULL) {
        path.add(current);
        current = current->previous;
    }
}

//Implement dijkstras algorith to find a path with lowest cost
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    bool h = false;
    return dijkstrasHelper(graph, start, end, h);
}

//a helper function used by both Dijkstra and A* algorithm with an extra bool value
//to indicate whether hueristics is on or off
Vector<Vertex*> dijkstrasHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool h) {
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    for(Vertex* v: graph.getVertexSet()) v->cost = POSITIVE_INFINITY;
    start->cost = 0;
    pq.enqueue(start,0);
    start->visited = true;
    while(!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->visited =true;
        v->setColor(GREEN);
        if(v == end) {
            backTracePath(end, path);
            return path;
        }
        exploreNeighbors(v, end, pq, h);
    }
    return path;
}

//A helpter function for dijkstras algorithm. It explore all the unvisited neighbors
//to determine whether to enqueue or change priority in the pq.
// Once it does, it also set the previous pointer of the vertex of edge->finish to v
void exploreNeighbors(Vertex* & v, Vertex* end, PriorityQueue<Vertex*> & pq, bool h ) {
    for(Edge*edge: v->edges) {
        if(edge->finish->visited) continue;
        double cost = edge->cost + v->cost;
        double priority = cost;
        if(h) priority += heuristicFunction(edge->finish, end);
        if(cost < edge->finish->cost) {
            if(edge->finish->cost != POSITIVE_INFINITY) {
                pq.changePriority(edge->finish,priority);
            }else {
                pq.enqueue(edge->finish,priority);
            }
            edge->finish->setColor(YELLOW);
            edge->finish->cost = cost;
            edge->finish->previous = v;
        }
    }
}

//Implement A* Algorithm by calling dijkstrasHelper, with the Hueristics true to
// take educated guesses on where to find the path
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    bool h = true;
    return dijkstrasHelper(graph, start, end, h);
}

//Implemets Kruskal algorithm by putting all edges of the graph into a priority queue, clearing
//all the edges from the graph;Put all the verteces into its own cluster.
//Build a Map<Vertex*, Set<Vertex*>* >, a map of Verteces asociated with the pointer pointing
//to the Set<Vertex*> where the vertex is in. As edges are dequeued from the pq, if its
//start and finish don't belong to the same set, merge them, and update both the Set, Set
//pointers and the Map. Free the memory holding discarded Sets.
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    PriorityQueue<Edge*> pq;
    for(Edge* edge: graph.getEdgeSet()) pq.enqueue(edge,edge->cost);
    graph.clearEdges();
    //A map with Verteces as keys, and the pointer to the cluster Set this Vertex is in as its value
    Map<Vertex*, Set<Vertex*>* > clusters;
    for(Vertex* v: graph.getVertexSet()) {
        Set<Vertex*>* cluster = new Set<Vertex*>;
        *cluster += v;
        clusters[v] = cluster;
    }

    int nOfClusters = clusters.size();//keep track of number of clusters
    //As long as there are more than one cluster, keep looping through to find the MST
    while(nOfClusters > 1) {
        Edge* e = pq.dequeue();
        if(clusters[e->start] != clusters[e->finish]) {
            *clusters[e->finish] += *clusters[e->start];//merge the two sets
            Set<Vertex*>* trash = clusters[e->start];//start the pointer to be deleted later
            for(Vertex* movedV: *clusters[e->start]) {
                clusters[movedV] = clusters[e->finish];//for each moved vertex, it resets it's Map value into the new Set address
            }
            delete trash;
            mst += e;//add this edge into mst
            nOfClusters--;//cluster number decrease by one after each merging of set
        }
    }
    return mst;
}

