#include <stdio.h>
#include <stdlib.h>

int* countSmaller(int* nums, int numsSize, int* returnSize){
    int* result = (int*)malloc(numsSize * sizeof(int));
    *returnSize = numsSize;
    for (int i = 0; i < numsSize; i++) {
        result[i] = 0;
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[j] < nums[i]) {
                result[i]++;
            }
        }
    }
    return result;
}

int arraysEqual(int* a, int* b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void runTest(int* nums, int numsSize, int* expected, int testNum) {
    int returnSize = 0;
    int* result = countSmaller(nums, numsSize, &returnSize);

    if (returnSize != numsSize) {
        printf("Test %d: FAIL (return size mismatch)\n", testNum);
    } else if (arraysEqual(result, expected, numsSize)) {
        printf("Test %d: PASS\n", testNum);
    } else {
        printf("Test %d: FAIL\nExpected: [", testNum);
        for (int i = 0; i < numsSize; i++) {
            printf("%d%s", expected[i], (i == numsSize - 1) ? "]" : ", ");
        }
        printf("\nGot:      [");
        for (int i = 0; i < numsSize; i++) {
            printf("%d%s", result[i], (i == numsSize - 1) ? "]\n" : ", ");
        }
    }
    free(result);
}

int main() {
    int test1[] = {5, 2, 6, 1};
    int expect1[] = {2, 1, 1, 0};
    runTest(test1, 4, expect1, 1);

    int test2[] = {-1};
    int expect2[] = {0};
    runTest(test2, 1, expect2, 2);

    int test3[] = {-1, -1};
    int expect3[] = {0, 0};
    runTest(test3, 2, expect3, 3);

    int test4[] = {1, 2, 3, 4};
    int expect4[] = {0, 0, 0, 0};
    runTest(test4, 4, expect4, 4);

    int test5[] = {4, 3, 2, 1};
    int expect5[] = {3, 2, 1, 0};
    runTest(test5, 4, expect5, 5);

    int test6[] = {2, 0, 1};
    int expect6[] = {2, 0, 0};
    runTest(test6, 3, expect6, 6);

    int test7[] = {10, 3, 2, 5};
    int expect7[] = {3, 1, 0, 0};
    runTest(test7, 4, expect7, 7);

    int test8[] = {1, 1, 1, 1};
    int expect8[] = {0, 0, 0, 0};
    runTest(test8, 4, expect8, 8);

    int test9[] = {6, 1, 2, 7, 1};
    int expect9[] = {3, 0, 1, 1, 0};
    runTest(test9, 5, expect9, 9);

    int test10[] = {5, 9, 2, 8, 6};
    int expect10[] = {1, 3, 0, 1, 0};
    runTest(test10, 5, expect10, 10);

    return 0;
}
