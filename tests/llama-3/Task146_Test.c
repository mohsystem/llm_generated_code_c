#include <stdio.h>
#include <assert.h>

int find_missing_number(int nums[], int size) {
    int n = size + 1;
    int expected_sum = n * (n + 1) / 2;
    int actual_sum = 0;
    for (int i = 0; i < size; i++) {
        actual_sum += nums[i];
    }
    return expected_sum - actual_sum;
}

void run_tests() {
    int arr1[] = {2, 3, 4, 5, 6};
    assert(find_missing_number(arr1, 5) == 1);

    int arr2[] = {1, 2, 3, 4, 5};
    assert(find_missing_number(arr2, 5) == 6);

    int arr3[] = {1, 2, 4, 5, 6};
    assert(find_missing_number(arr3, 5) == 3);

    int arr4[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr4[j++] = i;
    }
    assert(find_missing_number(arr4, 9999) == 5000);

    int arr5[] = {1};
    assert(find_missing_number(arr5, 1) == 2);

    int arr6[9999];
    for (int i = 2, j = 0; i <= 10000; i++) {
        arr6[j++] = i;
    }
    assert(find_missing_number(arr6, 9999) == 1);

    int arr7[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr7[j++] = i;
    }
    assert(find_missing_number(arr7, 9999) == 5000);

    int arr9[] = {1, 2, 3, 4};
    assert(find_missing_number(arr9, 4) == 5);

    int arr10[] = {2, 3, 4, 5};
    assert(find_missing_number(arr10, 4) == 1);

    printf("✓ All tests passed for find_missing_number.\n");
}

int main() {
    run_tests();
    return 0;
}
