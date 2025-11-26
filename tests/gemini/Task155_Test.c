#include <stdio.h>
#include <assert.h>

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i >= 0; i--) {
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

void testHeapSort() {
    int arr1[] = {12, 11, 13, 5, 6, 7};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    heapSort(arr1, n1);
    assert(isSorted(arr1, n1));

    int arr2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    heapSort(arr2, n2);
    assert(isSorted(arr2, n2));

    int arr3[] = {5, 4, 3, 2, 1};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    heapSort(arr3, n3);
    assert(isSorted(arr3, n3));

    int arr4[] = {4, 2, 2, 8, 3, 3, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    heapSort(arr4, n4);
    assert(isSorted(arr4, n4));

    int arr5[] = {1};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    heapSort(arr5, n5);
    assert(isSorted(arr5, n5));

    int arr6[] = {};
    int n6 = 0;
    heapSort(arr6, n6);
    assert(isSorted(arr6, n6));

    printf("All test cases passed!\n");
}

int main() {
    testHeapSort();

    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, n);

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
