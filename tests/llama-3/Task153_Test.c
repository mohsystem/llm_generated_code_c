#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct Graph {
    int numNodes;
    int adjList[MAX_NODES][MAX_NODES];
    bool visited[MAX_NODES];
} Graph;

void addEdge(Graph* g, int u, int v) {
    g->adjList[u][v] = 1;
}

void bfs_capture(Graph* g, int startNode, int* output, int* length) {
    int queue[MAX_NODES];
    int front = 0, rear = 0;

    // Reset visited before BFS
    for (int i = 0; i < g->numNodes; i++) {
        g->visited[i] = false;
    }

    queue[rear++] = startNode;
    g->visited[startNode] = true;

    *length = 0;

    while (front < rear) {
        int node = queue[front++];
        output[(*length)++] = node;

        for (int neighbor = 0; neighbor < g->numNodes; neighbor++) {
            if (g->adjList[node][neighbor] && !g->visited[neighbor]) {
                queue[rear++] = neighbor;
                g->visited[neighbor] = true;
            }
        }
    }
}

bool arraysEqual(int* arr1, int* arr2, int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) return false;
    }
    return true;
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void runTests() {
    Graph g;
    g.numNodes = 5;

    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);

    int output[MAX_NODES];
    int length;

    // Test 1: BFS from node 0
    int expected1[] = {0, 1, 2, 3, 4};
    bfs_capture(&g, 0, output, &length);
    if (length == 5 && arraysEqual(output, expected1, length)) {
        printf("Test 1 Passed\n");
    } else {
        printf("Test 1 Failed: Expected ");
        printArray(expected1, 5);
        printf("Got      ");
        printArray(output, length);
    }

    // Test 2: BFS from node 1
    int expected2[] = {1, 3};
    bfs_capture(&g, 1, output, &length);
    if (length == 2 && arraysEqual(output, expected2, length)) {
        printf("Test 2 Passed\n");
    } else {
        printf("Test 2 Failed: Expected ");
        printArray(expected2, 2);
        printf("Got      ");
        printArray(output, length);
    }

    // Test 3: BFS from node 2
    int expected3[] = {2, 4};
    bfs_capture(&g, 2, output, &length);
    if (length == 2 && arraysEqual(output, expected3, length)) {
        printf("Test 3 Passed\n");
    } else {
        printf("Test 3 Failed: Expected ");
        printArray(expected3, 2);
        printf("Got      ");
        printArray(output, length);
    }

    // Test 4: BFS from node 3 (isolated node, no outgoing edges)
    int expected4[] = {3};
    bfs_capture(&g, 3, output, &length);
    if (length == 1 && arraysEqual(output, expected4, length)) {
        printf("Test 4 Passed\n");
    } else {
        printf("Test 4 Failed: Expected ");
        printArray(expected4, 1);
        printf("Got      ");
        printArray(output, length);
    }
}

int main() {
    runTests();
    return 0;
}
