#include <stdio.h>
#include <assert.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int arrays_equal(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int expected[] = {11, 12, 22, 25, 34, 64, 90};
    int n = sizeof(arr)/sizeof(arr[0]);

    bubbleSort(arr, n);
    assert(arrays_equal(arr, expected, n));

    // اختبار مصفوفة فارغة
    int empty[] = {};
    bubbleSort(empty, 0);
    assert(1); // فقط تأكيد لا يحصل خطأ

    // اختبار مصفوفة عنصر واحد
    int single[] = {42};
    bubbleSort(single, 1);
    assert(single[0] == 42);

    printf("✓ All tests passed for bubbleSort.\n");
}

int main() {
    run_tests();
    return 0;
}
