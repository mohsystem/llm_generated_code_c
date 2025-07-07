#include <stdio.h>
#include <stdlib.h>

// دالة للتحقق من وجود مجموع النطاق ضمن [lower, upper] لكل i, j بطريقة محسنة (بس باستخدام prefix sums + فحص جمييييع)
// هذه طريقة O(n^2) وليست فعالة لكن دقيقة للأحجام الصغيرة.

int rangeSumBruteForce(int* nums, int numsSize, int lower, int upper) {
    long long* prefixSum = (long long*)malloc((numsSize + 1) * sizeof(long long));
    prefixSum[0] = 0;
    for (int i = 0; i < numsSize; i++) {
        prefixSum[i + 1] = prefixSum[i] + nums[i];
    }

    int count = 0;
    for (int i = 0; i < numsSize; i++) {
        for (int j = i; j < numsSize; j++) {
            long long rangeSum = prefixSum[j + 1] - prefixSum[i];
            if (rangeSum >= lower && rangeSum <= upper) {
                count++;
            }
        }
    }
    free(prefixSum);
    return count;
}

void runTest(int* nums, int numsSize, int lower, int upper, int expected, const char* testName) {
    int result = rangeSumBruteForce(nums, numsSize, lower, upper);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
}

int main() {
    int nums1[] = {-2, 5, -1};
    runTest(nums1, 3, -2, 2, 3, "Test Case 1");

    int nums2[] = {0};
    runTest(nums2, 1, 0, 0, 1, "Test Case 2");

    int nums3[] = {1, 2, 3, 4};
    runTest(nums3, 4, 3, 6, 4, "Test Case 3");

    int nums4[] = {-5, -3, -2, -1};
    runTest(nums4, 4, -8, -3, 5, "Test Case 4");

    int nums5[] = {1, -1, 1, -1};
    runTest(nums5, 4, 0, 0, 4, "Test Case 5");

    int nums6[] = {2, -1, 2};
    runTest(nums6, 3, -10, 10, 6, "Test Case 6");

    int nums7[] = {1, 2, 3};
    runTest(nums7, 3, 10, 20, 0, "Test Case 7");

    int nums8[] = {5};
    runTest(nums8, 1, 5, 5, 1, "Test Case 8");

    int nums9[] = {5};
    runTest(nums9, 1, 6, 10, 0, "Test Case 9");

    int nums10[] = {3, -1, 4, -2, 2};
    runTest(nums10, 5, 1, 5, 9, "Test Case 10");

    return 0;
}
