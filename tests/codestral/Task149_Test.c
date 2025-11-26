#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int item) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = item;
    temp->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

int dequeue(Queue* q) {
    if (q->front == NULL) {
        return -1;
    }

    Node* temp = q->front;
    int item = temp->data;
    q->front = temp->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return item;
}

int peek(Queue* q) {
    if (q->front == NULL) {
        return -1;
    }

    return q->front->data;
}

int is_empty(Queue* q) {
    return q->front == NULL;
}

void run_tests() {
    Queue* q1 = createQueue();
    enqueue(q1, 1);
    enqueue(q1, 2);
    assert(peek(q1) == 1);

    assert(dequeue(q1) == 1);
    assert(peek(q1) == 2);

    Queue* q2 = createQueue();
    assert(is_empty(q2) == 1);
    enqueue(q2, 1);
    assert(is_empty(q2) == 0);

    Queue* q3 = createQueue();
    assert(peek(q3) == -1);

    Queue* q4 = createQueue();
    assert(dequeue(q4) == -1);

    Queue* q5 = createQueue();
    enqueue(q5, 10);
    enqueue(q5, 20);
    enqueue(q5, 30);
    assert(dequeue(q5) == 10);
    assert(dequeue(q5) == 20);
    assert(dequeue(q5) == 30);

    printf("✓ All tests passed for queue implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
