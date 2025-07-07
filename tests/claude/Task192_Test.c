#include <stdio.h>
#include <stdbool.h>

bool xorGame(int* nums, int numsSize) {
    int xorSum = 0;
    for (int i = 0; i < numsSize; i++) {
        xorSum ^= nums[i];
    }
    return xorSum == 0 || numsSize % 2 == 0;
}

int main() {
    // Test case 1
    int nums1[] = {1, 1, 2};
    printf("%d\n", xorGame(nums1, 3));  // Output: 0 (false)

    // Test case 2
    int nums2[] = {0, 1};
    printf("%d\n", xorGame(nums2, 2));  // Output: 1 (true)

    // Test case 3
    int nums3[] = {1, 2, 3};
    printf("%d\n", xorGame(nums3, 3));  // Output: 1 (true)

    return 0;
}
