#include <stdio.h>
#include <stdlib.h>

typedef long long int lli;

int count;

void mergeSort(lli* sums, int lo, int hi, int lower, int upper);

int countRangeSum(int* nums, int numsSize, int lower, int upper) {
    count = 0;
    lli* sums = (lli*)malloc((numsSize + 1) * sizeof(lli));
    sums[0] = 0;
    for (int i = 0; i < numsSize; ++i) {
        sums[i + 1] = sums[i] + nums[i];
    }
    mergeSort(sums, 0, numsSize, lower, upper);
    free(sums);
    return count;
}

void mergeSort(lli* sums, int lo, int hi, int lower, int upper) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    mergeSort(sums, lo, mid, lower, upper);
    mergeSort(sums, mid + 1, hi, lower, upper);

    int i = lo, m = mid + 1, n = mid + 1;
    while (i <= mid) {
        while (m <= hi && sums[m] - sums[i] < lower) m++;
        while (n <= hi && sums[n] - sums[i] <= upper) n++;
        count += n - m;
        i++;
    }

    lli* temp = (lli*)malloc((hi - lo + 1) * sizeof(lli));
    int k = 0, j = mid + 1;
    i = lo;
    while (i <= mid && j <= hi) {
        if (sums[i] < sums[j]) {
            temp[k++] = sums[i++];
        } else {
            temp[k++] = sums[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = sums[i++];
    }
    while (j <= hi) {
        temp[k++] = sums[j++];
    }
    for (i = lo, k = 0; i <= hi; ++i, ++k) {
        sums[i] = temp[k];
    }
    free(temp);
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
