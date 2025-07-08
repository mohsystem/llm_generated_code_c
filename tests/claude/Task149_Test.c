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
    struct Queue* queue = createQueue();

    // test_enqueue_and_peek
    enqueue(queue, 1);
    enqueue(queue, 2);
    assert(peek(queue) == 1);

    // test_dequeue
    assert(dequeue(queue) == 1);
    assert(peek(queue) == 2);

    // test_empty_queue
    struct Queue* queue2 = createQueue();
    assert(is_empty(queue2) == 1);
    enqueue(queue2, 1);
    assert(is_empty(queue2) == 0);

    // test_peek_empty_queue
    struct Queue* queue3 = createQueue();
    assert(peek(queue3) == -1);

    // test_dequeue_empty_queue
    struct Queue* queue4 = createQueue();
    assert(dequeue(queue4) == -1);

    // test_multiple_enqueue_and_dequeue
    struct Queue* queue5 = createQueue();
    enqueue(queue5, 10);
    enqueue(queue5, 20);
    enqueue(queue5, 30);
    assert(dequeue(queue5) == 10);
    assert(dequeue(queue5) == 20);
    assert(dequeue(queue5) == 30);

    printf("✓ All tests passed for queue implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
