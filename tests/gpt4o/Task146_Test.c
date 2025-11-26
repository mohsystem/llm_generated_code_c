#include <stdio.h>
#include <assert.h>

int findMissingNumber(int arr[], int n) {
    int total_sum = n * (n + 1) / 2;
    int array_sum = 0;
    for (int i = 0; i < n - 1; i++) {
        array_sum += arr[i];
    }
    return total_sum - array_sum;
}

void run_tests() {
    int arr1[] = {2, 3, 4, 5, 6};
    assert(findMissingNumber(arr1, 6) == 1);

    int arr2[] = {1, 2, 3, 4, 5};
    assert(findMissingNumber(arr2, 6) == 6);

    int arr3[] = {1, 2, 4, 5, 6};
    assert(findMissingNumber(arr3, 6) == 3);

    int arr4[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr4[j++] = i;
    }
    assert(findMissingNumber(arr4, 10000) == 5000);

    int arr5[] = {1};
    assert(findMissingNumber(arr5, 2) == 2);

    int arr6[9999];
    for (int i = 2, j = 0; i <= 10000; i++) {
        arr6[j++] = i;
    }
    assert(findMissingNumber(arr6, 10000) == 1);

    int arr7[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr7[j++] = i;
    }
    assert(findMissingNumber(arr7, 10000) == 5000);

    int arr9[] = {1, 2, 3, 4};
    assert(findMissingNumber(arr9, 5) == 5);

    int arr10[] = {2, 3, 4, 5};
    assert(findMissingNumber(arr10, 5) == 1);

    printf("✓ All tests passed for findMissingNumber.\n");
}

int main() {
    run_tests();
    return 0;
}
