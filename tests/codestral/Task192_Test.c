#include <stdio.h>
#include <stdbool.h>

bool xorGame(int* nums, int numsSize) {
    int xor = 0;
    for (int i = 0; i < numsSize; i++) {
        xor ^= nums[i];
    }

    if (xor == 0) {
        return false;  // If XOR is 0, Alice loses
    } else if (numsSize % 2 == 0) {
        return false;  // If the array size is even, Alice loses
    } else {
        return true;  // If XOR is not 0 and the size is odd, Alice wins
    }
}

int main() {
    // Test case 1
    int nums1[] = {1, 1, 2};
    printf("%d\n", xorGame(nums1, 3));  // Output: 0 (false)

    // Test case 2
    int nums2[] = {0, 1};
    printf("%d\n", xorGame(nums2, 2));  // Output: 0 (false)

    // Test case 3
    int nums3[] = {1, 2, 3};
    printf("%d\n", xorGame(nums3, 3));  // Output: 1 (true)

    return 0;
}
