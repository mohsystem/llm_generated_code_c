#include <stdio.h>
#include <limits.h>
#include <assert.h>

// دالة لحساب أكبر مجموع فرعي متصل، مع تتبع حدود المصفوفة
int maxSubArraySum(int arr[], int n, int* start, int* end) {
    int max_so_far = INT_MIN;
    int max_ending_here = 0;
    int s = 0;

    *start = 0;
    *end = 0;

    for (int i = 0; i < n; i++) {
        max_ending_here += arr[i];

        if (max_so_far < max_ending_here) {
            max_so_far = max_ending_here;
            *start = s;
            *end = i;
        }

        if (max_ending_here < 0) {
            max_ending_here = 0;
            s = i + 1;
        }
    }
    return max_so_far;
}

int arrays_equal(int a[], int b[], int n) {
    for (int i=0; i<n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int start, end;
    int arr1[] = {1, 2, 3, 4, 5};
    int n1 = 5;
    int max_sum = maxSubArraySum(arr1, n1, &start, &end);
    assert(max_sum == 15);
    int expected1[] = {1,2,3,4,5};
    assert(arrays_equal(&arr1[start], expected1, end - start + 1));

    int arr2[] = {-1, -2, -3, -4, -5};
    int n2 = 5;
    max_sum = maxSubArraySum(arr2, n2, &start, &end);
    assert(max_sum == -1);
    int expected2[] = {-1};
    assert(arrays_equal(&arr2[start], expected2, end - start + 1));

    int arr3[] = {1, -2, 3, 4, -1, 2, 1, -5, 4};
    int n3 = 9;
    max_sum = maxSubArraySum(arr3, n3, &start, &end);
    assert(max_sum == 9);
    int expected3[] = {3, 4, -1, 2, 1};
    assert(arrays_equal(&arr3[start], expected3, end - start + 1));

    int arr4[] = {42};
    int n4 = 1;
    max_sum = maxSubArraySum(arr4, n4, &start, &end);
    assert(max_sum == 42);
    int expected4[] = {42};
    assert(arrays_equal(&arr4[start], expected4, end - start + 1));

    int arr5[] = {0, 0, 0, 0, 0};
    int n5 = 5;
    max_sum = maxSubArraySum(arr5, n5, &start, &end);
    assert(max_sum == 0);
    int expected5[] = {0, 0, 0, 0, 0};
    assert(arrays_equal(&arr5[start], expected5, end - start + 1));

    int arr6[1000];
    for (int i=0; i<1000; i++) arr6[i] = 1;
    max_sum = maxSubArraySum(arr6, 1000, &start, &end);
    assert(max_sum == 1000);
    int expected6[1000];
    for (int i=0; i<1000; i++) expected6[i] = 1;
    assert(arrays_equal(&arr6[start], expected6, end - start + 1));

    int arr7[] = {3, -1, 4, -1, 2, 1, -5, 4};
    int n7 = 8;
    max_sum = maxSubArraySum(arr7, n7, &start, &end);
    assert(max_sum == 8);
    int expected7[] = {3, -1, 4, -1, 2, 1};
    assert(arrays_equal(&arr7[start], expected7, end - start + 1));

    int arr8[] = {10, -1, -1, 10, -1, 10, -1};
    int n8 = 7;
    max_sum = maxSubArraySum(arr8, n8, &start, &end);
    assert(max_sum == 27);
    int expected8[] = {10, -1, -1, 10, -1, 10};
    assert(arrays_equal(&arr8[start], expected8, end - start + 1));

    int arr9[] = {1, -2, -3, 4, 5};
    int n9 = 5;
    max_sum = maxSubArraySum(arr9, n9, &start, &end);
    assert(max_sum == 9);
    int expected9[] = {4, 5};
    assert(arrays_equal(&arr9[start], expected9, end - start + 1));

    int arr10[] = {6, -1, -2, 1, 2};
    int n10 = 5;
    max_sum = maxSubArraySum(arr10, n10, &start, &end);
    assert(max_sum == 6);
    int expected10[] = {6};
    assert(arrays_equal(&arr10[start], expected10, end - start + 1));

    printf("✓ All tests passed for maxSubArraySum with subarray.\n");
}

int main() {
    run_tests();
    return 0;
}
