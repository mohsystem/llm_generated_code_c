#include <stdio.h>
#include <stdlib.h>

typedef struct MountainArray {
    int* arr;
    int size;
    int calls;
} MountainArray;

MountainArray* createMountainArray(int* arr, int size) {
    MountainArray* mountainArr = (MountainArray*)malloc(sizeof(MountainArray));
    mountainArr->arr = arr;
    mountainArr->size = size;
    mountainArr->calls = 0;
    return mountainArr;
}

int get(MountainArray* mountainArr, int index) {
    mountainArr->calls++;
    if (mountainArr->calls > 100) {
        printf("Too many calls\n");
        exit(1);
    }
    return mountainArr->arr[index];
}

int length(MountainArray* mountainArr) {
    return mountainArr->size;
}

int findPeakIndex(MountainArray* arr) {
    int left = 0, right = length(arr) - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        if (get(arr, mid) < get(arr, mid + 1)) left = mid + 1;
        else right = mid;
    }
    return left;
}

int binarySearch(MountainArray* arr, int target, int left, int right, int ascending) {
    while (left <= right) {
        int mid = (left + right) / 2;
        int value = get(arr, mid);
        if (value == target) return mid;
        if (value < target) {
            if (ascending) left = mid + 1;
            else right = mid - 1;
        } else {
            if (ascending) right = mid - 1;
            else left = mid + 1;
        }
    }
    return -1;
}

int findInMountainArray(int target, MountainArray* mountainArr) {
    int peak = findPeakIndex(mountainArr);
    int index = binarySearch(mountainArr, target, 0, peak, 1);
    if (index != -1) return index;
    return binarySearch(mountainArr, target, peak + 1, length(mountainArr) - 1, 0);
}

void runTest(int* arr, int size, int target, int expected, const char* testName) {
    MountainArray* mountainArr = createMountainArray(arr, size);
    int result = findInMountainArray(target, mountainArr);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
    free(mountainArr);
}

int main() {
    int arr1[] = {1,2,3,4,5,3,1};
    runTest(arr1, 7, 3, 2, "Test Case 1");
    runTest(arr1, 7, 1, 0, "Test Case 2"); // min index of 1 is 0

    int arr2[] = {0,1,2,4,2,1};
    runTest(arr2, 6, 3, -1, "Test Case 3");

    int arr3[] = {1,3,5,7,6,4,2};
    runTest(arr3, 7, 7, 3, "Test Case 4");

    int arr4[] = {5,4,3,2,1,0};
    runTest(arr4, 6, 5, 0, "Test Case 5");

    int arr5[] = {1,2,3,4,3,2,1};
    runTest(arr5, 7, 1, 0, "Test Case 6");

    // Large mountain array: 1..10000 then 9999..1
    int* largeArr = malloc(sizeof(int)*19999);
    for (int i=0; i<10000; i++) largeArr[i] = i+1;
    for (int i=0; i<9999; i++) largeArr[10000+i] = 9999 - i;

    runTest(largeArr, 19999, 5000, 4999, "Test Case 7");
    runTest(largeArr, 19999, 9998, 9997, "Test Case 8");
    runTest(largeArr, 19999, 10000, -1, "Test Case 9");

    free(largeArr);

    int arr6[] = {1,2,3};
    runTest(arr6, 3, 2, 1, "Test Case 10");

    return 0;
}
