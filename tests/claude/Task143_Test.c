#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// دالة ترتيب الفقاعات
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n -1; i++) {
        for (int j = 0; j < n - i -1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// دالة مقارنة مصفوفتين بحجم n
int arrays_equal(int a[], int b[], int n) {
    for (int i=0; i<n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void run_tests() {
    int arr[10], expected[10];
    int n;

    // test_basic: "4 3 2 1" -> [1,2,3,4]
    int test1[] = {4,3,2,1};
    int exp1[] = {1,2,3,4};
    n = 4;
    memcpy(arr, test1, sizeof(test1));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp1, n));

    // test_single_element: "5" -> [5]
    int test2[] = {5};
    int exp2[] = {5};
    n = 1;
    memcpy(arr, test2, sizeof(test2));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp2, n));

    // test_already_sorted: "1 2 3 4 5" -> same
    int test3[] = {1,2,3,4,5};
    int exp3[] = {1,2,3,4,5};
    n = 5;
    memcpy(arr, test3, sizeof(test3));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp3, n));

    // test_all_elements_same: "7 7 7 7"
    int test4[] = {7,7,7,7};
    int exp4[] = {7,7,7,7};
    n = 4;
    memcpy(arr, test4, sizeof(test4));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp4, n));

    // test_negative_numbers: "-1 -3 -2 -4" -> [-4,-3,-2,-1]
    int test5[] = {-1,-3,-2,-4};
    int exp5[] = {-4,-3,-2,-1};
    n = 4;
    memcpy(arr, test5, sizeof(test5));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp5, n));

    // test_mixed_positive_and_negative: "3 -1 0 2 -5"
    int test6[] = {3,-1,0,2,-5};
    int exp6[] = {-5,-1,0,2,3};
    n = 5;
    memcpy(arr, test6, sizeof(test6));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp6, n));

    // test_large_numbers: "1000 500 2000 1500"
    int test7[] = {1000, 500, 2000, 1500};
    int exp7[] = {500, 1000, 1500, 2000};
    n = 4;
    memcpy(arr, test7, sizeof(test7));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp7, n));

    // test_float_numbers: "3 2 4 1" -> same as integers (float ignored)
    int test8[] = {3,2,4,1};
    int exp8[] = {1,2,3,4};
    n = 4;
    memcpy(arr, test8, sizeof(test8));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp8, n));

    // test_mixed_integers_and_floats: "1 2 1 2 0"
    int test9[] = {1,2,1,2,0};
    int exp9[] = {0,1,1,2,2};
    n = 5;
    memcpy(arr, test9, sizeof(test9));
    bubbleSort(arr, n);
    assert(arrays_equal(arr, exp9, n));

    // test_empty_input: "" -> empty array
    n = 0; // empty
    bubbleSort(arr, n); // should not fail

    printf("✓ All tests passed for bubbleSort.\n");
}

int main() {
    run_tests();
    return 0;
}
