#include <stdio.h>
#include <stdlib.h>

int maxSumSubsequence(int* nums, int numsSize, int k) {
    int* dp = (int*)malloc(numsSize * sizeof(int));
    dp[0] = nums[0];
    for (int i = 1; i < numsSize; i++) {
        dp[i] = dp[i-1] > nums[i] ? dp[i-1] : nums[i];
        int start = (i - k - 1) > -1 ? (i - k - 1) : -1;
        for (int j = i-1; j > start; j--) {
            int cand = dp[j] + nums[i];
            if (cand > dp[i]) dp[i] = cand;
        }
    }
    int maxSum = dp[0];
    for (int i = 1; i < numsSize; i++) {
        if (dp[i] > maxSum) maxSum = dp[i];
    }
    free(dp);
    return maxSum;
}

void runTest(const char* testName, int* nums, int size, int k, int expected) {
    int result = maxSumSubsequence(nums, size, k);
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
