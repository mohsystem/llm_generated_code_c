#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int *arr;
    int front;
    int rear;
    int size;
} Deque;

Deque *createDeque(int capacity) {
    Deque *dq = (Deque *)malloc(sizeof(Deque));
    dq->arr = (int *)malloc(sizeof(int) * capacity);
    dq->front = -1;
    dq->rear = -1;
    dq->size = capacity;
    return dq;
}

int isEmpty(Deque *dq) {
    return dq->front == -1;
}

int isFull(Deque *dq) {
    return ((dq->rear + 1) % dq->size == dq->front);
}

void insertRear(Deque *dq, int item) {
    if (isFull(dq)) return;
    if (isEmpty(dq)) {
        dq->front = 0;
        dq->rear = 0;
    } else {
        dq->rear = (dq->rear + 1) % dq->size;
    }
    dq->arr[dq->rear] = item;
}

void deleteFront(Deque *dq) {
    if (isEmpty(dq)) return;
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else {
        dq->front = (dq->front + 1) % dq->size;
    }
}

void deleteRear(Deque *dq) {
    if (isEmpty(dq)) return;
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else {
        dq->rear = (dq->rear - 1 + dq->size) % dq->size;
    }
}

int getFront(Deque *dq) {
    if (isEmpty(dq)) return -1;
    return dq->arr[dq->front];
}

int getRear(Deque *dq) {
    if (isEmpty(dq)) return -1;
    return dq->arr[dq->rear];
}

int shortestSubarray(int* nums, int numsSize, int k) {
    long long preSum[numsSize + 1];
    preSum[0] = 0;
    for (int i = 0; i < numsSize; i++) {
        preSum[i + 1] = preSum[i] + nums[i];
    }
    int res = INT_MAX;
    Deque *q = createDeque(numsSize + 1);
    for (int i = 0; i <= numsSize; i++) {
        while (!isEmpty(q) && preSum[i] - preSum[getFront(q)] >= k) {
            res = (res < i - getFront(q)) ? res : (i - getFront(q));
            deleteFront(q);
        }
        while (!isEmpty(q) && preSum[i] <= preSum[getRear(q)]) {
            deleteRear(q);
        }
        insertRear(q, i);
    }
    free(q->arr);
    free(q);
    return (res == INT_MAX) ? -1 : res;
}

void runTest(const char* name, int* nums, int size, int k, int expected) {
    int result = shortestSubarray(nums, size, k);
    if (result == expected) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", name, expected, result);
    }
}

int main() {
    int t1[] = {1};
    runTest("Test Case 1", t1, 1, 1, 1);

    int t2[] = {1, 2};
    runTest("Test Case 2", t2, 2, 4, -1);

    int t3[] = {2, -1, 2};
    runTest("Test Case 3", t3, 3, 3, 3);

    int t4[] = {1, 2, 3, 4, 5};
    runTest("Test Case 4", t4, 5, 11, 3);

    int t5[] = {1, 2, 3, 4, 5};
    runTest("Test Case 5", t5, 5, 15, 5);

    int t6[] = {10, -5, 10};
    runTest("Test Case 6", t6, 3, 10, 1);

    int t7[] = {-1, -1, -1, -1, 5};
    runTest("Test Case 7", t7, 5, 5, 1);

    int t8[] = {1, -2, 3, 4, 5};
    runTest("Test Case 8", t8, 5, 7, 2);

    int t9[] = {100, -50, 50, 50, 50};
    runTest("Test Case 9", t9, 5, 150, 3);

    int t10[] = {5, -10, 7, -2, 15};
    runTest("Test Case 10", t10, 5, 15, 1);

    return 0;
}
