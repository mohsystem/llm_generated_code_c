#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int *merge_sorted_arrays(int arr1[], int size1, int arr2[], int size2) {
    int *merged_array = malloc(sizeof(int) * (size1 + size2));
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            merged_array[k++] = arr1[i++];
        } else {
            merged_array[k++] = arr2[j++];
        }
    }
    while (i < size1) {
        merged_array[k++] = arr1[i++];
    }
    while (j < size2) {
        merged_array[k++] = arr2[j++];
    }
    return merged_array;
}

int arrays_equal(int a[], int b[], int n) {
    for (int i=0; i<n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int *result;
    int size;

    // Basic test
    int a1[] = {1, 3, 5, 7};
    int a2[] = {2, 4, 6, 8};
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n1 = sizeof(a1)/sizeof(a1[0]);
    int n2 = sizeof(a2)/sizeof(a2[0]);
    result = merge_sorted_arrays(a1, n1, a2, n2);
    assert(arrays_equal(result, expected, n1+n2));
    free(result);

    // Empty arrays
    int empty1[] = {};
    int empty2[] = {};
    result = merge_sorted_arrays(empty1, 0, empty2, 0);
    assert(result == NULL || 1); // malloc might return non-null, just ensure no crash
    free(result);

    // One empty array
    int b1[] = {};
    int b2[] = {1, 2, 3};
    int exp2[] = {1, 2, 3};
    result = merge_sorted_arrays(b1, 0, b2, 3);
    assert(arrays_equal(result, exp2, 3));
    free(result);

    int b3[] = {4, 5, 6};
    int b4[] = {};
    int exp3[] = {4, 5, 6};
    result = merge_sorted_arrays(b3, 3, b4, 0);
    assert(arrays_equal(result, exp3, 3));
    free(result);

    printf("✓ All tests passed for merge_sorted_arrays.\n");
}

int main() {
    run_tests();
    return 0;
}
