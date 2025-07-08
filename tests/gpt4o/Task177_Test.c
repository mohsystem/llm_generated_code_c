#include <stdio.h>
#include <stdlib.h>

int maxSubsequenceSum(int* nums, int numsSize, int k) {
    if (numsSize == 0) return 0;

    int* dp = (int*)malloc(numsSize * sizeof(int));
    dp[0] = nums[0];
    int* deq = (int*)malloc(numsSize * sizeof(int));
    int front = 0, back = 0;

    deq[back++] = 0;

    for (int i = 1; i < numsSize; i++) {
        while (front < back && deq[front] < i - k) {
            front++;
        }
        dp[i] = nums[i] + ((front < back) ? dp[deq[front]] : 0);
        while (front < back && dp[i] >= dp[deq[back - 1]]) {
            back--;
        }
        deq[back++] = i;
    }

    int maxSum = dp[0];
    for (int i = 1; i < numsSize; i++) {
        if (dp[i] > maxSum) {
            maxSum = dp[i];
        }
    }

    free(dp);
    free(deq);

    return maxSum;
}

void runTest(const char* name, int* nums, int size, int k, int expected) {
    int result = maxSubsequenceSum(nums, size, k);
    if (result == expected) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", name, expected, result);
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
