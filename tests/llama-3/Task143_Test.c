#include <stdio.h>
#include <assert.h>

void sort_array(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
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
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int expected[] = {11, 12, 22, 25, 34, 64, 90};
    int n = sizeof(arr)/sizeof(arr[0]);

    sort_array(arr, n);
    assert(arrays_equal(arr, expected, n));

    // اختبار مصفوفة فارغة
    int empty[] = {};
    sort_array(empty, 0);
    assert(1); // فقط لتأكيد عدم حدوث خطأ

    // اختبار عنصر واحد
    int single[] = {42};
    sort_array(single, 1);
    assert(single[0] == 42);

    printf("✓ All tests passed for sort_array.\n");
}

int main() {
    run_tests();
    return 0;
}
