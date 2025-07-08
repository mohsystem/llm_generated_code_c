#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int arrays_equal(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int inputArray[] = {5, 2, 9, 1, 5, 6};
    int expected[] = {1, 2, 5, 5, 6, 9};
    int n = sizeof(inputArray) / sizeof(inputArray[0]);

    qsort(inputArray, n, sizeof(int), cmpfunc);
    assert(arrays_equal(inputArray, expected, n));

    // اختبار مصفوفة فارغة
    n = 0;
    qsort(inputArray, n, sizeof(int), cmpfunc);
    assert(n == 0);

    // اختبار مصفوفة عنصر واحد
    int single[] = {42};
    int expected_single[] = {42};
    n = 1;
    qsort(single, n, sizeof(int), cmpfunc);
    assert(arrays_equal(single, expected_single, n));

    printf("✓ All tests passed for qsort with cmpfunc.\n");
}

int main() {
    run_tests();
    return 0;
}
