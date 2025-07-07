#include <stdio.h>

// تعريف الدالة كما في كودك
double findMedianSortedArrays(int* nums1, int m, int* nums2, int n) {
    int* nums = malloc((m + n) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < m && j < n) {
        if (nums1[i] < nums2[j]) {
            nums[k++] = nums1[i++];
        } else {
            nums[k++] = nums2[j++];
        }
    }
    while (i < m) {
        nums[k++] = nums1[i++];
    }
    while (j < n) {
        nums[k++] = nums2[j++];
    }

    double median;
    if ((m + n) % 2 == 0) {
        median = (nums[(m + n) / 2] + nums[(m + n) / 2 - 1]) / 2.0;
    } else {
        median = nums[(m + n) / 2];
    }

    free(nums);
    return median;
}

int main() {
    // Test case 1
    int nums1_1[] = {1, 3};
    int nums2_1[] = {2};
    double res1 = findMedianSortedArrays(nums1_1, 2, nums2_1, 1);
    printf("Test Case 1: Expected 2.0, Got %.1f\n", res1);

    // Test case 2
    int nums1_2[] = {1, 2};
    int nums2_2[] = {3, 4};
    double res2 = findMedianSortedArrays(nums1_2, 2, nums2_2, 2);
    printf("Test Case 2: Expected 2.5, Got %.1f\n", res2);

    return 0;
}
