#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    if (nums1Size > nums2Size) {
        return findMedianSortedArrays(nums2, nums2Size, nums1, nums1Size);
    }

    int m = nums1Size;
    int n = nums2Size;
    int low = 0, high = m;

    while (low <= high) {
        int partitionX = (low + high) / 2;
        int partitionY = (m + n + 1) / 2 - partitionX;

        int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
        int minRightX = (partitionX == m) ? INT_MAX : nums1[partitionX];

        int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
        int minRightY = (partitionY == n) ? INT_MAX : nums2[partitionY];

        if (maxLeftX <= minRightY && maxLeftY <= minRightX) {
            if ((m + n) % 2 == 0) {
                return (MAX(maxLeftX, maxLeftY) + MIN(minRightX, minRightY)) / 2.0;
            } else {
                return (double)MAX(maxLeftX, maxLeftY);
            }
        } else if (maxLeftX > minRightY) {
            high = partitionX - 1;
        } else {
            low = partitionX + 1;
        }
    }

    fprintf(stderr, "Input arrays are not sorted\n");
    exit(1);
}

// Same test cases from your Python code:
int main() {
    int nums1_case1[] = {1, 3};
    int nums2_case1[] = {2};
    double expected1 = 2.0;

    int nums1_case2[] = {1, 2};
    int nums2_case2[] = {3, 4};
    double expected2 = 2.5;

    // Test case 1
    double res1 = findMedianSortedArrays(nums1_case1, 2, nums2_case1, 1);
    if (res1 == expected1) {
        printf("Test Case 1 Passed\n");
    } else {
        printf("Test Case 1 Failed: got %f expected %f\n", res1, expected1);
    }

    // Test case 2
    double res2 = findMedianSortedArrays(nums1_case2, 2, nums2_case2, 2);
    if (res2 == expected2) {
        printf("Test Case 2 Passed\n");
    } else {
        printf("Test Case 2 Failed: got %f expected %f\n", res2, expected2);
    }

    return 0;
}
