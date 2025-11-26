#include <stdio.h>
#include <assert.h>

// دالة maxSubArraySum باستخدام خوارزمية كادان
int maxSubArraySum(int arr[], int size) {
    int maxCurrent = arr[0];
    int maxGlobal = arr[0];

    for (int i = 1; i < size; i++) {
        maxCurrent = arr[i] > (maxCurrent + arr[i]) ? arr[i] : (maxCurrent + arr[i]);
        if (maxCurrent > maxGlobal) {
            maxGlobal = maxCurrent;
        }
    }
    return maxGlobal;
}

void run_tests() {
    // test_positive_numbers
    int arr1[] = {1, 2, 3, 4, 5};
    assert(maxSubArraySum(arr1, 5) == 15);

    // test_negative_numbers
    int arr2[] = {-1, -2, -3, -4, -5};
    assert(maxSubArraySum(arr2, 5) == -1);

    // test_mixed_numbers
    int arr3[] = {1, -2, 3, 4, -1, 2, 1, -5, 4};
    assert(maxSubArraySum(arr3, 9) == 9);

    // test_single_element
    int arr4[] = {42};
    assert(maxSubArraySum(arr4, 1) == 42);

    // test_all_zeroes
    int arr5[] = {0, 0, 0, 0, 0};
    assert(maxSubArraySum(arr5, 5) == 0);

    // test_large_array
    int arr6[1000];
    for (int i = 0; i < 1000; i++) arr6[i] = 1;
    assert(maxSubArraySum(arr6, 1000) == 1000);

    // test_alternating_signs
    int arr7[] = {3, -1, 4, -1, 2, 1, -5, 4};
    assert(maxSubArraySum(arr7, 8) == 8);

    // test_large_positive_and_negative_numbers
    int arr8[] = {10, -1, -1, 10, -1, 10, -1};
    assert(maxSubArraySum(arr8, 7) == 27);

    // test_subarray_at_the_end
    int arr9[] = {1, -2, -3, 4, 5};
    assert(maxSubArraySum(arr9, 5) == 9);

    // test_subarray_at_the_start
    int arr10[] = {6, -1, -2, 1, 2};
    assert(maxSubArraySum(arr10, 5) == 6);

    printf("✓ All tests passed for maxSubArraySum.\n");
}

int main() {
    run_tests();
    return 0;
}
