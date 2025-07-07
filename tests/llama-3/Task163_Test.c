#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int longestRun(int* nums, int numsSize) {
    if (numsSize == 0) {
        return 0;
    }
    qsort(nums, numsSize, sizeof(int), compare);
    int longest_run = 1;
    int current_run = 1;
    for (int i = 1; i < numsSize; i++) {
        if (nums[i] - nums[i-1] == 1) {
            current_run += 1;
        } else {
            longest_run = (longest_run > current_run) ? longest_run : current_run;
            current_run = 1;
        }
    }
    return (longest_run > current_run) ? longest_run : current_run;
}

int main() {
    int nums1[] = {1, 2, 3, 5, 6, 7, 8, 9};
    int nums2[] = {1, 2, 3, 10, 11, 15};
    int nums3[] = {5, 4, 2, 1};
    int nums4[] = {3, 5, 7, 10, 15};

    printf("%d\n", longestRun(nums1, sizeof(nums1) / sizeof(nums1[0]))); // Output: 5
    printf("%d\n", longestRun(nums2, sizeof(nums2) / sizeof(nums2[0]))); // Output: 3
    printf("%d\n", longestRun(nums3, sizeof(nums3) / sizeof(nums3[0]))); // Output: 2
    printf("%d\n", longestRun(nums4, sizeof(nums4) / sizeof(nums4[0]))); // Output: 1

    return 0;
}
