#include <stdlib.h>
#include <stdio.h>
//can't fix,    // error: ‘compare’ undeclared (first use in this function). just replace with null
double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int* nums = (int*)malloc((nums1Size + nums2Size) * sizeof(int));
    for (int i = 0; i < nums1Size; i++) {
        nums[i] = nums1[i];
    }
    for (int i = 0; i < nums2Size; i++) {
        nums[nums1Size + i] = nums2[i];
    }
    // error: ‘compare’ undeclared (first use in this function)
    // qsort(nums, nums1Size + nums2Size, sizeof(int), compare);
    qsort(nums, nums1Size + nums2Size, sizeof(int), NULL);
    int n = nums1Size + nums2Size;
    double median;
    if (n % 2 == 0) {
        median = (nums[n/2] + nums[n/2 - 1]) / 2.0;
    } else {
        median = nums[n/2];
    }
    free(nums);
    return median;
}

int compare(const void* a, const void* b) {
    if (*(int*)a < *(int*)b) {
        return -1;
    } else if (*(int*)a > *(int*)b) {
        return 1;
    } else {
        return 0;
    }
}
