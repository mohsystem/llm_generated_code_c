#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

struct Node {
    int data;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    bool* visited;
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
    graph->visited = (bool*)malloc(vertices * sizeof(bool));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = false;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void BFS(struct Graph* graph, int startVertex, int* output, int* out_len) {
    int queue[MAX_VERTICES];
    int front = 0, rear = -1;

    for (int i = 0; i < graph->numVertices; i++)
        graph->visited[i] = false;

    graph->visited[startVertex] = true;
    queue[++rear] = startVertex;

    *out_len = 0;

    while (front <= rear) {
        int currentVertex = queue[front++];
        output[(*out_len)++] = currentVertex;
        struct Node* temp = graph->adjLists[currentVertex];
        while (temp) {
            int adjVertex = temp->data;
            if (!graph->visited[adjVertex]) {
                graph->visited[adjVertex] = true;
                queue[++rear] = adjVertex;
            }
            temp = temp->next;
        }
    }
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

    int output[MAX_VERTICES];
    int out_len;

    BFS(graph, 0, output, &out_len);

    int expected[] = {0, 2, 1, 5, 4, 3};

    if (!arraysEqual(output, expected, out_len)) {
        printf("Test failed: BFS output does not match expected\n");
        printf("Expected: ");
        for (int i = 0; i < out_len; i++) printf("%d ", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < out_len; i++) printf("%d ", output[i]);
        printf("\n");
        exit(1);
    }

    printf("All test cases passed!\n");

    // Cleanup graph memory
    for (int i = 0; i < graph->numVertices; i++) {
        struct Node* curr = graph->adjLists[i];
        while (curr) {
            struct Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(graph->adjLists);
    free(graph->visited);
    free(graph);
}

int main() {
    runTests();
    return 0;
}
