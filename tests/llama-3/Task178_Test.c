#include <stdio.h>

int shortestSubarray(int* nums, int numsSize, int k) {
    int n = numsSize;
    int prefixSum[n + 1];
    prefixSum[0] = 0;
    for (int i = 0; i < n; i++) {
        prefixSum[i + 1] = prefixSum[i] + nums[i];
    }
    int res = n + 1;
    int queue[n + 1];
    int front = 0, rear = 0;
    for (int i = 0; i <= n; i++) {
        while (front < rear && prefixSum[i] - prefixSum[queue[front]] >= k) {
            res = res < i - queue[front] ? res : i - queue[front];
            front++;
        }
        while (front < rear && prefixSum[i] <= prefixSum[queue[rear - 1]]) {
            rear--;
        }
        queue[rear++] = i;
    }
    return res <= n ? res : -1;
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
