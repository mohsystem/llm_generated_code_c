#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int merge_sort(int* nums, int start, int end, int* temp) {
    if (end - start <= 1) {
        return 0;
    }

    int mid = (start + end) / 2;
    int count = 0;

    count += merge_sort(nums, start, mid, temp);
    count += merge_sort(nums, mid, end, temp);

    int j = mid;
    for (int i = start; i < mid; i++) {
        while (j < end && (long long)nums[i] > 2LL * (long long)nums[j]) {
            j++;
        }
        count += j - mid;
    }

    int l = start, r = mid, k = start;
    while (l < mid && r < end) {
        if (nums[l] <= nums[r]) {
            temp[k++] = nums[l++];
        } else {
            temp[k++] = nums[r++];
        }
    }
    while (l < mid) {
        temp[k++] = nums[l++];
    }
    while (r < end) {
        temp[k++] = nums[r++];
    }

    memcpy(nums + start, temp + start, (end - start) * sizeof(int));

    return count;
}

int reversePairs(int* nums, int numsSize) {
    int* temp = (int*)malloc(numsSize * sizeof(int));
    int count = merge_sort(nums, 0, numsSize, temp);
    free(temp);
    return count;
}

void runTest(int* nums, int numsSize, int expected, const char* testName) {
    int* copy = malloc(numsSize * sizeof(int));
    for (int i = 0; i < numsSize; i++) copy[i] = nums[i];

    int result = reversePairs(copy, numsSize);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
    free(copy);
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
