#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int constrainedSubsetSum(int* nums, int numsSize, int k) {
    int* dq = (int*)malloc(numsSize * sizeof(int));
    int front = 0, rear = -1;
    int maxSum = nums[0];

    for (int i = 0; i < numsSize; i++) {
        if (front <= rear) {
            nums[i] += dq[front];
        }
        while (front <= rear && dq[rear] < nums[i]) {
            rear--;
        }
        if (nums[i] > 0) {
            dq[++rear] = nums[i];
        }
        if (i >= k && front <= rear && dq[front] == nums[i - k]) {
            front++;
        }
        if (nums[i] > maxSum) maxSum = nums[i];
    }

    free(dq);
    return maxSum;
}

void runTest(const char* testName, int* nums, int size, int k, int expected) {
    // نسخ المصفوفة لأن الدالة تغيرها
    int* numsCopy = (int*)malloc(sizeof(int)*size);
    for(int i=0; i<size; i++) numsCopy[i] = nums[i];

    int result = constrainedSubsetSum(numsCopy, size, k);
    free(numsCopy);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
}

int main() {
    int t1[] = {10, 2, -10, 5, 20};
    runTest("Test Case 1", t1, 5, 2, 37);

    int t2[] = {-1, -2, -3};
    runTest("Test Case 2", t2, 3, 1, -1);

    int t3[] = {10, -2, -10, -5, 20};
    runTest("Test Case 3", t3, 5, 2, 23);

    int t4[] = {1, 2, 3, 4, 5};
    runTest("Test Case 4", t4, 5, 2, 15);

    int t5[] = {-1, 0, 1};
    runTest("Test Case 5", t5, 3, 2, 1);

    int t6[] = {1, -1, 2, 3, -2};
    runTest("Test Case 6", t6, 5, 2, 6);

    int t7[] = {5, -1, 3, -2, 4, -1};
    runTest("Test Case 7", t7, 6, 3, 12);

    int t8[] = {2, 1, -2, 3, -4};
    runTest("Test Case 8", t8, 5, 1, 6);

    int t9[] = {10, -2, 3, -4, 5};
    runTest("Test Case 9", t9, 5, 4, 18);

    int t10[] = {1, -2, 3, 10, -1};
    runTest("Test Case 10", t10, 5, 2, 14);

    return 0;
}
