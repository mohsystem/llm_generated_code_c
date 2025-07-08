#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

struct Node {
    int vertex;
    struct Node* next;
};

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
};

struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

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

struct Queue* createQueue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

int isEmpty(struct Queue* queue) {
    return queue->rear == -1;
}

void enqueue(struct Queue* queue, int value) {
    if (queue->rear == MAX_NODES - 1) {
        printf("\nQueue is Full!!");
    } else {
        if (queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->items[queue->rear] = value;
    }
}

int dequeue(struct Queue* queue) {
    int item;
    if (isEmpty(queue)) {
        printf("Queue is empty");
        item = -1;
    } else {
        item = queue->items[queue->front];
        queue->front++;
        if (queue->front > queue->rear) {
            queue->front = queue->rear = -1;
        }
    }
    return item;
}

// Modified BFS to capture output in array instead of printing
void bfs_capture(struct Graph* graph, int startVertex, int* output, int* length) {
    struct Queue* queue = createQueue();

    // Reset visited
    for (int i = 0; i < graph->numVertices; i++) {
        graph->visited[i] = 0;
    }

    graph->visited[startVertex] = 1;
    enqueue(queue, startVertex);

    *length = 0;

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        output[(*length)++] = currentVertex;

        struct Node* temp = graph->adjLists[currentVertex];

        while (temp) {
            int adjVertex = temp->vertex;

            if (graph->visited[adjVertex] == 0) {
                graph->visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
}

int arraysEqual(int* arr1, int* arr2, int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) return 0;
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

    int output[MAX_NODES];
    int length;

    // Expected BFS starting at vertex 2
    int expected[] = {2, 0, 3, 1};

    bfs_capture(graph, 2, output, &length);

    if (length == 4 && arraysEqual(output, expected, length)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printArray(expected, 4);
        printf("Got:      ");
        printArray(output, length);
    }
}

int main() {
    runTests();
    return 0;
}
