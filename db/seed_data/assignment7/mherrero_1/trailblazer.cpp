/*
 * Filename - Trailblazer.cpp - Matt Herrero
 * ----------------------------------------------------------------------------------------------------------------
 * This is the implementation for the 5 algorithms assigned in Assignment 7. The search algorithms are Depth First
 * Search, Breadth First Search, Dijkstras Search, and A* Search. These search algorithms can, upon recieving a
 * starting vertex, an ending vertex, and a graph, find the path that algorithm is optimized to find (any path for
 * DFS, the shortest for BFS, the cheapest for Dijkstras and A*) Also, an method tha uses the kruskal's algorithm
 * to randomly generate a minimum spanning tree that can be utilized as a maze environment for the program is also
 * included. There are other methods that aid in the implementation of the search algorithms and kruskal's algorithm.
 * The functionality of each method is described in detail before each method.
 *
 */

#include "costs.h"
#include "queue.h"
#include "trailblazer.h"
#include "pqueue.h"
using namespace std;

/*
 * This method recieves a connected BasicGraph, a starting vertex in the graph, and an ending vertex in the graph,
 * and uses them to implement a Depth First Search algorithm on the grid to find a path from the starting vertex
 * to the ending vertex. This method first resets the data in the graph, and then makes a Vector of vertex
 * pointers to store the final path. Then, it recursively searches the graph for a path using the depthFirstSearchHelper
 * method. It then returns this path.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(path, graph, start, end);
    return path;
}

/*
 * This is the method that recursively searches a graph using a depth first search algorithm to find a path from
 * the starting vertex to the ending vertex, returning true once it has found the path. It first checks to see if
 * the starting algorimth is one that has already been visited in a previous recursive call and returns false if
 * it has been. Then, it marks the vertex as visited and adds it to the path. If this vertex is the end vertex, the
 * search ends and the method returns true. If neither of these are true, then the method recursively searches
 * the rest of the graph to see if there is a valid path starting at each of the vertex's neighbors. As soon
 * as it finds one (the recursive call returns true) the method returns true, and the search ends. If it fully
 * executes this neighbor loop without finding a path, the method returns false, and no path was found.
 */
bool depthFirstSearchHelper(Vector<Vertex*>& path, BasicGraph& graph, Vertex* start, Vertex* end) {
    if(start->visited) return false;
    start->visited=true;
    path.add(start);
    start->setColor(GREEN);
    if(end == start) return true;
    for(Vertex* neighborEdge: graph.getNeighbors(start)){
        if(depthFirstSearchHelper(path, graph, neighborEdge, end)) return true;
    }
    path.remove(path.size()-1);
    start->setColor(GRAY);
    return false;
}

/*
 * This is the method that searches a graph using a breadth first search algorithm to find a path from
 * the starting vertex to the ending vertex. The method first resets the data of the graph. It then creates
 * a Vector of vertex pointers that will contain the final path as well as a queue of Vertex pointers containing
 * the next vertices that must be examined by the search algorithm. It begins by enqueuing the start vertex
 * and setting it as visited. Then, while the possibleVertices queue is not empty (if this condition is false,
 * there is not possible path between the vertices) the method dequeues a vertex from the queue. If this vertex
 * is the end, the path is reconstructed using the reconstructPath method and the search ends. If this is not
 * true, then each neighbor that has not been previously visited by the search is enqueued into the possibleVertices
 * queue and marked as visited, as well as has the previous pointer set to the original pointer. Then, the loop
 * repeats until the vertex that is dequeued is the end vertex.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> possibleVertices;
    possibleVertices.enqueue(start);
    start->visited = true;
    start->setColor(GREEN);
    while(!possibleVertices.isEmpty()) {
        Vertex* nextVertex = possibleVertices.dequeue();
        nextVertex->setColor(GREEN);
        if(nextVertex == end) {
            return reconstructPath(start, nextVertex, path);
        }
        for(Vertex* neighbor : graph.getNeighbors(nextVertex)) {
            if(!neighbor->visited) {
                neighbor->setColor(YELLOW);
                neighbor->visited = true;
                neighbor->previous = nextVertex;
                possibleVertices.enqueue(neighbor);
            }
        }
    }
    return path;
}

/*
 * This method recieves a starting vertex, and ending vertex, and a Vector of vertex pointers. This method will
 * trace back through the previous pointers in each vector to reconstruct the found path between the two vertices.
 * Once it has done this, the method returns the path.
 */
Vector<Vertex*> reconstructPath(Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    Vertex* vertexOnPath = end;
    while(vertexOnPath->previous!=NULL) {
        path.insert(0, vertexOnPath);
        vertexOnPath = vertexOnPath->previous;
    }
    path.insert(0, start);
    return path;
}

/*
 * This is the method that searches a graph using dijkstras search algorithm to find the cheapest path from
 * the starting vertex to the ending vertex. It begins by resetting the graph data, then making a Vector of vertex
 * pointers that will contain the final path as well as a priority queue of vertex pointers that will return the
 * cheapest vertex that must be explored next. Then, while the priority queue is not empty, the next vertex pointer
 * is dequeued and marked as visited. If this vertex is the end vertex, then the path is reconstructed and returned.
 * If it is not, then the method iterates through each neighbor of the vertex. If it has not yet been visited, it
 * adds it to the priority queue with a priority that is equal to the cost of the previous vertex plus the cost
 * of the edge that was just traveled. If the vertex has been previously visited, but the new cost is cheaper than
 * the cost currently stored in the priority queue, then its priority is changed in the queue and the previous
 * pointer of the vertex is changed so that it points to the recently-found cheaper path, rather than the old
 * path. If the priority queue is completely dequeued without finding a path, then there is no path and an empty
 * path vector is returned.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> vertexQueue;
    vertexQueue.enqueue(start, 0);
    while(!vertexQueue.isEmpty()) {
        Vertex* nextVertex = vertexQueue.dequeue();
        nextVertex->visited = true;
        nextVertex->setColor(GREEN);
        if(nextVertex == end) {
            return reconstructPath(start, end, path);
        }
        for(Edge* neighborEdge : nextVertex->edges) {
            Vertex* neighbor = neighborEdge->finish;
            if(!neighbor->visited) {
                vertexQueue.enqueue(neighbor, nextVertex->cost + neighborEdge->cost);
                neighbor->setColor(YELLOW);
                neighbor->previous = nextVertex;
            } else if(neighbor->cost > neighborEdge->start->cost + neighborEdge->cost){
                neighbor->setColor(YELLOW);
                neighbor->cost = neighbor->previous->cost + neighborEdge->cost;
                neighbor->previous = nextVertex;
                vertexQueue.changePriority(neighbor, neighbor->cost);
            }
        }
    }
    return path;
}

/*
 * This method is, in almost all respects, identical to the dijkstras algorithm. See the dijkstrasAlgorithm
 * method's header function to find a full comment description.
 *
 * The only difference between the aStar method's funcionality and the dijkstras method's functionality is the
 * introduction of a heuristic function to enhance the probability of finding the cheapest path sooner. This
 * means that, when calculating a vector's weight to be put into the queue, we use the weight of the previous
 * vector plus the weight of the edge that was just taken plus a certain heuristicFunction value.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> vertexQueue;
    vertexQueue.enqueue(start, heuristicFunction(start, end));
    while(!vertexQueue.isEmpty()) {
        Vertex* nextVertex = vertexQueue.dequeue();
        nextVertex->visited = true;
        nextVertex->setColor(GREEN);
        if(nextVertex == end) {
            return reconstructPath(start, end, path);
        }
        for(Edge* neighborEdge : nextVertex->edges) {
            Vertex* neighbor = neighborEdge->finish;
            if(!neighbor->visited) {
                vertexQueue.enqueue(neighbor, nextVertex->cost + neighborEdge->cost + heuristicFunction(neighbor, end));
                neighbor->setColor(YELLOW);
                neighbor->previous = nextVertex;
            } else if(neighbor->cost > neighborEdge->start->cost + neighborEdge->cost){
                neighbor->setColor(YELLOW);
                neighbor->cost = neighbor->previous->cost + neighborEdge->cost;
                neighbor->previous = nextVertex;
                vertexQueue.changePriority(neighbor, neighbor->cost + heuristicFunction(neighbor, end));
            }
        }
    }
    return path;
}

/*
 * This method uses the kruskal's algorithm to randomly generate a minimum spanning tree of the graph passed to it
 * that can be made into a map for the search algorithms to use. It first creates a set of edge pointers that will
 * be returned with the minimum spanning tree, a priority queue of edge pointers that will be used to order the
 * edges in order of lowest cost first, and a map that maps each cluster (clusters determined by the int key value)
 * that maps ints to sets of vertex pointers, which contain all the vertices in each cluster. Then, the clusterMap
 * is filled such that each vertex is given its own cluster. In this same method, the costs of each vertex are altered
 * to match the cluster number, which will be useful in determining if two vertices are in the same or different
 * clusters. Next, all the edges are fed into the priority queue.Then, while the cluster map has a size greater
 * than 1 (more than one cluster left), the algorithm removes the cheapest edge from the priority queue. If the
 * vertex that is at the start of this edge is of a different cluster, the edge is added to the spanning tree. Then,
 * all the vertices of the finish vertex are converted to the new cluster, which involves changing their individual
 * cost values as well as merging the sets in the map and deleting the finish cluster's map entry. Once this while
 * loop is finish and there is one cluster entry inside the map, the minimum spanning tree is returned.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData();
    Set<Edge*> mst;
    PriorityQueue<Edge*> edgeQueue;
    Map< int, Set<Vertex*> > clusterMap;
    fillClusterMap(graph, clusterMap);
    fillEdgeQueue(edgeQueue, graph);
    while(clusterMap.size() > 1) {
        Edge* edge = edgeQueue.dequeue();
        if(edge->start->cost!=edge->finish->cost) {
            mst.add(edge);
            int trashCluster = edge->finish->cost;
            for(Vertex* vertex : clusterMap[trashCluster]) {
                vertex->cost = edge->start->cost;
            }
            clusterMap[edge->start->cost] = clusterMap[edge->start->cost] + clusterMap[trashCluster];
            clusterMap.remove(trashCluster);
        }
    }
    return mst;
}

/*
 * This method recieves a graph and a map of ints to sets of vertex pointers. This method adds puts a new map entry
 * with a int value denoting the cluster number and a set representing the vertices in that cluster into the map for
 * each vertex. The map will begin with as many entry as vertices in the graph, and each set will only initially
 * contain one vertex. Also, the cost of each vertex is changed to its specific cluster number.
 */
void fillClusterMap(BasicGraph& graph, Map<int, Set<Vertex*> >& clusterMap) {
    int groupNumber = 0;
    for(Vertex* vertex : graph.getNodeSet()) {
        Set<Vertex*> nextSet;
        nextSet.add(vertex);
        clusterMap.put(groupNumber, nextSet);
        vertex->cost = groupNumber;
        groupNumber++;
    }
}

/*
 * This method recieves a priority queue of edge pointers and a graph and fills the priority queue with the edge
 * pointers along with their respective costs.
 */
void fillEdgeQueue(PriorityQueue<Edge*>& edgeQueue, BasicGraph& graph) {
    for(Edge* edge : graph.getEdgeSet()) {
        edgeQueue.enqueue(edge, edge->cost);
    }
}

