#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int arrays_equal(int a[], int b[], int n) {
    for (int i=0; i<n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int arr[10], expected[10];
    int n;

    int test1[] = {5, 2, 8, 1, 9};
    int exp1[] = {1, 2, 5, 8, 9};
    n = 5;

    for (int i=0; i<n; i++) {
        arr[i] = test1[i];
    }

    qsort(arr, n, sizeof(int), compare);
    assert(arrays_equal(arr, exp1, n));

    // اختبار مصفوفة فارغة
    n = 0;
    qsort(arr, n, sizeof(int), compare);
    assert(n == 0); // فقط تأكد أن الدالة لا تتسبب بخطأ

    // اختبار مصفوفة ذات عنصر واحد
    int test2[] = {42};
    int exp2[] = {42};
    n = 1;
    arr[0] = test2[0];
    qsort(arr, n, sizeof(int), compare);
    assert(arrays_equal(arr, exp2, n));

    printf("✓ All tests passed for qsort.\n");
}

int main() {
    run_tests();
    return 0;
}
