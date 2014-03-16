/*
 * Trailblazer code
 * Andrés Camperi
 * SL: Jimmy Lee
 */

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "stack.h"
#include "pqueue.h"
#include "hashset.h"

using namespace std;

//recursive function for the depth search
void depthRecursion (BasicGraph& graph , Vertex* start , Vertex* end , Vector<Vertex*>& path , bool& found){
    start->visited = true; //sets start (current vertex) to visited
    path.add(start); //adds start (current) to the vector
    start->setColor(GREEN);

    if (start == end){ //if we're at the end
        found = true; //boolean to halt the program when we've found it
        return;
    }

    for (Edge* edge : start->edges){ //iterates over the edges
        if (edge->finish->visited == false && !found){ //if it's not visited and we haven't found the end
            depthRecursion(graph , edge->finish , end , path , found); //recurse
        }
    }
    if (!found){ //if we still haven't found it
        path.get(path.size()-1)->setColor(GRAY); //set color to gray (abandoning the search)
        path.remove(path.size()-1); //remove the last element from the path
    }
}

//DFS
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets the graph
    Vector<Vertex*> path; //initializes the path
    bool found = false; //initializes a boolean that will halt the program once we've found the end
    depthRecursion (graph , start , end , path , found); //recursion helper
    return path;
}

//used to take the pointer to current vertex and trace back
//using previous pointers to the beginning
//then adds them all to the vector path
void traceBack (Vector<Vertex*>& path , Vertex* v){
    //we need to reverse the order, hence the stack
    Stack<Vertex*> stack;
    //adds all the vertices to the stack through previous pointers
    while (v->previous != NULL){
        stack.push(v);
        v = v->previous;
    }
    //reverses the order (by popping them)
    while (!stack.isEmpty()){
        path.add(stack.pop());
    }
}

//BFS
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets the graph

    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    queue.enqueue(start);
    start->visited = true;
    start->setColor(GREEN);
    path.add(start);

    //starts the while loop (while there are still vertices in the queue)
    while(!queue.isEmpty()){
        Vertex* v = queue.dequeue(); //dequeues
        v->setColor(GREEN);
        if (v == end){ //if the vertex is the same as the end
            traceBack(path , v); //runs the traceback functions with the previous pointers to add to the path
            break;
        } else{ //if we haven't found the end
            for (Edge* edge : v->edges){ //iterates through each edge connected to the current vertex
                if (edge->finish->visited == false){ //if the connected vertex hasn't been visited
                    edge->finish->visited = true; //set it to visited
                    edge->finish->previous = edge->start; //sets the previous pointer
                    queue.enqueue(edge->finish); //enqueues it
                    edge->finish->setColor(YELLOW); //sets its color to yellow
                }
            }
        }
    }
    return path;
}

//function to intialize all the vertices in the graph to infinity (used in dijkstra and a*)
void initVertices(BasicGraph& graph){
    for (Vertex* v : graph.getVertexSet()){
        v->cost = POSITIVE_INFINITY;
    }
}

//dijkstras algorithm
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets the graph

    initVertices(graph); //initializes all the vertices to cost infinity

    start->cost = 0; //set the cost of start to 0
    start->visited = true;
    start->setColor(GREEN);
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> queue;
    queue.enqueue(start , start->cost); //enqueues start with its cost
    path.add(start);

    //while there are still vertices in the queue
    while (!queue.isEmpty()){
        Vertex* v = queue.dequeue(); //dequeues the vertex with lowest priority
        v->setColor(GREEN); //sets its color
        if (v == end){ //if it matches the end
            traceBack(path , v); //we trace back with previous pointers
            break;
        }
        //if we haven't found the end
        for (Edge* edge : v->edges){
            //iterate over all the unvisited neighbors
            if (!edge->finish->visited){
                double cost = edge->cost + v->cost; //gets a variable cost equals to the previous vertex's + the edge's cost
                if (cost < edge->finish->cost){ //if the new cost is inferior to the vertex's cost
                    edge->finish->cost = cost; //set the vertex's cost to be the new cost
                    edge->finish->previous = edge->start; //set the previous pointer
                    if (edge->finish->getColor() == YELLOW){ //if it's yellow = if it's been enqueued already
                        queue.changePriority(edge->finish , cost); //change its priority
                    } else{ //if it hasn't been enqueued
                        queue.enqueue(edge->finish , cost); //enqueue it
                    }
                    edge->finish->setColor(YELLOW); //set it to yellow
                }
            }
        }
    }
    return path;
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets the graph

    initVertices(graph); //initializes all the vertices to cost infinity

    start->cost = 0; //set the cost of start to 0
    start->visited = true;
    start->setColor(GREEN);
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> queue;
    queue.enqueue(start , start->cost + heuristicFunction(start , end)); //enqueues start with its cost + the heuristic function
    path.add(start);

    //while there are still vertices in the queue
    while (!queue.isEmpty()){
        Vertex* v = queue.dequeue(); //dequeues the vertex with loset priority
        v->visited = true; //sets it to visited
        v->setColor(GREEN);
        if (v == end){ //if we've found the end
            traceBack(path , v); //do the traceback
            break;
        }
        //if we haven't found the end
        for (Edge* edge : v->edges){
            if (!edge->finish->visited){
                double cost = edge->cost + v->cost; //gets a variable cost equals to the previous vertex's + the edge's cost
                if (cost < edge->finish->cost){ //if the new cost is inferior to the vertex's cost
                    edge->finish->cost = cost; //set the vertex's cost to be the new cost
                    edge->finish->previous = edge->start; //set the previous pointer
                    if (edge->finish->getColor() == YELLOW){ //if it's yellow = if it's been enqueued already
                        queue.changePriority(edge->finish , cost + heuristicFunction(edge->finish , end)); //change its priority and add the heuristic
                    } else{ //if it hasn't been enqueues
                        queue.enqueue(edge->finish , cost + heuristicFunction(edge->finish , end)); //enqueue with the heuristic
                    }
                    edge->finish->setColor(YELLOW); //set it to yellow
                }
            }
        }
    }
    return path;
}

//kruskal's algorithm
//basically creates a map with keys: the name of all the nodes, and values: a set of all the nodes that it's conntected to
//the large mazes run somewhat slowly though (around 1 minute for large, way too long for huge)
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData(); //resets the graph

    HashMap< string , Set<Vertex*> > vertexClusters; //initializes the cluster map
    PriorityQueue <Edge*> edgeSet; //pqueue of edge pointers
    Set<Edge*> mst; //minimum spanning tree

    //iterates over all the vertices in the graph
    for (Vertex* v : graph.getVertexSet()){
        vertexClusters[v->name].add(v); //creates a key-value paiting with the name of the node connteced to the node
    }

    //puts all the edged from the graph into the priority queue
    for (Edge* e : graph.getEdgeSet()){
        edgeSet.enqueue(e , e->cost);
    }

    //while the edge set isn't empty
    while (!edgeSet.isEmpty()){
        Edge* e = edgeSet.dequeue(); //dequeue an edge

        //if they aren't in the same cluster
        //i.e. here if the value set of the first node doesn't contain the second
        if (!vertexClusters[e->start->name].contains(e->finish)){
            Set<Vertex*> newSet = vertexClusters[e->start->name] + vertexClusters[e->finish->name]; //creates a new set of vertices as the sum of both previous ones (that the edge conntects to)
            Set<Vertex*>* setPointer = &newSet; //pointer to it (for efficiency)
            for (Vertex* v : *setPointer){ //iterates over all the vertices in that set
                vertexClusters[v->name] = *setPointer; //sets that vertex's value set to be the new combined set
            }
            mst.add(e); //adds the edge to the MST
        }
    }

    return mst;
}
