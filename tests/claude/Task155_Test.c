#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest])
        largest = l;
    if (r < n && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        int swap = arr[i];
        arr[i] = arr[largest];
        arr[largest] = swap;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

int isSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

void printArray(int arr[], int n) {
    for (int i=0; i<n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void testHeapSort() {
    // Test Case 1
    int arr1[] = {4, 10, 3, 5, 1};
    int n1 = sizeof(arr1)/sizeof(arr1[0]);
    heapSort(arr1, n1);
    assert(isSorted(arr1, n1));

    // Test Case 2: Already sorted array
    int arr2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(arr2)/sizeof(arr2[0]);
    heapSort(arr2, n2);
    assert(isSorted(arr2, n2));

    // Test Case 3: Reverse sorted array
    int arr3[] = {5, 4, 3, 2, 1};
    int n3 = sizeof(arr3)/sizeof(arr3[0]);
    heapSort(arr3, n3);
    assert(isSorted(arr3, n3));

    // Test Case 4: Array with duplicates
    int arr4[] = {4, 2, 2, 8, 3, 3, 1};
    int n4 = sizeof(arr4)/sizeof(arr4[0]);
    heapSort(arr4, n4);
    assert(isSorted(arr4, n4));

    // Test Case 5: Single element array
    int arr5[] = {1};
    int n5 = sizeof(arr5)/sizeof(arr5[0]);
    heapSort(arr5, n5);
    assert(isSorted(arr5, n5));

    // Test Case 6: Empty array
    int arr6[] = {};
    int n6 = 0;
    heapSort(arr6, n6);
    assert(isSorted(arr6, n6));

    printf("All test cases passed!\n");
}

int main() {
    testHeapSort();
    return 0;
}
