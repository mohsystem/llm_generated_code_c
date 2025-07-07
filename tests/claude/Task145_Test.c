#include <stdio.h>
#include <limits.h>
#include <assert.h>

// دالة max بين رقمين
int max(int a, int b) {
    return (a > b) ? a : b;
}

// خوارزمية كادان لحساب أكبر مجموع فرعي متصل
int maxSubarraySum(int arr[], int n) {
    int maxSum = INT_MIN;
    int currentSum = 0;

    for (int i = 0; i < n; i++) {
        currentSum = max(arr[i], currentSum + arr[i]);
        maxSum = max(maxSum, currentSum);
    }

    return maxSum;
}

// دالة تساعد بتشغيل التست كيسز والتحقق منها
void run_tests() {
    // test_positive_numbers
    int arr1[] = {1, 2, 3, 4, 5};
    assert(maxSubarraySum(arr1, 5) == 15);

    // test_negative_numbers
    int arr2[] = {-1, -2, -3, -4, -5};
    assert(maxSubarraySum(arr2, 5) == -1);

    // test_mixed_numbers
    int arr3[] = {1, -2, 3, 4, -1, 2, 1, -5, 4};
    assert(maxSubarraySum(arr3, 9) == 9);

    // test_single_element
    int arr4[] = {42};
    assert(maxSubarraySum(arr4, 1) == 42);

    // test_all_zeroes
    int arr5[] = {0, 0, 0, 0, 0};
    assert(maxSubarraySum(arr5, 5) == 0);

    // test_large_array
    int arr6[1000];
    for (int i = 0; i < 1000; i++) arr6[i] = 1;
    assert(maxSubarraySum(arr6, 1000) == 1000);

    // test_alternating_signs
    int arr7[] = {3, -1, 4, -1, 2, 1, -5, 4};
    assert(maxSubarraySum(arr7, 8) == 8);

    // test_large_positive_and_negative_numbers
    int arr8[] = {10, -1, -1, 10, -1, 10, -1};
    assert(maxSubarraySum(arr8, 7) == 27);

    // test_subarray_at_the_end
    int arr9[] = {1, -2, -3, 4, 5};
    assert(maxSubarraySum(arr9, 5) == 9);

    // test_subarray_at_the_start
    int arr10[] = {6, -1, -2, 1, 2};
    assert(maxSubarraySum(arr10, 5) == 6);

    printf("✓ All tests passed for maxSubarraySum.\n");
}

int main() {
    run_tests();
    return 0;
}
