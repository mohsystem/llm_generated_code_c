#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE 100

struct Queue {
    int items[SIZE];
    int front;
    int rear;
};

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(struct Queue* q) {
    return q->rear == -1;
}

void enqueue(struct Queue* q, int value) {
    if (q->rear == SIZE - 1) {
        // تجاهل حالة الطابور الممتلئ
        return;
    }
    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue* q) {
    if (isEmpty(q)) {
        return -1;
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

int peek(struct Queue* q) {
    if (isEmpty(q)) {
        return -1;
    }
    return q->items[q->front];
}

void run_tests() {
    struct Queue* q = createQueue();

    assert(isEmpty(q) == 1);

    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);
    assert(dequeue(q) == 10);
    assert(peek(q) == 20);

    // test dequeue all and empty queue
    assert(dequeue(q) == 20);
    assert(dequeue(q) == 30);
    assert(dequeue(q) == -1);
    assert(peek(q) == -1);
    assert(isEmpty(q) == 1);

    // test enqueue after empty
    enqueue(q, 5);
    assert(peek(q) == 5);
    assert(isEmpty(q) == 0);

    printf("✓ All tests passed for array-based queue implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
