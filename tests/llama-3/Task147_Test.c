#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// دالة مقارنة لترتيب تنازلي
int compare(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int findKthLargest(int* nums, int numsSize, int k) {
    qsort(nums, numsSize, sizeof(int), compare);
    return nums[k-1];
}

void run_tests() {
    int nums1[] = {3, 2, 1, 5, 6, 4};
    assert(findKthLargest(nums1, 6, 2) == 5);

    int nums2[] = {7, 10, 4, 3, 20, 15};
    assert(findKthLargest(nums2, 6, 3) == 10);

    int nums3[] = {1, 1, 1, 1};
    assert(findKthLargest(nums3, 4, 2) == 1);

    int nums4[] = {-1, -3, -2, -4, -5};
    assert(findKthLargest(nums4, 5, 1) == -1);

    int nums5[] = {-10, -50, 20, 10, 30, 0};
    assert(findKthLargest(nums5, 6, 4) == 0);

    int nums6[] = {100};
    assert(findKthLargest(nums6, 1, 1) == 100);

    int nums7[] = {1, 23, 12, 9, 30, 2, 50};
    assert(findKthLargest(nums7, 7, 7) == 1);

    int nums8[] = {1, 2, 3, 4, 5};
    assert(findKthLargest(nums8, 5, 1) == 5);

    int nums9[] = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    assert(findKthLargest(nums9, 9, 4) == 4);

    int nums10[] = {3, 6, 1, 0, 10, 7, 8, 2, 4, 5, 9};
    assert(findKthLargest(nums10, 11, 10) == 1);

    printf("✓ All tests passed for findKthLargest.\n");
}

int main() {
    run_tests();
    return 0;
}
