#include <stdio.h>
#include <stdlib.h>

int merge(long long* arr, int left, int mid, int right, int lower, int upper) {
    int count = 0;
    int j = mid + 1, k = mid + 1;
    for (int i = left; i <= mid; i++) {
        while (j <= right && arr[j] - arr[i] < lower) j++;
        while (k <= right && arr[k] - arr[i] <= upper) k++;
        count += k - j;
    }

    long long* temp = (long long*)malloc((right - left + 1) * sizeof(long long));
    int i = left, m = mid + 1, t = 0;
    while (i <= mid && m <= right) {
        if (arr[i] <= arr[m]) temp[t++] = arr[i++];
        else temp[t++] = arr[m++];
    }
    while (i <= mid) temp[t++] = arr[i++];
    while (m <= right) temp[t++] = arr[m++];
    for (i = 0; i < right - left + 1; i++) arr[left + i] = temp[i];
    free(temp);
    return count;
}

int mergeSort(long long* arr, int left, int right, int lower, int upper) {
    if (left >= right) return 0;
    int mid = (left + right) / 2;
    int count = mergeSort(arr, left, mid, lower, upper) + mergeSort(arr, mid + 1, right, lower, upper);
    return count + merge(arr, left, mid, right, lower, upper);
}

int countRangeSum(int* nums, int numsSize, int lower, int upper) {
    long long* prefixSum = (long long*)malloc((numsSize + 1) * sizeof(long long));
    prefixSum[0] = 0;
    for (int i = 0; i < numsSize; i++) {
        prefixSum[i + 1] = prefixSum[i] + nums[i];
    }
    int result = mergeSort(prefixSum, 0, numsSize, lower, upper);
    free(prefixSum);
    return result;
}

void runTest(int* nums, int numsSize, int lower, int upper, int expected, const char* testName) {
    int result = countRangeSum(nums, numsSize, lower, upper);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
}

int main() {
    // Test 1
    int nums1[] = {-2, 5, -1};
    runTest(nums1, 3, -2, 2, 3, "Test Case 1");

    // Test 2
    int nums2[] = {0};
    runTest(nums2, 1, 0, 0, 1, "Test Case 2");

    // Test 3: all positive, range includes sums
    int nums3[] = {1, 2, 3, 4};
    runTest(nums3, 4, 3, 6, 4, "Test Case 3");

    // Test 4: all negative, range includes sums
    int nums4[] = {-5, -3, -2, -1};
    runTest(nums4, 4, -8, -3, 5, "Test Case 4");

    // Test 5: zero range includes only zero sums
    int nums5[] = {1, -1, 1, -1};
    runTest(nums5, 4, 0, 0, 4, "Test Case 5");

    // Test 6: large range covers all sums
    int nums6[] = {2, -1, 2};
    runTest(nums6, 3, -10, 10, 6, "Test Case 6");

    // Test 7: range excludes all sums
    int nums7[] = {1, 2, 3};
    runTest(nums7, 3, 10, 20, 0, "Test Case 7");

    // Test 8: single element array, range covers element
    int nums8[] = {5};
    runTest(nums8, 1, 5, 5, 1, "Test Case 8");

    // Test 9: single element array, range excludes element
    int nums9[] = {5};
    runTest(nums9, 1, 6, 10, 0, "Test Case 9");

    // Test 10: mixed positive and negative numbers
    int nums10[] = {3, -1, 4, -2, 2};
    runTest(nums10, 5, 1, 5, 9, "Test Case 10");

    return 0;
}
