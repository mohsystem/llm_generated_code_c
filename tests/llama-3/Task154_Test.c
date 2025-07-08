#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 100

typedef struct {
    int numNodes;
    int adjList[MAX_NODES][MAX_NODES];
} Graph;

void initGraph(Graph* g, int numNodes) {
    g->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            g->adjList[i][j] = 0;
        }
    }
}

void addEdge(Graph* g, int u, int v) {
    g->adjList[u][v] = 1;
}

int output[100];
int output_index = 0;

void dfs(Graph* g, int startNode, bool visited[]) {
    visited[startNode] = true;
    output[output_index++] = startNode;
    for (int neighbor = 0; neighbor < g->numNodes; neighbor++) {
        if (g->adjList[startNode][neighbor] && !visited[neighbor]) {
            dfs(g, neighbor, visited);
        }
    }
}

int arraysEqual(int *a, int *b, int size) {
    for (int i=0; i<size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void printArray(int *arr, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void runTest() {
    Graph g;
    initGraph(&g, 5);
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);

    bool visited[5];
    for (int i=0; i<5; i++) visited[i] = false;
    output_index = 0;

    dfs(&g, 0, visited);

    int expected[] = {0, 1, 3, 2, 4};
    int expected_size = 5;

    if (output_index == expected_size && arraysEqual(output, expected, expected_size)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printArray(expected, expected_size);
        printf("Got:      ");
        printArray(output, output_index);
    }
}

int main() {
    runTest();
    return 0;
}
