#include <stdio.h>

int merge(int nums[], int left, int mid, int right) {
    int temp[right - left + 1];
    int left_index = left, right_index = mid + 1, temp_index = 0, count = 0;
    while (left_index <= mid && right_index <= right) {
        if (nums[left_index] <= 2LL * nums[right_index]) {
            temp[temp_index++] = nums[left_index++];
        } else {
            temp[temp_index++] = nums[right_index++];
            count += mid - left_index + 1;
        }
    }
    while (left_index <= mid) {
        temp[temp_index++] = nums[left_index++];
    }
    while (right_index <= right) {
        temp[temp_index++] = nums[right_index++];
    }
    for (int i = 0; i < right - left + 1; i++) {
        nums[left + i] = temp[i];
    }
    return count;
}

int mergeSort(int nums[], int left, int right) {
    if (left >= right) {
        return 0;
    }
    int mid = left + (right - left) / 2;
    int left_count = mergeSort(nums, left, mid);
    int right_count = mergeSort(nums, mid + 1, right);
    int merged_count = merge(nums, left, mid, right);
    return left_count + right_count + merged_count;
}

int reversePairs(int nums[], int numsSize) {
    return mergeSort(nums, 0, numsSize - 1);
}

void runTest(int* nums, int numsSize, int expected, const char* testName) {
    int copy[numsSize];
    for (int i = 0; i < numsSize; i++) copy[i] = nums[i];

    int result = reversePairs(copy, numsSize);
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
