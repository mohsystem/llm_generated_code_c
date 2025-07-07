#include <stdlib.h>
#include <stdio.h>

int compare(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    else if (int_a > int_b) return 1;
    else return 0;
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int totalSize = nums1Size + nums2Size;
    int* nums = (int*)malloc(totalSize * sizeof(int));
    if (!nums) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < nums1Size; i++) {
        nums[i] = nums1[i];
    }
    for (int i = 0; i < nums2Size; i++) {
        nums[nums1Size + i] = nums2[i];
    }
    // استخدام الدالة compare صحيحة
    qsort(nums, totalSize, sizeof(int), compare);

    double median;
    if (totalSize % 2 == 0) {
        median = (nums[totalSize/2] + nums[totalSize/2 - 1]) / 2.0;
    } else {
        median = nums[totalSize/2];
    }
    free(nums);
    return median;
}

int main() {
    // Test case 1
    int nums1_case1[] = {1, 3};
    int nums2_case1[] = {2};
    double expected1 = 2.0;
    double res1 = findMedianSortedArrays(nums1_case1, 2, nums2_case1, 1);
    if (res1 == expected1) printf("Test Case 1 Passed\n");
    else printf("Test Case 1 Failed: got %f expected %f\n", res1, expected1);

    // Test case 2
    int nums1_case2[] = {1, 2};
    int nums2_case2[] = {3, 4};
    double expected2 = 2.5;
    double res2 = findMedianSortedArrays(nums1_case2, 2, nums2_case2, 2);
    if (res2 == expected2) printf("Test Case 2 Passed\n");
    else printf("Test Case 2 Failed: got %f expected %f\n", res2, expected2);

    return 0;
}
