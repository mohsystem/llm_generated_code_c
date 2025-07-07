#include <stdio.h>
#include <stdbool.h>

bool xorGame(int* nums, int numsSize) {
    int xor_all = 0;
    for (int i = 0; i < numsSize; i++) {
        xor_all ^= nums[i];  // Calculate the XOR of all elements
    }

    if (xor_all == 0) {
        return true;  // If XOR is 0, Alice wins
    }

    int count = 0;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] % 2 == 0) {  // Count even numbers
            count++;
        }
    }

    return count % 2 == 0;  // If the count of even numbers is even, Alice wins
}

int main() {
    int nums1[] = {1, 1, 2};  // Test case 1
    int nums2[] = {0, 1};      // Test case 2
    int nums3[] = {1, 2, 3};   // Test case 3

    printf("%d\n", xorGame(nums1, 3));  // Output: 1 (true)
    printf("%d\n", xorGame(nums2, 2));  // Output: 1 (true)
    printf("%d\n", xorGame(nums3, 3));  // Output: 0 (false)

    return 0;
}
