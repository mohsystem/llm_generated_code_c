#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void posNegSort(int arr[], int n) {
    int pos[n], pos_i = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) {
            pos[pos_i++] = arr[i];
        }
    }
    bubbleSort(pos, pos_i);
    pos_i = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) {
            arr[i] = pos[pos_i++];
        }
    }
}

int arraysEqual(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void runTest(int* input, int inputSize, int* expected, int expectedSize, int testNum) {
    int* arrCopy = (int*)malloc(inputSize * sizeof(int));
    for (int i = 0; i < inputSize; i++) {
        arrCopy[i] = input[i];
    }

    posNegSort(arrCopy, inputSize);

    if (inputSize == expectedSize && arraysEqual(arrCopy, expected, expectedSize)) {
        printf("Test case %d: PASS\n", testNum);
    } else {
        printf("Test case %d: FAIL\n", testNum);
    }
    free(arrCopy);
}

int main() {
    int test1[] = {6, 3, -2, 5, -8, 2, -2};
    int expected1[] = {2, 3, -2, 5, -8, 6, -2};

    int test2[] = {6, 5, 4, -1, 3, 2, -1, 1};
    int expected2[] = {1, 2, 3, -1, 4, 5, -1, 6};

    int test3[] = {-5, -5, -5, -5, 7, -5};
    int expected3[] = {-5, -5, -5, -5, 7, -5};

    int test4[] = {};
    int expected4[] = {};

    int test5[] = {10, -10, 20, -20};
    int expected5[] = {10, -10, 20, -20};

    int test6[] = {9, 3, -3, -2, 7};
    int expected6[] = {3, 7, -3, -2, 9};

    int test7[] = {5, 4, -1, -2, 1};
    int expected7[] = {1, 4, -1, -2, 5};

    int test8[] = {-3, -2, -1};
    int expected8[] = {-3, -2, -1};

    int test9[] = {100, -50, 75, -25};
    int expected9[] = {75, -50, 100, -25};

    int test10[] = {2, 1, -9, -8};
    int expected10[] = {1, 2, -9, -8};

    runTest(test1, sizeof(test1)/sizeof(test1[0]), expected1, sizeof(expected1)/sizeof(expected1[0]), 1);
    runTest(test2, sizeof(test2)/sizeof(test2[0]), expected2, sizeof(expected2)/sizeof(expected2[0]), 2);
    runTest(test3, sizeof(test3)/sizeof(test3[0]), expected3, sizeof(expected3)/sizeof(expected3[0]), 3);
    runTest(test4, sizeof(test4)/sizeof(test4[0]), expected4, sizeof(expected4)/sizeof(expected4[0]), 4);
    runTest(test5, sizeof(test5)/sizeof(test5[0]), expected5, sizeof(expected5)/sizeof(expected5[0]), 5);
    runTest(test6, sizeof(test6)/sizeof(test6[0]), expected6, sizeof(expected6)/sizeof(expected6[0]), 6);
    runTest(test7, sizeof(test7)/sizeof(test7[0]), expected7, sizeof(expected7)/sizeof(expected7[0]), 7);
    runTest(test8, sizeof(test8)/sizeof(test8[0]), expected8, sizeof(expected8)/sizeof(expected8[0]), 8);
    runTest(test9, sizeof(test9)/sizeof(test9[0]), expected9, sizeof(expected9)/sizeof(expected9[0]), 9);
    runTest(test10, sizeof(test10)/sizeof(test10[0]), expected10, sizeof(expected10)/sizeof(expected10[0]), 10);

    return 0;
}
