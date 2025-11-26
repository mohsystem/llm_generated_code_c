#include <stdio.h>
#include <stdlib.h>

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    if (nums == NULL || numsSize == 0 || k <= 0) {
        *returnSize = 0;
        return NULL;
    }

    int* result = (int*)malloc(sizeof(int) * (numsSize - k + 1));
    int ri = 0;

    int* deque = (int*)malloc(sizeof(int) * numsSize);
    int front = 0, rear = -1;

    for (int i = 0; i < numsSize; i++) {
        while (front <= rear && deque[front] < i - k + 1) {
            front++;
        }
        while (front <= rear && nums[deque[rear]] < nums[i]) {
            rear--;
        }
        deque[++rear] = i;

        if (i >= k - 1) {
            result[ri++] = nums[deque[front]];
        }
    }

    free(deque);
    *returnSize = ri;
    return result;
}

void runTest(const char* name, int* nums, int size, int k, int* expected, int expectedSize) {
    int returnSize = 0;
    int* res = maxSlidingWindow(nums, size, k, &returnSize);

    int pass = (returnSize == expectedSize);
    for (int i = 0; i < returnSize && pass; i++) {
        if (res[i] != expected[i]) {
            pass = 0;
        }
    }
    if (pass) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s\nExpected: ", name);
        for (int i = 0; i < expectedSize; i++) printf("%d ", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < returnSize; i++) printf("%d ", res[i]);
        printf("\n");
    }
    free(res);
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
