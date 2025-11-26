#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int data;
    struct Node* next;
};

struct Queue {
    struct Node *front, *rear;
};

struct Node* newNode(int data) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(struct Queue* q, int data) {
    struct Node* temp = newNode(data);
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

int dequeue(struct Queue* q) {
    if (q->front == NULL)
        return -1;
    struct Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return data;
}

int peek(struct Queue* q) {
    if (q->front == NULL)
        return -1;
    return q->front->data;
}

int is_empty(struct Queue* q) {
    return q->front == NULL;
}

void run_tests() {
    struct Queue* q1 = createQueue();
    enqueue(q1, 1);
    enqueue(q1, 2);
    assert(peek(q1) == 1);

    assert(dequeue(q1) == 1);
    assert(peek(q1) == 2);

    struct Queue* q2 = createQueue();
    assert(is_empty(q2) == 1);
    enqueue(q2, 1);
    assert(is_empty(q2) == 0);

    struct Queue* q3 = createQueue();
    assert(peek(q3) == -1);

    struct Queue* q4 = createQueue();
    assert(dequeue(q4) == -1);

    struct Queue* q5 = createQueue();
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
