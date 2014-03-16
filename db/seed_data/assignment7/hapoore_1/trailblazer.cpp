// This file comprises the bodies of the various search algorithms, including DFS, BFS, Dijkstra,
// A*, and Kruskal's. The functions in this file get called by the client main program to
// execute the searches and generate the mazes.

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "hashset.h"

using namespace std;

bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);
void searchGraph(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Queue<Vertex*>& vertices);
void initializeVertices(BasicGraph& graph);
void reconstructPath(Vertex* end, Vector<Vertex*>& path);
void checkNeighbors(Vertex* current, Vertex* end, Set<Edge*>& edgeSet, PriorityQueue<Vertex*>& vertices, Set<Vertex*>& queueContents, bool dijkstra = false);
void aStarDijkstraHelper(BasicGraph& graph, Vector<Vertex*>& path, Vertex* start, Vertex* end, bool dijkstra = false);
void initializeClusters(Map<int, Set<Vertex*>>& clusters, Set<Vertex*>& vertices);
void enqueueEdges(BasicGraph& graph, PriorityQueue<Edge*>& edges);
void mergeClusters(int& cluster1, int& cluster2, Map<int, Set<Vertex*>>& clusters);
void locateClusters(int& cluster1, int& cluster2, Edge* edge, Map<int, Set<Vertex*>>& clusters);

/* This method executes the depth first search algorithm by calling a recursive helper function.
 * This function takes a graph by reference and start and endpoints by copy.
 */

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(graph, start, end, path);
    return path;
}

/* This is the recursive helper for the DFS. It takes a graph and a vector representing the
 * current path by reference, and then also a starting point and an endpoint. It marks the
 * vertices it visits, and then uses recursive backtracking to explore the path.
 */

bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    start->setColor(GREEN);
    path.add(start);
    start->visited = true;
    if(start == end) {
        return true;
    }
    Set<Edge*> edgeSet = start->edges;
    for(Edge* nextEdge: edgeSet) {
        if(nextEdge->finish->visited == false) {
            if(depthFirstSearchHelper(graph, nextEdge->finish, end, path)) {
                return true;
            }
        }
    }
    path[path.size() - 1]->setColor(GRAY);
    path.remove(path.size() - 1);
    return false;
}

/* This function executes the breadth first search by making use of a helper function, called
 * searchGraph. This function takes a graph, a starting point, and an endpoint, and returns the
 * shortest path between the two.
 */

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> vertices;
    vertices.enqueue(start);
    start->setColor(YELLOW);
    start->visited = true;
    searchGraph(graph, start, end, path, vertices);
    return path;
}

/* This function searches the graph for the breadth first search. It uses the queue of vertices
 * passed in from the BFS method, and explores step by step. It is guaranteed to find the shortest
 * path between the two vertices.
 */

void searchGraph(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, Queue<Vertex*>& vertices) {
    while(!vertices.isEmpty()) {
        Vertex* currVertex = vertices.dequeue();
        currVertex->setColor(GREEN);
        if(currVertex == end) {
            reconstructPath(end, path); // Helper function
        } else {
            Set<Edge*> edgeSet = currVertex->edges;
            for(Edge* nextEdge: edgeSet) {
                Vertex* neighbor = nextEdge->finish;
                if(!neighbor->visited) {
                    neighbor->visited = true;
                    neighbor->previous = currVertex;
                    vertices.enqueue(neighbor);
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
}

/* This function uses Dijkstras algorithm to find the least cost path between two vertices
 * which get passed in. It uses a helper function, and returns a vector containing the path.
 */

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    aStarDijkstraHelper(graph, path, start, end, true);
    return path;
}

/* This function gets called from the aStarDijkstraHelper function, and sets the cost of
 * all vertices to a constant called POSITIVE_INFINITY.
 */

void initializeVertices(BasicGraph& graph) {
    Set<Vertex*> vertices = graph.getVertexSet();
    for(Vertex* nextVertex : vertices) {
        nextVertex->cost = POSITIVE_INFINITY;
    }
}

/* This is a helper function which uses the previous pointers stored within a vertex to reconstruct
 * the path between vertices and store it in a vector.
 */

void reconstructPath(Vertex* end, Vector<Vertex*>& path) {
    while(end->previous != NULL) {
        path.insert(0, end);
        end = end->previous;
    }
    path.insert(0, end);
}

/* This function is a helper for Dijkstra and A* that examines the neighbors of a given vertex.
 * If they have not been visited, it checks the edge cost. If the path is better, it gets
 * added to the queue again with the proper priority, depending on which algorithm it was
 * called from.
 */

void checkNeighbors(Vertex* current, Vertex* end, Set<Edge*>& edgeSet, PriorityQueue<Vertex*>& vertices, Set<Vertex*>& queueContents, bool dijkstra) {
    for(Edge* nextEdge: edgeSet) {
        Vertex* neighbor = nextEdge->finish;
        if(!neighbor->visited) {
            double newCost = current->cost + nextEdge->cost;
            if(newCost < neighbor->cost) {
                neighbor->cost = newCost;
                neighbor->previous = current;
                if(queueContents.contains(neighbor)) { // Already in the queue
                    if(dijkstra) {
                        vertices.changePriority(neighbor, newCost);
                    } else {
                        vertices.changePriority(neighbor, newCost + heuristicFunction(neighbor, end));
                    }
                } else {
                    if(dijkstra) {
                        vertices.enqueue(neighbor, newCost);
                    } else {
                        vertices.enqueue(neighbor, newCost + heuristicFunction(neighbor, end));
                    }
                    neighbor->setColor(YELLOW);
                    queueContents.add(neighbor);
                }
            }
        }
    }
}

/* This function executes the A* algorithm. It takes a graph, a starting point, and an endpoint,
 * and returns the shortest path between the two. It uses heuristics to direct its search, and it
 * is guaranteed to find the shortest path between two vertices.
 */

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    aStarDijkstraHelper(graph, path, start, end);
    return path;
}

/* This function is a helper method for both Dijkstra and A*. The algorithms are essentially
 * identical, besides the use of the heuristic function, so this function takes a boolean
 * to determine which algorithm it is currently executing so that it can apply the heuristic
 * properly. It sets up a PQ of vertices and then dequeues them one at a time to examine
 * them.
 */

void aStarDijkstraHelper(BasicGraph& graph, Vector<Vertex*>& path, Vertex* start, Vertex* end, bool dijkstra) {
    PriorityQueue<Vertex*> vertices;
    Set<Vertex*> queueContents;
    initializeVertices(graph);
    start->cost = 0;
    if(dijkstra) {
        vertices.enqueue(start, start->cost);
    } else {
        vertices.enqueue(start, heuristicFunction(start, end));
    }
    start->setColor(YELLOW);
    queueContents.add(start);
    while(!vertices.isEmpty()) {
        Vertex* current = vertices.dequeue();
        current->setColor(GREEN);
        queueContents.remove(current);
        current->visited = true;
        if(current == end) {
            reconstructPath(end, path);
            break;
        }
        Set<Edge*> edgeSet = current->edges;
        checkNeighbors(current, end, edgeSet, vertices, queueContents);
    }
}

/* This function executes Kruskal's algorithm to find a minimum spanning tree from the graph
 * it gets passed. It creates clusters of vertices, stored in sets, and merges sets together
 * whenever two vertices from different sets are connected until only one cluster remains.
 * The edges remaining constitute the minimum spanning tree, and are returned in a set.
 */

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> tree;
    Map<int, Set<Vertex*>> clusters;
    Set<Vertex*> vertices = graph.getVertexSet();
    PriorityQueue<Edge*> edges;
    initializeClusters(clusters, vertices);
    enqueueEdges(graph, edges);
    while(!edges.isEmpty()) {
        Edge* nextEdge = edges.dequeue();
        int cluster1;
        int cluster2;
        locateClusters(cluster1, cluster2, nextEdge, clusters);
        if(cluster1 != cluster2) {
            mergeClusters(cluster1, cluster2, clusters);
            tree.add(nextEdge);
        }

    }
    return tree;
}

/* This function sets up the initial cluster. It puts each vertex in its own cluster.
 */

void initializeClusters(Map<int, Set<Vertex*>>& clusters, Set<Vertex*>& vertices) {
    int counter = 1;
    for(Vertex* currVertex : vertices) {
        Set<Vertex*> cluster;
        cluster.add(currVertex);
        clusters.put(counter, cluster);
        counter++;
    }
}

/* This function takes all of the edges from a graph and enqueues them into a PQ.
 */

void enqueueEdges(BasicGraph& graph, PriorityQueue<Edge*>& edges) {
    Set<Edge*> allEdges = graph.getEdgeSet();
    for(Edge* currEdge : allEdges) {
        edges.enqueue(currEdge, currEdge->cost);
    }
}

/* This function gets an integer reference to a pair of clusters and combines
 * them into a single cluster in the map. The new cluster retains the number of the
 * first cluster that gets passed in.
 */

void mergeClusters(int& cluster1, int& cluster2, Map<int, Set<Vertex*>>& clusters) {
    clusters.put(cluster1, clusters[cluster1] + clusters[cluster2]);
    clusters.remove(cluster2);
}

/* This function takes an edge and figures out which clusters the vertices at its endpoints belong to. It
 * stores the cluster numbers in a pair of reference parameters.
 */

void locateClusters(int& cluster1, int& cluster2, Edge* edge, Map<int, Set<Vertex*>>& clusters) {
    for(int numCluster : clusters) {
        if(clusters[numCluster].contains(edge->start)) {
            cluster1 = numCluster;
        }
        if(clusters[numCluster].contains(edge->finish)) {
            cluster2 = numCluster;
        }
    }
}
