#include <stdio.h>
#include <stdlib.h>

int reversePairs(int* nums, int numsSize) {
    if (numsSize <= 1) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < numsSize - 1; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if ((long long)nums[i] > 2LL * (long long)nums[j]) {
                count++;
            }
        }
    }
    return count;
}

void runTest(int* nums, int numsSize, int expected, const char* testName) {
    int result = reversePairs(nums, numsSize);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
}

int main() {
    int nums1[] = {1, 3, 2, 3, 1};
    int nums2[] = {2, 4, 3, 5, 1};
    int nums3[] = {1, 2, 3, 4, 5};
    int nums4[] = {5, 4, 3, 2, 1};
    int nums5[] = {1, 5, 2, 6, 3};
    int nums6[] = {1};
    int nums7[] = {3, 1, 4, 2, 5};
    int nums8[] = {10, 5, 3, 2, 1};
    int nums9[] = {4, 2, 6, 1, 3};
    int nums10[] = {7, 5, 8, 2, 4};

    runTest(nums1, 5, 2, "Test Case 1");
    runTest(nums2, 5, 3, "Test Case 2");
    runTest(nums3, 5, 0, "Test Case 3");
    runTest(nums4, 5, 6, "Test Case 4");
    runTest(nums5, 5, 2, "Test Case 5");
    runTest(nums6, 1, 0, "Test Case 6");
    runTest(nums7, 5, 1, "Test Case 7");
    runTest(nums8, 5, 6, "Test Case 8");
    runTest(nums9, 5, 3, "Test Case 9");
    runTest(nums10, 5, 3, "Test Case 10");

    printf("All tests done.\n");
    return 0;
}
