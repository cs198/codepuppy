// This is the CS106B Trailblazer assignment.
// I received help from the LAIR, and discussed the assignment
// with Monica Yupa and Reynis Vasquez Guzman.

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "stack.h"
using namespace std;

bool depthFirstSearchHelp(BasicGraph& graph, Vertex *start, Vertex *end, Vector<Vertex*>& path);
void reconstructPath(Vertex* v, Vector<Vertex*>& path);
Set<Vertex*> getVertices(BasicGraph& graph);
PriorityQueue<Vertex*> createPQ(Set<Vertex*>& vertices);

/* The depthFirstSearch function explores a single function all the way. It resets the
 * graph and creates a Vector of Vertices for the path. It then calls its recursvie helper.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelp(graph, start, end, path);
    return path;
}

/* The depthFirstSearchHelp function adds the current start to the path and sets it to visited. If the
 * current start is the end, the function returns. Otherwise, it obtains all the neighbors of the current
 * start, and visits all of the unvisited neighbors amongst this set. If it finds the end, it returns.
 * If the end is not found, the squares color is turned to gray and removed from the path.
 */
bool depthFirstSearchHelp(BasicGraph& graph, Vertex *start, Vertex *end, Vector<Vertex*>& path){
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);

    if(start==end){
        return true;
    }

    Set<Vertex*> neighbors = graph.getNeighbors(start);
    for(Vertex* n : neighbors){
        if(!n->visited){
            bool found = depthFirstSearchHelp(graph, n, end, path);
            if(found){
                return true;
            }
        }
    }

    start->setColor(GRAY);
    path.remove(path.size()-1);
    return false;
}

/* The breadthFirstSearch function resets the graph and creates a path vector and queue of the visited
 * vertices. The starting vertex is equeued in the queue and is marked as visited. While the visited
 * vertex queue is not empty, a vertex is dequeued, and if it is not the end, its neighbors are put
 * in a set. If a neighbor is unvisited, it is marked as visited and reenqueued. If the current vertex
 * is the end, the loop breaks, and the path is reconstructed and returned.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Vector<Vertex*> path;
    Queue<Vertex*> visitedvertex;

    visitedvertex.enqueue(start);
    start->visited = true;
    start->setColor(YELLOW);

    Vertex* v;

    while(!visitedvertex.isEmpty()){
        v = visitedvertex.dequeue();
        v->setColor(GREEN);
        if(v==end){
            break;
        }

        Set<Vertex*> neighbors = graph.getNeighbors(v);

        for(Vertex* n : neighbors){
            if(!n->visited){
                n->visited = true;
                n->setColor(YELLOW);
                n->previous = v;
                visitedvertex.enqueue(n);
            }
        }
    }

    reconstructPath(v, path);
    return path;
}

/* The dijkstrasAlgorithm function resets the graph and creates a vector to store the path. It gets all
 * the vertices and creates a priority queue of vertices and their costs, initially set to zero. While
 * this priority queue is not empty, it dequeues a vertex, marks it as visited, and if this is not the
 * end vertex, it checks the neighbors. When it finds the end, it reconstructs the path and returns it.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Vector<Vertex*> path;

    Set<Vertex*> vertices = getVertices(graph);

    start->cost = 0;

    PriorityQueue<Vertex*> pq = createPQ(vertices);

    Vertex* v;

    while(!pq.isEmpty()){
        v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);

        if(v==end){
            break;
        }

        Set<Vertex*> neighbors = graph.getNeighbors(v);

        for(Vertex* n : neighbors){
            if(!n->visited){
                double cost = v->cost + Edge(v,n).cost;
                if(cost < n->cost){
                    n->cost = cost;
                    n->previous = v;
                }
                if(!n->getColor()==YELLOW){
                    pq.changePriority(n, cost);  //change priority in queue
                }else{
                    n->setColor(YELLOW);
                    pq.enqueue(n, cost);
                }
            }
        }
    }

    reconstructPath(v, path);
    return path;
}

/* The aStar function has the same functionality as dijkstrasAlgorithm; however, it
 * implements the heuristicFunction to make the function faster.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Vector<Vertex*> path;

    Set<Vertex*> vertices = getVertices(graph);

    start->cost = heuristicFunction(start, end);

    PriorityQueue<Vertex*> pq = createPQ(vertices);

    Vertex* v;

    while(!pq.isEmpty()){
        v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);

        if(v==end){
            break;
        }

        Set<Vertex*> neighbors = graph.getNeighbors(v);

        for(Vertex* n : neighbors){
            if(!n->visited){
                double cost = v->cost + Edge(v,n).cost + heuristicFunction(n, end);
                if(cost < n->cost){
                    n->cost = cost;
                    n->previous = v;

                    pq.changePriority(n, cost);  //change priority in queue
                    n->setColor(YELLOW);
                }
            }
        }
    }

    reconstructPath(v, path);
    return path;
}

/* The kruskal function creates a minimum spanning tree for the graph by iteratively choosing
 * edges in the graph and merging clusters if the start and ends of edge vertices are held in
 * different clusters.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    Set<Vertex*> vertices = graph.getVertexSet();

    Vector<Set<Vertex*> > clusters;

    //put each vertex into its own cluster (group of reachable vertices)
    for(Vertex* v : vertices){
        Set<Vertex*> cluster;
        cluster.add(v);
        clusters.add(cluster);
    }

    Set<Edge*> edges = graph.getEdgeSet();
    PriorityQueue<Edge*> edgepriority;

    //iterate over edges to create priority queue of edges
    for(Edge* e : edges){
        edgepriority.enqueue(e, e->cost);
    }

    while(clusters.size() > 1){
        Edge* e = edgepriority.dequeue();

        //if start & finsih vertices of e are not in same cluster
        //merge the clusters containing the start and finsih vertices of e
        //add e to spanning tree mst

        int startindex;
        int finishindex;

        for(int i=0; i<clusters.size(); i++){
            Set<Vertex*> cluster = clusters[i];

            if(cluster.contains(e->start)) startindex = i;
            if(cluster.contains(e->finish)) finishindex=i;
        }

        if(startindex != finishindex){
            clusters[startindex] += clusters[finishindex];
            clusters.remove(finishindex);
            mst.add(e);
        }
    }

    return mst;
}

/* The reconstructPath function follows the pointers of the end vertex back
 * to recreate the path between two squares.
 */
void reconstructPath(Vertex* v, Vector<Vertex*>& path){
    Stack<Vertex*> storevertex;
    storevertex.push(v);
    Vertex* current = v;

    while(current->previous!=NULL){
        storevertex.push(current->previous);
        current = current->previous;
    }

    while(!storevertex.isEmpty()){
        path.add(storevertex.pop());
    }
}

/* The getVertices function returns a set of all the vertices in the graph,
 * setting all their costs to positive infinity and their previous pointers to
 * null.
 */
Set<Vertex*> getVertices(BasicGraph& graph){
    Set<Vertex*> vertices = graph.getVertexSet();
    for(Vertex* v : vertices){
        v->cost = POSITIVE_INFINITY;
        v->previous = NULL;
    }

    return vertices;
}

/* The createPQ function creates the priority queue of vertices and costs.
 */
PriorityQueue<Vertex*> createPQ(Set<Vertex*>& vertices){
    PriorityQueue<Vertex*> pq;
    for(Vertex* v : vertices){
        pq.enqueue(v, v->cost); //add to priority queue
    }
    return pq;
}
