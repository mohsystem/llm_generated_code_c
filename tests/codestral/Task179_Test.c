#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* data;
    int front;
    int rear;
    int size;
    int capacity;
} Deque;

Deque* createDeque(int capacity) {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    deque->data = (int*)malloc(capacity * sizeof(int));
    deque->front = -1;
    deque->rear = -1;
    deque->size = 0;
    deque->capacity = capacity;
    return deque;
}

void destroyDeque(Deque* deque) {
    free(deque->data);
    free(deque);
}

void pushBack(Deque* deque, int value) {
    if (deque->size == deque->capacity) return;
    if (deque->rear == -1) {
        deque->front = 0;
        deque->rear = 0;
    } else {
        deque->rear = (deque->rear + 1) % deque->capacity;
    }
    deque->data[deque->rear] = value;
    deque->size++;
}

void pushFront(Deque* deque, int value) {
    if (deque->size == deque->capacity) return;
    if (deque->rear == -1) {
        deque->front = 0;
        deque->rear = 0;
    } else {
        deque->front = (deque->front - 1 + deque->capacity) % deque->capacity;
    }
    deque->data[deque->front] = value;
    deque->size++;
}

int popFront(Deque* deque) {
    if (deque->size == 0) return -1;
    int value = deque->data[deque->front];
    if (deque->front == deque->rear) {
        deque->front = -1;
        deque->rear = -1;
    } else {
        deque->front = (deque->front + 1) % deque->capacity;
    }
    deque->size--;
    return value;
}

int popBack(Deque* deque) {
    if (deque->size == 0) return -1;
    int value = deque->data[deque->rear];
    if (deque->front == deque->rear) {
        deque->front = -1;
        deque->rear = -1;
    } else {
        deque->rear = (deque->rear - 1 + deque->capacity) % deque->capacity;
    }
    deque->size--;
    return value;
}

int front(Deque* deque) {
    if (deque->size == 0) return -1;
    return deque->data[deque->front];
}

int back(Deque* deque) {
    if (deque->size == 0) return -1;
    return deque->data[deque->rear];
}

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    int* result = (int*)malloc((numsSize - k + 1) * sizeof(int));
    *returnSize = numsSize - k + 1;
    Deque* window = createDeque(numsSize);

    for (int i = 0; i < numsSize; i++) {
        while (window->size > 0 && front(window) <= i - k) {
            popFront(window);
        }
        while (window->size > 0 && nums[back(window)] < nums[i]) {
            popBack(window);
        }
        pushBack(window, i);
        if (i >= k - 1) {
            result[i - k + 1] = nums[front(window)];
        }
    }

    destroyDeque(window);
    return result;
}

void runTest(const char* name, int* nums, int size, int k, int* expected, int expectedSize) {
    int returnSize = 0;
    int* result = maxSlidingWindow(nums, size, k, &returnSize);

    int pass = (returnSize == expectedSize);
    for (int i = 0; i < returnSize && pass; i++) {
        if (result[i] != expected[i]) {
            pass = 0;
        }
    }

    if (pass) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s\nExpected: ", name);
        for (int i = 0; i < expectedSize; i++) printf("%d ", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < returnSize; i++) printf("%d ", result[i]);
        printf("\n");
    }

    free(result);
}

int main() {
    int t1[] = {1,3,-1,-3,5,3,6,7};
    int e1[] = {3,3,5,5,6,7};
    runTest("Test Case 1", t1, 8, 3, e1, 6);

    int t2[] = {1};
    int e2[] = {1};
    runTest("Test Case 2", t2, 1, 1, e2, 1);

    int t3[] = {1,-1};
    int e3[] = {1,-1};
    runTest("Test Case 3", t3, 2, 1, e3, 2);

    int t4[] = {9,11};
    int e4[] = {11};
    runTest("Test Case 4", t4, 2, 2, e4, 1);

    int t5[] = {4,-2};
    int e5[] = {4};
    runTest("Test Case 5", t5, 2, 2, e5, 1);

    int t6[] = {7,2,4};
    int e6[] = {7,4};
    runTest("Test Case 6", t6, 3, 2, e6, 2);

    int t7[] = {1,3,1,2,0,5};
    int e7[] = {3,3,2,5};
    runTest("Test Case 7", t7, 6, 3, e7, 4);

    int t8[] = {1,2,3,4,5,6,7,8,9};
    int e8[] = {3,4,5,6,7,8,9};
    runTest("Test Case 8", t8, 9, 3, e8, 7);

    int t9[] = {10,9,8,7,6,5,4,3,2,1};
    int e9[] = {10,9,8,7,6,5,4};
    runTest("Test Case 9", t9, 10, 4, e9, 7);

    int t10[] = {5,5,5,5,5};
    int e10[] = {5,5,5,5};
    runTest("Test Case 10", t10, 5, 2, e10, 4);

    return 0;
}
