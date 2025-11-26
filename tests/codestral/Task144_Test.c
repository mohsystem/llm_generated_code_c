#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int* mergeSortedArrays(int* arr1, int size1, int* arr2, int size2, int* mergedSize) {
    int* merged = (int*)malloc((size1 + size2) * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {
        if (arr1[i] < arr2[j]) {
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

    *mergedSize = k;
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

    // empty arrays
    int a0[] = {};
    int b0[] = {};
    result = mergeSortedArrays(a0, 0, b0, 0, &size);
    assert(size == 0);
    free(result);

    // one empty array
    int a1[] = {};
    int b1[] = {1, 3, 5};
    int exp1[] = {1, 3, 5};
    result = mergeSortedArrays(a1, 0, b1, 3, &size);
    assert(size == 3 && arrays_equal(result, exp1, size));
    free(result);

    int a2[] = {2, 4, 6};
    int b2[] = {};
    int exp2[] = {2, 4, 6};
    result = mergeSortedArrays(a2, 3, b2, 0, &size);
    assert(size == 3 && arrays_equal(result, exp2, size));
    free(result);

    // single element arrays
    int a3[] = {1};
    int b3[] = {1};
    int exp3[] = {1,1};
    result = mergeSortedArrays(a3, 1, b3, 1, &size);
    assert(size == 2 && arrays_equal(result, exp3, size));
    free(result);

    int a4[] = {1};
    int b4[] = {2};
    int exp4[] = {1,2};
    result = mergeSortedArrays(a4, 1, b4, 1, &size);
    assert(size == 2 && arrays_equal(result, exp4, size));
    free(result);

    // different sizes all smaller elements
    int a5[] = {1,2,3};
    int b5[] = {4,5,6};
    int exp5[] = {1,2,3,4,5,6};
    result = mergeSortedArrays(a5, 3, b5, 3, &size);
    assert(size == 6 && arrays_equal(result, exp5, size));
    free(result);

    // overlapping elements
    int a6[] = {1,3,5};
    int b6[] = {2,3,4};
    int exp6[] = {1,2,3,3,4,5};
    result = mergeSortedArrays(a6, 3, b6, 3, &size);
    assert(size == 6 && arrays_equal(result, exp6, size));
    free(result);

    // all elements same
    int a7[] = {2,2,2};
    int b7[] = {2,2,2};
    int exp7[] = {2,2,2,2,2,2};
    result = mergeSortedArrays(a7, 3, b7, 3, &size);
    assert(size == 6 && arrays_equal(result, exp7, size));
    free(result);

    // interleaving elements
    int a8[] = {1,3,5,7};
    int b8[] = {2,4,6,8};
    int exp8[] = {1,2,3,4,5,6,7,8};
    result = mergeSortedArrays(a8, 4, b8, 4, &size);
    assert(size == 8 && arrays_equal(result, exp8, size));
    free(result);

    // large element diff
    int a9[] = {1,2};
    int b9[] = {1000,2000,3000};
    int exp9[] = {1,2,1000,2000,3000};
    result = mergeSortedArrays(a9, 2, b9, 3, &size);
    assert(size == 5 && arrays_equal(result, exp9, size));
    free(result);

    printf("✓ All tests passed for mergeSortedArrays.\n");
}

int main() {
    run_tests();
    return 0;
}
