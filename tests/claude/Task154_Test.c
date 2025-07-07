#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

struct Graph {
    int V;
    bool adj[MAX_VERTICES][MAX_VERTICES];
};

struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph->adj[i][j] = false;
        }
    }

    return graph;
}

void addEdge(struct Graph* graph, int v, int w) {
    graph->adj[v][w] = true;
}

void DFSUtil(struct Graph* graph, int v, bool visited[], int* output, int* index) {
    visited[v] = true;
    output[(*index)++] = v;

    for (int i = 0; i < graph->V; i++) {
        if (graph->adj[v][i] && !visited[i]) {
            DFSUtil(graph, i, visited, output, index);
        }
    }
}

void DFS(struct Graph* graph, int v, int* output, int* length) {
    bool visited[MAX_VERTICES] = {false};
    *length = 0;
    DFSUtil(graph, v, visited, output, length);
}

int arraysEqual(int* a, int* b, int size) {
    for (int i=0; i<size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void printArray(int* arr, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void runTests() {
    struct Graph* graph = createGraph(4);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 0);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 3);

    int output[MAX_VERTICES];
    int length;

    int expected[] = {2, 0, 1, 3};  // Expected DFS order from vertex 2

    DFS(graph, 2, output, &length);

    if (length == 4 && arraysEqual(output, expected, length)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printArray(expected, 4);
        printf("Got:      ");
        printArray(output, length);
    }

    free(graph);
}

int main() {
    runTests();
    return 0;
}
