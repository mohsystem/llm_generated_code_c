#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int vertex;
    struct Node* next;
};

struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
};

struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(struct Node*));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

int output[100];
int output_index = 0;

void DFS(struct Graph* graph, int vertex) {
    struct Node* adjList = graph->adjLists[vertex];
    struct Node* temp = adjList;
    graph->visited[vertex] = 1;
    output[output_index++] = vertex;

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
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
    struct Graph* graph = createGraph(4);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 0);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 3);

    // Reset visited and output
    memset(graph->visited, 0, sizeof(int) * graph->numVertices);
    output_index = 0;

    DFS(graph, 2);

    int expected[] = {2, 0, 1, 3};

    if (output_index == 4 && arraysEqual(output, expected, 4)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printArray(expected, 4);
        printf("Got:      ");
        printArray(output, output_index);
    }
}

int main() {
    runTest();
    return 0;
}
