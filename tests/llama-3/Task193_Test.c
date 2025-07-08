#include <stdio.h>
#include <stdbool.h>

bool xorGame(int* nums, int numsSize) {
    int xor_all = 0;
    int count = 0;
    for (int i = 0; i < numsSize; i++) {
        xor_all ^= nums[i];  // Calculate XOR of all elements
        if ((nums[i] & 1) == 0) {  // Check if the element is even
            count++;
        }
    }
    return xor_all == 0 || count % 2 == 1;  // Return true if XOR is 0 or count of even numbers is odd
}

int main() {
    // Test case 1: nums = {1, 1, 2}
    int nums1[] = {1, 1, 2};
    printf("%d\n", xorGame(nums1, 3));  // Expected output: 0 (false)

    // Test case 2: nums = {0, 1}
    int nums2[] = {0, 1};
    printf("%d\n", xorGame(nums2, 2));  // Expected output: 1 (true)

    // Test case 3: nums = {1, 2, 3}
    int nums3[] = {1, 2, 3};
    printf("%d\n", xorGame(nums3, 3));  // Expected output: 0 (false)

    return 0;
}
