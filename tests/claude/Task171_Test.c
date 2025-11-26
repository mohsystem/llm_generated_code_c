#include <stdio.h>
#include <stdlib.h>

int merge(int* nums, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int count = 0;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = nums[left + i];
    for (j = 0; j < n2; j++)
        R[j] = nums[mid + 1 + j];

    j = 0;
    for (i = 0; i < n1; i++) {
        while (j < n2 && (long long)L[i] > 2LL * R[j]) {
            j++;
        }
        count += j;
    }

    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            nums[k++] = L[i++];
        } else {
            nums[k++] = R[j++];
        }
    }
    while (i < n1) nums[k++] = L[i++];
    while (j < n2) nums[k++] = R[j++];

    free(L);
    free(R);

    return count;
}

int mergeSort(int* nums, int left, int right) {
    int count = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        count += mergeSort(nums, left, mid);
        count += mergeSort(nums, mid + 1, right);
        count += merge(nums, left, mid, right);
    }
    return count;
}

int reversePairs(int* nums, int numsSize) {
    return mergeSort(nums, 0, numsSize - 1);
}

void runTest(int* nums, int numsSize, int expected, const char* testName) {
    // نسخ المصفوفة لان mergeSort يعدلها
    int* copy = (int*)malloc(numsSize * sizeof(int));
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
