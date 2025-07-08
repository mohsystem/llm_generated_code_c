#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VERTICES 100

typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(Queue* q) {
    return q->rear == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_VERTICES - 1)
        printf("Queue is full\n");
    else {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(Queue* q) {
    int item;
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        item = -1;
    } else {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear) {
            q->front = q->rear = -1;
        }
    }
    return item;
}

void bfs(int graph[MAX_VERTICES][MAX_VERTICES], int start, int vertices, int* output, int* output_len) {
    bool visited[MAX_VERTICES] = {false};
    Queue q;
    initQueue(&q);

    *output_len = 0;

    visited[start] = true;
    output[(*output_len)++] = start;
    enqueue(&q, start);

    while (!isEmpty(&q)) {
        int currentVertex = dequeue(&q);

        for (int i = 0; i < vertices; i++) {
            if (graph[currentVertex][i] == 1 && !visited[i]) {
                visited[i] = true;
                output[(*output_len)++] = i;
                enqueue(&q, i);
            }
        }
    }
}

bool arraysEqual(int* a, int* b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void runTests() {
    int graph1[MAX_VERTICES][MAX_VERTICES] = {
        {0,1,1,0,0,0},
        {1,0,0,1,1,0},
        {1,0,0,0,0,1},
        {0,1,0,0,0,0},
        {0,1,0,0,0,1},
        {0,0,1,0,1,0}
    };
    int output[MAX_VERTICES];
    int out_len;

    // Test BFS from vertex 0
    bfs(graph1, 0, 6, output, &out_len);
    int expected0[] = {0,1,2,3,4,5};
    if (!arraysEqual(output, expected0, out_len)) {
        printf("Test BFS from 0 failed\n");
        exit(1);
    }

    // Test BFS from vertex 3
    bfs(graph1, 3, 6, output, &out_len);
    int expected3[] = {3,1,0,4,2,5};
    if (!arraysEqual(output, expected3, out_len)) {
        printf("Test BFS from 3 failed\n");
        exit(1);
    }

    // Test BFS from vertex 5
    bfs(graph1, 5, 6, output, &out_len);
    int expected5[] = {5,2,4,0,1,3};
    if (!arraysEqual(output, expected5, out_len)) {
        printf("Test BFS from 5 failed\n");
        exit(1);
    }

    // Test BFS on disconnected graph
    int graph2[MAX_VERTICES][MAX_VERTICES] = {0};
    int vertices2 = 4;
    graph2[0][1] = graph2[1][0] = 1;
    graph2[2][3] = graph2[3][2] = 1;

    bfs(graph2, 0, vertices2, output, &out_len);
    int expected2_0[] = {0,1};
    if (!arraysEqual(output, expected2_0, out_len)) {
        printf("Test BFS on disconnected graph from 0 failed\n");
        exit(1);
    }

    bfs(graph2, 2, vertices2, output, &out_len);
    int expected2_2[] = {2,3};
    if (!arraysEqual(output, expected2_2, out_len)) {
        printf("Test BFS on disconnected graph from 2 failed\n");
        exit(1);
    }

    printf("All BFS test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
