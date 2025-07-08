#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NODES 100

struct Node {
    int data;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = (struct Node**)malloc(vertices * sizeof(struct Node*));
    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void enqueue(struct Node** head, struct Node** tail, int data) {
    struct Node* newNode = createNode(data);
    if (*tail == NULL) {
        *head = *tail = newNode;
    } else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

int dequeue(struct Node** head, struct Node** tail) {
    if (*head == NULL) return -1;
    struct Node* temp = *head;
    int data = temp->data;
    *head = temp->next;
    if (*head == NULL) *tail = NULL;
    free(temp);
    return data;
}

void bfs(struct Graph* graph, int startVertex, int* output, int* output_len) {
    bool* visited = (bool*)calloc(graph->numVertices, sizeof(bool));
    struct Node* queueHead = NULL;
    struct Node* queueTail = NULL;

    *output_len = 0;

    visited[startVertex] = true;
    enqueue(&queueHead, &queueTail, startVertex);

    while (queueHead != NULL) {
        int currentVertex = dequeue(&queueHead, &queueTail);
        output[(*output_len)++] = currentVertex;

        struct Node* adjList = graph->adjLists[currentVertex];
        while (adjList != NULL) {
            int adjVertex = adjList->data;
            if (!visited[adjVertex]) {
                visited[adjVertex] = true;
                enqueue(&queueHead, &queueTail, adjVertex);
            }
            adjList = adjList->next;
        }
    }

    free(visited);
}

bool arraysEqual(int* a, int* b, int size) {
    for (int i = 0; i < size; i++)
        if (a[i] != b[i]) return false;
    return true;
}

void runTests() {
    struct Graph* graph = createGraph(6);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 4, 5);

    int output[100];
    int out_len;

    bfs(graph, 0, output, &out_len);
    int expected[] = {0, 2, 1, 5, 4, 3};
    if (!arraysEqual(output, expected, out_len)) {
        printf("Test failed for BFS starting at 0\n");
        exit(1);
    }

    printf("All tests passed!\n");

    // Cleanup graph
    for (int i = 0; i < graph->numVertices; i++) {
        struct Node* curr = graph->adjLists[i];
        while (curr) {
            struct Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(graph->adjLists);
    free(graph);
}

int main() {
    runTests();
    return 0;
}
