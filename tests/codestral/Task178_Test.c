#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int shortestSubarray(int* nums, int numsSize, int k) {
    long* prefixSum = (long*)malloc((numsSize + 1) * sizeof(long));
    prefixSum[0] = 0;
    for (int i = 0; i < numsSize; i++) {
        prefixSum[i + 1] = prefixSum[i] + nums[i];
    }

    int minLength = numsSize + 1;
    int* monoQueue = (int*)malloc(numsSize * sizeof(int));
    int front = 0, rear = -1;
    for (int i = 0; i <= numsSize; i++) {
        while (front <= rear && prefixSum[i] - prefixSum[monoQueue[front]] >= k) {
            minLength = (minLength < i - monoQueue[front]) ? minLength : i - monoQueue[front];
            front++;
        }
        while (front <= rear && prefixSum[i] <= prefixSum[monoQueue[rear]]) {
            rear--;
        }
        rear++;
        monoQueue[rear] = i;
    }

    free(prefixSum);
    free(monoQueue);

    return minLength <= numsSize ? minLength : -1;
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
