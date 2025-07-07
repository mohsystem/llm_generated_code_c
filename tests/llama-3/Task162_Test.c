#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

// Structure to represent a node in the graph
struct Node {
    char name;
    int distance;
};

// Structure to represent an edge in the graph
struct Edge {
    char from;
    char to;
    int weight;
};

// Global arrays to store nodes and edges
struct Edge edges[MAX_EDGES];
int edgeCount = 0;

struct Node nodes[MAX_VERTICES];
int nodeCount = 0;

// Function to add a node to the graph
void addNode(char name) {
    nodes[nodeCount].name = name;
    nodes[nodeCount].distance = INT_MAX;
    nodeCount++;
}

// Function to add an edge to the graph
void addEdge(char from, char to, int weight) {
    edges[edgeCount].from = from;
    edges[edgeCount].to = to;
    edges[edgeCount].weight = weight;
    edgeCount++;
}

// Function to find the node index by name
int findNodeIndex(char name) {
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i].name == name) {
            return i;
        }
    }
    return -1;
}

// Function to find the vertex with minimum distance
int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < nodeCount; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Function to implement Dijkstra's algorithm
void dijkstra(char start) {
    int dist[MAX_VERTICES];
    bool sptSet[MAX_VERTICES];

    for (int i = 0; i < nodeCount; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    int startIndex = findNodeIndex(start);
    dist[startIndex] = 0;

    for (int count = 0; count < nodeCount - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int i = 0; i < edgeCount; i++) {
            if (edges[i].from == nodes[u].name) {
                int v = findNodeIndex(edges[i].to);
                if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + edges[i].weight < dist[v]) {
                    dist[v] = dist[u] + edges[i].weight;
                }
            }
        }
    }

    printf("Shortest distances from %c:\n", start);
    for (int i = 0; i < nodeCount; i++) {
        printf("%c: %d\n", nodes[i].name, dist[i]);
    }
}

int main() {
    addNode('A');
    addNode('B');
    addNode('C');
    addNode('D');

    addEdge('A', 'B', 1);
    addEdge('A', 'C', 4);
    addEdge('B', 'A', 1);
    addEdge('B', 'C', 2);
    addEdge('B', 'D', 5);
    addEdge('C', 'A', 4);
    addEdge('C', 'B', 2);
    addEdge('C', 'D', 1);
    addEdge('D', 'B', 5);
    addEdge('D', 'C', 1);

    dijkstra('A');

    return 0;
}
