#include <stdio.h>
#include <assert.h>

int findMissingNumber(int arr[], int size) {
    int n = size + 1;
    int expectedSum = n * (n + 1) / 2;
    int actualSum = 0;
    for (int i = 0; i < size; i++) {
        actualSum += arr[i];
    }
    return expectedSum - actualSum;
}

void run_tests() {
    int arr1[] = {2, 3, 4, 5, 6};
    assert(findMissingNumber(arr1, 5) == 1);

    int arr2[] = {1, 2, 3, 4, 5};
    assert(findMissingNumber(arr2, 5) == 6);

    int arr3[] = {1, 2, 4, 5, 6};
    assert(findMissingNumber(arr3, 5) == 3);

    int arr4[9999];
    for (int i = 1, j=0; i <= 10000; i++) {
        if (i != 5000) {
            arr4[j++] = i;
        }
    }
    assert(findMissingNumber(arr4, 9999) == 5000);

    int arr5[] = {1};
    assert(findMissingNumber(arr5, 1) == 2);

    int arr6[9999];
    for (int i = 2, j=0; i <= 10000; i++) {
        arr6[j++] = i;
    }
    assert(findMissingNumber(arr6, 9999) == 1);

    int arr7[9999];
    for (int i = 1, j=0; i <= 10000; i++) {
        if (i != 5000) {
            arr7[j++] = i;
        }
    }
    assert(findMissingNumber(arr7, 9999) == 5000);

    int arr8[] = {10, 11, 13, 14, 15};
    assert(findMissingNumber(arr8, 5) == 12);

    int arr9[] = {1, 2, 3, 4};
    assert(findMissingNumber(arr9, 4) == 5);

    int arr10[] = {2, 3, 4, 5};
    assert(findMissingNumber(arr10, 4) == 1);

    printf("✓ All tests passed for findMissingNumber.\n");
}

int main() {
    run_tests();
    return 0;
}
