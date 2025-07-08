#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int* merged = (int*)malloc((nums1Size + nums2Size) * sizeof(int));
    for (int i = 0; i < nums1Size; i++) {
        merged[i] = nums1[i];
    }
    for (int i = 0; i < nums2Size; i++) {
        merged[nums1Size + i] = nums2[i];
    }
    qsort(merged, nums1Size + nums2Size, sizeof(int), compare);
    int length = nums1Size + nums2Size;
    double median;
    if (length % 2 == 0) {
        median = (merged[length / 2 - 1] + merged[length / 2]) / 2.0;
    } else {
        median = merged[length / 2];
    }
    free(merged);
    return median;
}

int main() {
    int nums1_1[] = {1, 3};
    int nums2_1[] = {2};
    printf("Test 1 Output: %lf\n", findMedianSortedArrays(nums1_1, 2, nums2_1, 1)); // 2.0

    int nums1_2[] = {1, 2};
    int nums2_2[] = {3, 4};
    printf("Test 2 Output: %lf\n", findMedianSortedArrays(nums1_2, 2, nums2_2, 2)); // 2.5

    // Constraints cases and edge cases can be added here as well if needed

    return 0;
}
