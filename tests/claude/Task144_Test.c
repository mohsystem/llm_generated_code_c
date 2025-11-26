#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int* mergeSortedArrays(int* arr1, int size1, int* arr2, int size2, int* resultSize) {
    int* merged = (int*)malloc((size1 + size2) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            merged[k++] = arr1[i++];
        } else {
            merged[k++] = arr2[j++];
        }
    }
    while (i < size1) {
        merged[k++] = arr1[i++];
    }
    while (j < size2) {
        merged[k++] = arr2[j++];
    }
    *resultSize = k;
    return merged;
}

int arrays_equal(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int *result;
    int size;

    // test_empty_arrays
    int arr1_0[] = {};
    int arr2_0[] = {};
    result = mergeSortedArrays(arr1_0, 0, arr2_0, 0, &size);
    assert(size == 0);
    free(result);

    // test_one_empty_array
    int arr1_1[] = {};
    int arr2_1[] = {1, 3, 5};
    int expected1[] = {1, 3, 5};
    result = mergeSortedArrays(arr1_1, 0, arr2_1, 3, &size);
    assert(size == 3 && arrays_equal(result, expected1, size));
    free(result);

    int arr1_2[] = {2, 4, 6};
    int arr2_2[] = {};
    int expected2[] = {2, 4, 6};
    result = mergeSortedArrays(arr1_2, 3, arr2_2, 0, &size);
    assert(size == 3 && arrays_equal(result, expected2, size));
    free(result);

    // test_single_element_arrays
    int arr1_3[] = {1};
    int arr2_3[] = {1};
    int expected3[] = {1, 1};
    result = mergeSortedArrays(arr1_3, 1, arr2_3, 1, &size);
    assert(size == 2 && arrays_equal(result, expected3, size));
    free(result);

    int arr1_4[] = {1};
    int arr2_4[] = {2};
    int expected4[] = {1, 2};
    result = mergeSortedArrays(arr1_4, 1, arr2_4, 1, &size);
    assert(size == 2 && arrays_equal(result, expected4, size));
    free(result);

    // test_different_sizes_with_all_smaller_elements
    int arr1_5[] = {1, 2, 3};
    int arr2_5[] = {4, 5, 6};
    int expected5[] = {1, 2, 3, 4, 5, 6};
    result = mergeSortedArrays(arr1_5, 3, arr2_5, 3, &size);
    assert(size == 6 && arrays_equal(result, expected5, size));
    free(result);

    // test_some_overlapping_elements
    int arr1_6[] = {1, 3, 5};
    int arr2_6[] = {2, 3, 4};
    int expected6[] = {1, 2, 3, 3, 4, 5};
    result = mergeSortedArrays(arr1_6, 3, arr2_6, 3, &size);
    assert(size == 6 && arrays_equal(result, expected6, size));
    free(result);

    // test_all_elements_same
    int arr1_7[] = {2, 2, 2};
    int arr2_7[] = {2, 2, 2};
    int expected7[] = {2, 2, 2, 2, 2, 2};
    result = mergeSortedArrays(arr1_7, 3, arr2_7, 3, &size);
    assert(size == 6 && arrays_equal(result, expected7, size));
    free(result);

    // test_interleaving_elements
    int arr1_8[] = {1, 3, 5, 7};
    int arr2_8[] = {2, 4, 6, 8};
    int expected8[] = {1, 2, 3, 4, 5, 6, 7, 8};
    result = mergeSortedArrays(arr1_8, 4, arr2_8, 4, &size);
    assert(size == 8 && arrays_equal(result, expected8, size));
    free(result);

    // test_large_element_diff
    int arr1_9[] = {1, 2};
    int arr2_9[] = {1000, 2000, 3000};
    int expected9[] = {1, 2, 1000, 2000, 3000};
    result = mergeSortedArrays(arr1_9, 2, arr2_9, 3, &size);
    assert(size == 5 && arrays_equal(result, expected9, size));
    free(result);

    // test_reverse_order_arrays (sort both first)
    int arr1_10[] = {9, 7, 5};
    int arr2_10[] = {8, 6, 4};
    // sort arr1_10 and arr2_10 before merging
    int temp;
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (arr1_10[i] > arr1_10[j]) {
                temp = arr1_10[i];
                arr1_10[i] = arr1_10[j];
                arr1_10[j] = temp;
            }
            if (arr2_10[i] > arr2_10[j]) {
                temp = arr2_10[i];
                arr2_10[i] = arr2_10[j];
                arr2_10[j] = temp;
            }
        }
    }
    int expected10[] = {4,5,6,7,8,9};
    result = mergeSortedArrays(arr1_10, 3, arr2_10, 3, &size);
    assert(size == 6 && arrays_equal(result, expected10, size));
    free(result);

    printf("✓ All tests passed for mergeSortedArrays.\n");
}

int main() {
    run_tests();
    return 0;
}
