#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int constrainedSubsetSum(int* nums, int numsSize, int k) {
    int n = numsSize;
    int* dp = (int*)malloc((n + 1) * sizeof(int));
    if (dp == NULL) {
        return INT_MIN; // allocation failure
    }
    for (int i = 0; i <= n; i++) {
        dp[i] = INT_MIN;
    }
    dp[0] = 0;

    int* deque = (int*)malloc((n + 1) * sizeof(int));
    if (deque == NULL) {
        free(dp);
        return INT_MIN; // allocation failure
    }
    int front = 0, rear = -1;

    for (int i = 0; i < n; i++) {
        while (front <= rear && deque[front] < i - k) {
            front++;
        }

        if (front <= rear) {
            dp[i + 1] = MAX(dp[i + 1], MAX(nums[i], dp[deque[front]] + nums[i]));
        } else {
            dp[i + 1] = MAX(dp[i + 1], nums[i]);
        }

        while (front <= rear && dp[deque[rear]] <= dp[i + 1]) {
            rear--;
        }
        rear++;
        deque[rear] = i + 1;
    }

    int maxSum = INT_MIN;
    for (int i = 1; i <= n; i++) {
        if (dp[i] > maxSum) maxSum = dp[i];
    }

    free(dp);
    free(deque);
    return maxSum;
}

void runTest(const char* testName, int* nums, int size, int k, int expected) {
    int result = constrainedSubsetSum(nums, size, k);
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
