/* Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * File: trailblazer.cpp
 * -----------------------------
 * Implements functions necessary for trailblazer to run. This includes implementations
 * for a depth-first search through a graph, a breadth-first search through a graph,
 * Dijkstra's Algorithm, and A*. In addition, there is an implementation for Kruskal's
 * Algorithm that is used by the program to create a randomly generated maze.
 */

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;

//Function prototypes
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Vertex* vertex);
bool breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end, Vertex* vertex, Vector<Vertex*>& path, Queue<Vertex*>& queue);
void determinePath(Vector<Vertex*>& path, Vertex* end, Vertex* start);
bool determineIfQueueContainsVertex(Vertex *neighbor, PriorityQueue<Vertex*> &pq, Vertex* end, bool heuristicNecessary);

/* Searches a graph using a depth-first search to find a path form the start vertex
 * to the end vertex. It returns the path at the end. The function uses the recursive
 * depthFirstSearchHelper function to perform its task.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    path.add(start);
    depthFirstSearchHelper(graph,start,end,path,start);
    return path;
}

/* The helper function to the depth-first search function. Traverses the graph
 * trying to find a path from the start to the end, exploring each path that it chooses
 * until it has determined that the choice is impossible and then moving to the next choice.
 * The function colors vertices that it has visited green and vertices that it knows are
 * impossible gray.
 */
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Vertex* vertex) {
    vertex->setColor(GREEN);
    vertex->visited = true;
    if (vertex == end) {
        return true;
    }
    for (Vertex* neighbor : graph.getNeighbors(vertex)) {
        if (!neighbor->visited) {
            path.add(neighbor);
            if (depthFirstSearchHelper(graph,start,end,path,neighbor)) {
                return true;
            }
        }
    }
    vertex->setColor(GRAY);
    path.remove(path.size()-1);
    return false;
}

/* Searches for the shortest path between two vertices of a graph using a
 * breadth-first search. The function uses a queue to implement the breadth-first search.
 * It colors vertices that it has visited green and vertices it has enqueued but has yet to visit
 * yellow. The function returns the shortest path in the graph from the start vertex
 * to the end vertex.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    queue.enqueue(start);
    start->previous = NULL;
    start->visited = true;
    start->setColor(YELLOW);
    Vertex* v;
    while (!queue.isEmpty()) {
        v = queue.dequeue();
        v->setColor(GREEN);
        if (v == end) {
            break;
        }
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (!neighbor->visited) {
                neighbor->previous = v;
                neighbor->visited = true;
                neighbor->setColor(YELLOW);
                queue.enqueue(neighbor);
            }
        }
    }
    determinePath(path,end,start);
    return path;
}

/*Used by Dijkstra's, Breadth-First Search, and A* to reconstruct the shortest path
 * at the end of the algorithm. It uses the previous pointers of each vertex to track back
 * and add each vertex to the path, in the correct order.
 */
void determinePath(Vector<Vertex*>& path, Vertex* end, Vertex* start) {
    Vector<Vertex*> reversePath;
    Vertex* v = end;
    cout << "determining path" << endl;
    while(true) {
        reversePath.add(v);
        if (v == start) break;
        v = v->previous;
    }
    for (int i = reversePath.size() - 1 ; i >= 0 ; i--) {
        path.add(reversePath[i]);
    }
}

/* Implements Dijkstra's Algorithm in order to determine the path with the least cost
 * between two vertices in a weighted graph. It uses the determinePath function to
 * reconstruct the path of vertices at the end and returns that path.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    for (Vertex* vertex : graph.getVertexSet()) {
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start,start->cost);
    start->setColor(YELLOW);
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);
        if (v == end) break;
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (!neighbor->visited) {
                double cost = v->cost + graph.getEdge(v,neighbor)->cost;
                if (cost < neighbor->cost) {
                    neighbor->cost = cost;
                    neighbor->previous = v;
                    neighbor->setColor(YELLOW);
                    bool alreadyExists = determineIfQueueContainsVertex(neighbor,pq,end,false);
                    if (alreadyExists) {
                        pq.changePriority(neighbor,cost);
                    } else {
                        pq.enqueue(neighbor,cost);
                    }
                }
            }
        }
    }
    Vector<Vertex*> path;
    determinePath(path,end,start);
    return path;
}

/* Returns true if the vertex passed in as an argument already exists in the Priority Queue
 * and false otherwise. This function is used by the Dijkstra's and A* functions. The boolean
 * parameter is used by the function to determine whether or not it needs to factor
 * in the heuristic when readding the vertices to the priority queue.
 */
bool determineIfQueueContainsVertex(Vertex* neighbor,PriorityQueue<Vertex*>& pq, Vertex* end, bool heuristicNecessary) {
    Queue<Vertex*> temp;
    bool result = false;
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        if (v==neighbor) {
            result = true;
        }
        temp.enqueue(v);
    }
    while (!temp.isEmpty()) {
        Vertex* v = temp.dequeue();
        if (heuristicNecessary) {
            pq.enqueue(v,v->cost + heuristicFunction(v,end));
        } else {
            pq.enqueue(v,v->cost);
        }
    }
    return result;
}

/* Implements the A* algorithm in order to determine the path with the least cost
 * between two vertices in a weighted graph. This algorithm is implemented
 * exactly the same as Dijkstra, except when enqueueing a vertex it adds
 * the value of a heuristic function to the priority in order to get the
 * priority queue to prioritize vertices closer to the end vertex. It uses the
 * determinePath function to reconstruct the path of vertices at the end and returns
 * that path.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    for (Vertex* vertex : graph.getVertexSet()) {
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start,start->cost + heuristicFunction(start,end));
    start->setColor(YELLOW);
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);
        if (v == end) break;
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (!neighbor->visited) {
                double cost = v->cost + graph.getEdge(v,neighbor)->cost;
                if (cost < neighbor->cost) {
                    neighbor->cost = cost;
                    neighbor->previous = v;
                    neighbor->setColor(YELLOW);
                    bool alreadyExists = determineIfQueueContainsVertex(neighbor,pq,end,true);
                    cost += heuristicFunction(neighbor,end);
                    if (alreadyExists) {
                        pq.changePriority(neighbor,cost);
                    } else {
                        pq.enqueue(neighbor,cost);
                    }
                }
            }
        }
    }
    Vector<Vertex*> path;
    determinePath(path,end,start);
    return path;
}

/* Implements Kruskal's Algorithm to find a minimum spanning tree for a graph,
 * which it returns at the end of the function.The function creates the
 * minimum spanning tree by putting each vertex of the graph into its
 * own cluster and putting each edge of the graph in a priority queue, where
 * the priority of each edge is its cost. Then, while the number of clusters is
 * greater than one, the function dequeues an edge from the priority queue and,
 * if the two vertices of that edge are not in the same cluster, it combines
 * the clusters and adds the edge to the minimum spanning tree.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData();
    Vector< Set <Vertex*> > clusters;
    int count = 0;
    for (Vertex* v : graph.getVertexSet()) {
        Set<Vertex*> cluster;
        cluster.add(v);
        clusters.add(cluster);
        v->cost = count;
        count++;
    }
    PriorityQueue<Edge*> pq;
    for (Edge* edge : graph.getEdgeSet()) {
        pq.enqueue(edge,edge->cost);
    }
    Set<Edge*> mst;
    int nClusters = count;
    while (nClusters > 1) {
        Edge* e = pq.dequeue();
        if (e->start->cost != e->finish->cost) {
            for (Vertex* v : clusters[e->finish->cost]) {
                v->cost = e->start->cost;
                clusters[e->start->cost].add(v);
            }
            nClusters -= 1;
            mst.add(e);
        }
    }
    return mst;
}
