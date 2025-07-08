#include <stdio.h>
#include <assert.h>

void mergeAndSort(int arr1[], int size1, int arr2[], int size2, int merged[]) {
    int i, j, k;
    for (i = 0; i < size1; i++) {
        merged[i] = arr1[i];
    }
    for (j = 0; j < size2; j++) {
        merged[i + j] = arr2[j];
    }

    int total = size1 + size2;
    for (i = 0; i < total; i++) {
        for (j = i + 1; j < total; j++) {
            if (merged[i] > merged[j]) {
                int temp = merged[i];
                merged[i] = merged[j];
                merged[j] = temp;
            }
        }
    }
}

int arrays_equal(int a[], int b[], int n) {
    for (int i=0; i<n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int merged[20];

    // Test 1
    int arr1_1[] = {1, 3, 5, 7};
    int arr2_1[] = {2, 4, 6, 8};
    int expected1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    mergeAndSort(arr1_1, 4, arr2_1, 4, merged);
    assert(arrays_equal(merged, expected1, 8));

    // Test 2: one empty array
    int arr1_2[] = {};
    int arr2_2[] = {1, 2, 3};
    mergeAndSort(arr1_2, 0, arr2_2, 3, merged);
    int expected2[] = {1, 2, 3};
    assert(arrays_equal(merged, expected2, 3));

    // Test 3: both empty
    int arr1_3[] = {};
    int arr2_3[] = {};
    mergeAndSort(arr1_3, 0, arr2_3, 0, merged);
    // No elements, so nothing to compare but ensure no crash

    printf("✓ All tests passed for mergeAndSort.\n");
}

int main() {
    run_tests();
    return 0;
}
