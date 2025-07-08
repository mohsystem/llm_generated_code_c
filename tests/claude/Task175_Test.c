#include <stdio.h>
#include <stdlib.h>

struct MountainArray {
    int* arr;
    int size;
};

int get(struct MountainArray* mountainArr, int index) {
    if (index < 0 || index >= mountainArr->size) {
        printf("Index out of bounds\n");
        exit(1);
    }
    return mountainArr->arr[index];
}

int length1(struct MountainArray* mountainArr) {
    return mountainArr->size;
}

int findInMountainArray(int target, struct MountainArray* mountainArr) {
    int length = length1(mountainArr);

    // Find peak element index
    int left = 0, right = length - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (get(mountainArr, mid) < get(mountainArr, mid + 1)) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    int peak = left;

    // Binary search ascending part
    left = 0;
    right = peak;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int val = get(mountainArr, mid);
        if (val == target) return mid;
        else if (val < target) left = mid + 1;
        else right = mid - 1;
    }

    // Binary search descending part
    left = peak + 1;
    right = length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int val = get(mountainArr, mid);
        if (val == target) return mid;
        else if (val > target) left = mid + 1;
        else right = mid - 1;
    }

    return -1;
}

// دالة لمساعدة إنشاء MountainArray
struct MountainArray createMountainArray(int* arr, int size) {
    struct MountainArray mountainArr;
    mountainArr.arr = arr;
    mountainArr.size = size;
    return mountainArr;
}

void runTest(int* arr, int size, int target, int expected, const char* testName) {
    struct MountainArray mountainArr = createMountainArray(arr, size);
    int result = findInMountainArray(target, &mountainArr);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", testName, expected, result);
    }
}

int main() {
    int arr1[] = {1,2,3,4,5,3,1};
    runTest(arr1, 7, 3, 2, "Test Case 1");
    runTest(arr1, 7, 1, 0, "Test Case 2");  // min index of 1 is 0

    int arr2[] = {0,1,2,4,2,1};
    runTest(arr2, 6, 3, -1, "Test Case 3");

    int arr3[] = {1,3,5,7,6,4,2};
    runTest(arr3, 7, 7, 3, "Test Case 4");

    int arr4[] = {5,4,3,2,1,0};
    runTest(arr4, 6, 5, 0, "Test Case 5");

    int arr5[] = {1,2,3,4,3,2,1};
    runTest(arr5, 7, 1, 0, "Test Case 6");

    // large mountain array 1..10000 and 9999..1
    int* largeArr = (int*)malloc(19999 * sizeof(int));
    for (int i = 0; i < 10000; i++) largeArr[i] = i + 1;
    for (int i = 0; i < 9999; i++) largeArr[10000 + i] = 9999 - i;

    runTest(largeArr, 19999, 5000, 4999, "Test Case 7");
    runTest(largeArr, 19999, 9998, 9997, "Test Case 8");
    runTest(largeArr, 19999, 10000, -1, "Test Case 9");

    free(largeArr);

    int arr6[] = {1,2,3};
    runTest(arr6, 3, 2, 1, "Test Case 10");

    return 0;
}
