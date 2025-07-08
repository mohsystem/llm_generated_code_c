#include <stdio.h>
#include <assert.h>

void mergeArrays(int arr1[], int n1, int arr2[], int n2, int merged[]) {
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            merged[k++] = arr1[i++];
        } else {
            merged[k++] = arr2[j++];
        }
    }
    while (i < n1) {
        merged[k++] = arr1[i++];
    }
    while (j < n2) {
        merged[k++] = arr2[j++];
    }
}

int arrays_equal(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int merged[20];
    int expected[20];

    // Test 1
    int arr1_1[] = {1, 3, 5, 7};
    int arr2_1[] = {2, 4, 6, 8};
    int n1 = sizeof(arr1_1) / sizeof(arr1_1[0]);
    int n2 = sizeof(arr2_1) / sizeof(arr2_1[0]);
    int expected1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    mergeArrays(arr1_1, n1, arr2_1, n2, merged);
    assert(arrays_equal(merged, expected1, n1 + n2));

    // Test 2: one empty array
    int arr1_2[] = {};
    int arr2_2[] = {1, 2, 3};
    int n3 = 0;
    int n4 = sizeof(arr2_2) / sizeof(arr2_2[0]);
    int expected2[] = {1, 2, 3};
    mergeArrays(arr1_2, n3, arr2_2, n4, merged);
    assert(arrays_equal(merged, expected2, n3 + n4));

    // Test 3: both empty
    int arr1_3[] = {};
    int arr2_3[] = {};
    int n5 = 0, n6 = 0;
    mergeArrays(arr1_3, n5, arr2_3, n6, merged);
    assert(n5 + n6 == 0); // just ensure no crash

    printf("✓ All tests passed for mergeArrays.\n");
}

int main() {
    run_tests();
    return 0;
}
